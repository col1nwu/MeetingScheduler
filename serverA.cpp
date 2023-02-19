#include "utils.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

const string IP_ADDR = "127.0.0.1";
const int SERVERM_PORT = 23092;
const int SERVERA_PORT = 21092;
const string SERVER_ID = "A";

map<string, string> proc_inp_file() {
	ifstream file("a.txt");
	if (!file.is_open())
	{
		cerr << "[ServerA] Cannot open input file..." << endl;
		exit(1);
	}

	map<string, string> avals;
	string line;
	while (getline(file, line))
	{
		string new_line;
		for (char curr_char : line)
		{
			if (curr_char != ' ') new_line += curr_char;
		}

		vector<string> usr_aval = split_str(new_line, ';');
		avals[usr_aval[0]] = usr_aval[1];
	}

	file.close();

	return avals;
}

int main(int argc, char *argv[])
{
	cout << "Server " << SERVER_ID << " is up and running using UDP on port " << SERVERA_PORT << "." << endl;

	int sock_fd = init_udp_sock(IP_ADDR, SERVERA_PORT);
	struct sockaddr_in addr_serverM = init_dest_addr_udp(IP_ADDR, SERVERM_PORT);

	map<string, string> avals = proc_inp_file();

	vector<string> usrs;
	for (auto kv : avals)
	{
		usrs.push_back(kv.first);
	}

	send_msg_udp(sock_fd, addr_serverM, vec_to_str(usrs, ","));

	while (true)
	{
		string req_usrs_msg = recv_msg(sock_fd);

		cout << "Server " << SERVER_ID << " received the usernames from Main Server using UDP over port " << SERVERA_PORT << "." << endl;

		vector<string> req_usrs = split_str(req_usrs_msg, ',');
		vector<string> tss;
		for (string usr : req_usrs)
		{
			tss.push_back(avals[usr]);
		}

		string intxns = find_intxn(tss);
		cout << "Found the intersection result: " << intxns << " for " << vec_to_str(req_usrs, ", ") << "." << endl;

		send_msg_udp(sock_fd, addr_serverM, intxns);
		cout << "Server " << SERVER_ID << " finished sending the response to Main Server." << endl;
	}

	return 0;
}