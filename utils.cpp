#include "utils.h"

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

string recv_msg(int sock_fd)
{
	char buffer[1024];

	struct sockaddr_in addr_rmt;
	socklen_t len_rmt = sizeof(addr_rmt);
	int len_resp = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_rmt, &len_rmt);
	if (len_resp > 0)
	{
		buffer[len_resp] = '\0';
		return string(buffer);
	}
	else if (len_resp < 0) cout << "Error in receiving message..." << endl;

	return "";
}

/*
-------------------------------------------------
      Utility functions for UDP connections
-------------------------------------------------
*/

int init_udp_sock(string ip_addr, int port_num)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	addr.sin_port = htons(port_num);

	bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr));

	return sock_fd;
}

struct sockaddr_in init_dest_addr_udp(string ip_addr, int port_num)
{
	struct sockaddr_in dest_addr;
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	dest_addr.sin_port = htons(port_num);

	return dest_addr;
}

void send_msg_udp(int sock_fd, struct sockaddr_in addr_dest, string msg)
{
	// can use a variable to contain the result
	sendto(sock_fd, msg.c_str(), msg.length(), 0, (struct sockaddr *) &addr_dest, sizeof(addr_dest));
}

/*
-------------------------------------------------
      Utility functions for TCP connections
-------------------------------------------------
*/

int acpt_tcp_conn(int sock_fd_loc)
{
	struct sockaddr_in addr_rmt;
	socklen_t len_rmt = sizeof(addr_rmt);
	
	int sock_fd = accept(sock_fd_loc, (struct sockaddr *) &addr_rmt, &len_rmt);

	return sock_fd;
}

void send_msg_tcp(int sock_fd, string msg)
{
	send(sock_fd, msg.c_str(), msg.length(), 0);
}

/*
-------------------------------------------------
           Other utility functions
-------------------------------------------------
*/

vector<string> split_str(string str, char x)
{
	vector<string> words;
	string word;
	istringstream iss(str);

	while (getline(iss, word, x))
	{
		words.push_back(word);
	}

	return words;
}

string map_to_str(map<string, string> map)
{
	string str;
	for (auto kv : map)
	{
		str += kv.first + ";";
		str += kv.second + " ";
	}
	return str;
}

map<string, string> str_to_map(string str)
{
	map<string, string> map;

	vector<string> usr_avals = split_str(str, ' ');
	for (string usr_aval : usr_avals)
	{
		vector<string> tmp = split_str(usr_aval, ';');
	}
	return map;
}

string vec_to_str(vector<string> strs, string x)
{
	string a;

	size_t i = 0;
	for (string str : strs)
	{
		a += str;
		++i;
		if (i < strs.size()) a += x;
	}

	return a;
}

/*
-------------------------------------------------
        Interval Intersection Alogorithm
-------------------------------------------------
*/

struct ts
{
	int start;
	int end;
};

bool comp_ts(ts ts1, ts ts2)
{
	return ts1.start < ts2.start;
}

vector<ts> str_to_ts(string str)
{
	string nums_str;
	for (char a : str)
	{
		if (a == '[' || a == ']') continue;
		if (a == ',') nums_str += ' ';
		else nums_str += a;
	}
	vector<string> nums = split_str(nums_str, ' ');
	vector<ts> tss;
	size_t i = 0;
	while (i < nums.size())
	{
		tss.push_back({stoi(nums[i]), stoi(nums[++i])});
		++i;
	}
	return tss;
}

string ts_to_str(vector<ts> tss)
{
	int n = tss.size();
	string str = "[";
	for (int i = 0; i < n; i++)
	{
		str += "[";
		str += to_string(tss[i].start);
		str += ",";
		str += to_string(tss[i].end);
		str += "]";
		if (i < n - 1) str += ",";
	}
	str += "]";
	return str;
}

string find_intxn(string avals1, string avals2)
{
	vector<ts> intxns;
	vector<ts> tss1 = str_to_ts(avals1), tss2 = str_to_ts(avals2);
	
	size_t i = 0, j = 0;
	while (i < tss1.size() && j < tss2.size())
	{
		int start = max(tss1[i].start, tss2[j].start);
		int end = min(tss1[i].end, tss2[j].end);
		if (start < end) intxns.push_back({start, end});
		if (tss1[i].end < tss2[j].end) ++i;
		else ++j;
	}

	return ts_to_str(intxns);
}

string find_intxn(vector<string> avals)
{
	int n = avals.size();
	if (n == 0) return "[]";
	if (n == 1) return avals[0];
	string tmp = find_intxn(avals[0], avals[1]);
	for (int i = 2; i < n; i++)
	{
		tmp = find_intxn(tmp, avals[i]);
	}
	return tmp;
}

// int main()
// {
// 	vector<string> avals;
// 	avals.push_back("[[1,10],[11,12]]");
// 	avals.push_back("[[5,9],[11,15]]");
// 	avals.push_back("[[4,12]]");
// 	cout << find_intxn(avals) << endl;
// 	return 0;
// }