/**
 * @file utils.cpp
 * Implementation of a list of utility functions
 * 
 * These utilities functions are commonly used throughout the project, so they were extracted
 * to reduce redundancy.
 * 
 * @date April 23, 2023
 * @author Colin Wu, daizongw@usc.edu
 */

#include "utils.h"

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

/**
 * Receives message from remote.
 * 
 * TCP and UDP connections can both use this function.
 * 
 * @param sock_fd Socket descriptor of remote
 * @return message from remote and port number of remote
 */
recv_struct recv_msg(int sock_fd)
{
	char buffer[1024];

	struct sockaddr_in addr_rmt;
	socklen_t len_rmt = sizeof(addr_rmt);
	int len_resp = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_rmt, &len_rmt);
	if (len_resp > 0)
	{
		buffer[len_resp] = '\0';
		return {string(buffer), ntohs(addr_rmt.sin_port)};
	}
	else if (len_resp < 0) cout << "Error in receiving message..." << endl;

	return {"", -1};
}

/*
-------------------------------------------------
      Utility functions for UDP connections
-------------------------------------------------
*/

/**
 * Initializes UDP socket.
 * 
 * @param ip_addr IP address of host
 * @param port_num Port number of host
 * @return socket descriptor (int) of host
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

/**
 * Initializes destination UDP socket.
 * 
 * @param ip_addr IP address of remote host
 * @param port_num Port number of remote host
 * @return socket descriptor (int) of remote host
 */
struct sockaddr_in init_dest_addr_udp(string ip_addr, int port_num)
{
	struct sockaddr_in dest_addr;
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	dest_addr.sin_port = htons(port_num);

	return dest_addr;
}

/**
 * Sends UDP message from host to remote host.
 * 
 * @param sock_fd Socket descriptor of UDP socket on host
 * @param sockaddr_in UDP Socket descriptor of UDP socket on remote host
 * @param msg Message that needs to be transmitted 
 * @return void
 */
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

/**
 * Accepts TCP connection.
 * 
 * @param sock_fd Socket descriptor of TCP socket
 * @return socket descriptor of child TCP process
 */
int acpt_tcp_conn(int sock_fd_loc)
{
	struct sockaddr_in addr_rmt;
	socklen_t len_rmt = sizeof(addr_rmt);
	
	int sock_fd = accept(sock_fd_loc, (struct sockaddr *) &addr_rmt, &len_rmt);

	return sock_fd;
}

/**
 * Sends TCP message from host to remote host.
 * 
 * @param sock_fd Socket descriptor of TCP socket on host
 * @param msg Message that needs to be transmitted 
 * @return void
 */
void send_msg_tcp(int sock_fd, string msg)
{
	send(sock_fd, msg.c_str(), msg.length(), 0);
}

/*
-------------------------------------------------
           Other utility functions
-------------------------------------------------
*/

/**
 * Splits string by delimiter
 * 
 * @param str String to split
 * @param x Delimiter used to split string
 * @return vector of strings containing substrings
 */
vector<string> split_str(string str, string x)
{
	vector<string> words;
	size_t i = 0;

	while ((i = str.find(x)) != string::npos)
	{
		string word = str.substr(0, i);
		if (word != "") words.push_back(word);
		str.erase(0, i + x.length());
	}

	if (str != "") words.push_back(str);
	return words;
}

/**
 * Converts a map into string representation.
 * 
 * @param map Map to convert
 * @return string representing key-value pairs in map
 */
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

/**
 * Converts string into map of key-value pairs.
 * 
 * @param str String to convert
 * @return map containing key-value pairs parsed from string
 */
map<string, string> str_to_map(string str)
{
	map<string, string> map;

	vector<string> usr_avals = split_str(str, " ");
	for (string usr_aval : usr_avals)
	{
		vector<string> tmp = split_str(usr_aval, ";");
	}
	return map;
}

/**
 * Converts vector into string representation.
 * 
 * @param strs Vector of strings to convert
 * @param x Separator of strings
 * @return string representing elements in vector
 */
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

/**
 * Converts vector into set representation.
 * 
 * @param strs Vector of strings to convert
 * @return set representing elements in vector
 */
set<string> vec_to_set(vector<string> strs)
{
	set<string> set_str(strs.begin(), strs.end());
	return set_str;
}

/**
 * Extracts information from string (retrieves substring from string).
 * 
 * @param str String containing information
 * @param start_wrd Start word of substring. If empty, starts from beginning
 * @param end_wrd End word of substring. If empty, goes towards end
 * @return information wanted
 */
string ext_str(string str, string start_wrd, string end_wrd)
{
	if (start_wrd == "")
	{
		size_t end_i = str.find(end_wrd);

		if (end_i == 0 || end_i == string::npos) return "";
		--end_i;

		return str.substr(0, end_i);
	}
	else if (end_wrd == "")
	{
		size_t start_i = str.find(start_wrd);
		
		if (start_i == string::npos) return "";
		start_i += start_wrd.length() + 1;
		
		return str.substr(start_i);
	}
	else {
		size_t start_i = str.find(start_wrd);
		size_t end_i = str.find(end_wrd);

		if (start_i == string::npos || end_i == string::npos || end_i <= start_i) return "";

		start_i += start_wrd.length() + 1;
		--end_i;

		return str.substr(start_i, end_i - start_i);
	}
}

/*
-------------------------------------------------
        Interval Intersection Alogorithm
-------------------------------------------------
*/

/**
 * Converts string to vector of timeslots.
 * 
 * @param str String to convert
 * @return vector of timeslots
 */
vector<ts> str_to_ts(string str)
{
	string new_str;
	for (char a : str)
	{
		if (a != ' ') new_str += a;
	}

	string nums_str;
	for (char a : new_str)
	{
		if (a == '[' || a == ']') continue;
		if (a == ',') nums_str += ' ';
		else nums_str += a;
	}
	vector<string> nums = split_str(nums_str, " ");
	vector<ts> tss;
	size_t i = 0;
	while (i < nums.size())
	{
		tss.push_back({stoi(nums[i]), stoi(nums[++i])});
		++i;
	}
	return tss;
}

/**
 * Converts vector of timeslots to string.
 * 
 * @param tss vector of timeslots to convert
 * @return string representing all timeslots
 */
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

/**
 * Finds intersections between two lists of timeslots (representing in string)
 * 
 * @param avals1 first list of timeslots
 * @param avals2 second list of timeslots
 * @return intersections between two lists (representing in string)
 */
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

/**
 * Finds intersections between multiple lists of timeslots (representing in string)
 * 
 * @param avals Multiple lists of timeslots
 * @return intersections among these lists (representing in string)
 */
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

/**
 * Checks if a timeslot is within user's availabilities
 * 
 * @param a Timeslot to check
 * @param tss User's availabilities
 * @return true if timeslot is valid, otherwise false
 */
bool is_valid_ts(ts a, vector<ts> tss)
{
	int min_start = a.start, max_end = a.end;
	for (ts intvl : tss)
	{
		if (intvl.start > min_start) return false;
		if (intvl.end >= max_end) return true;
		max_end = max(max_end, intvl.end);
	}
	return false;
}

/**
 * Registers meeting time and updates availabilities for a list of users
 * 
 * @param avals Multiple lists of timeslots
 * @param users List of users to update
 * @param mtg_time Meeting time to register
 * @return updated availabilities
 */
map<string, string> update_avals(map<string, string> avals, vector<string> usrs, string mtg_time)
{
	// if meeting time is empty, then we don't need to update availability table
	if (mtg_time == "[]") return avals;

	// make a deep copy of current availabilities
	map<string, string> new_avals;
	for (auto kv = avals.begin(); kv != avals.end(); ++kv)
	{
		new_avals[kv->first] = kv->second;
	}

	ts mtg = str_to_ts(mtg_time)[0];
	for (string usr : usrs)
	{
		vector<ts> new_ts;
		string orig_ts_str = avals[usr];
		vector<ts> orig_ts = str_to_ts(orig_ts_str);
		for (ts intvl : orig_ts)
		{
			if (intvl.end <= mtg.start || intvl.start >= mtg.end) new_ts.push_back(intvl);
			else
			{
				if (intvl.start < mtg.start) new_ts.push_back({intvl.start, mtg.start});
				if (intvl.end > mtg.end) new_ts.push_back({mtg.end, intvl.end});
			}
		}
		new_avals[usr] = ts_to_str(new_ts);
	}
	return new_avals;
}

// int main()
// {
// 	vector<ts> strs = str_to_ts("[]");
// 	cout << strs.size() << endl;
// 	return 0;
// }