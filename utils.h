#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <vector>

using namespace std;

string recv_msg(int sock_fd);

int init_udp_sock(string ip_addr, int port_num);
struct sockaddr_in init_dest_addr_udp(string ip_addr, int port_num);
void send_msg_udp(int sock_fd, struct sockaddr_in addr_dest, string msg);

int acpt_tcp_conn(int sock_fd_loc);
// vector<string> recv_msg_tcp(int sock_fd);
void send_msg_tcp(int sock_fd, string msg);

vector<string> split_str(string str, char x);
string map_to_str(map<string, string> map);
map<string, string> str_to_map(string str);
string vec_to_str(vector<string> strs, string x);

string find_intxn(vector<string> avals);

#endif
