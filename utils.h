#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.h
 * This is header file for utils.cpp. Other files can include this header to use utility functions.
 * 
 * @date April 23, 2023
 * @author Colin Wu, daizongw@usc.edu
 */ 

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

/*
-------------------------------------------------
  Utility functions for TCP and UDP connections
-------------------------------------------------
*/

struct recv_struct
{
	string msg;
	int port_num;
};

recv_struct recv_msg(int sock_fd);

int init_udp_sock(string ip_addr, int port_num);
struct sockaddr_in init_dest_addr_udp(string ip_addr, int port_num);
void send_msg_udp(int sock_fd, struct sockaddr_in addr_dest, string msg);

int acpt_tcp_conn(int sock_fd_loc);
void send_msg_tcp(int sock_fd, string msg);

vector<string> split_str(string str, string x);
string map_to_str(map<string, string> map);
map<string, string> str_to_map(string str);
string vec_to_str(vector<string> strs, string x);
set<string> vec_to_set(vector<string> strs);
string ext_str(string str, string start_wrd, string end_wrd);

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

vector<ts> str_to_ts(string str);
string find_intxn(string avals1, string avals2);
string find_intxn(vector<string> avals);
bool is_valid_ts(ts a, vector<ts> tss);
map<string, string> update_avals(map<string, string> avals, vector<string> usrs, string mtg_time);

#endif
