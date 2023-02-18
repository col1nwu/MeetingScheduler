#include "utils.h"

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

string recv_msg_udp(int sock_fd)
{
	char buffer[1024];

	struct sockaddr_in addr_sndr;
	socklen_t len_sndr = sizeof(addr_sndr);
	int len_resp = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_sndr, &len_sndr);
	buffer[len_resp] = '\0';

	return string(buffer);
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

vector<string> recv_msg_tcp(int sock_fd)
{
	vector<string> a;

	char buffer[1024];
	struct sockaddr_in addr_rmt;
	socklen_t len_rmt = sizeof(addr_rmt);
	int len_resp = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_rmt, &len_rmt);
	if (len_resp > 0)
	{
		buffer[len_resp] = '\0';
		string resp = string(buffer);
		a.push_back(resp);

		char ip_rmt[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr_rmt.sin_addr, ip_rmt, INET_ADDRSTRLEN);
		int port_rmt = ntohs(addr_rmt.sin_port);
		a.push_back(to_string(port_rmt));
	}

	return a;
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

// int main()
// {
// 	string a = "123 34 35 23 12";
// 	vector<string> strs = split_str(a, ' ');
// 	for (string str : strs)
// 	{
// 		cout << str << endl;
// 	}
// 	return 0;
// }