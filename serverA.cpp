/**
 * @file serverA.cpp
 * Implementation of the backend server A
 * 
 * Server A first reads users' availabilities from text files and then communicates with Main
 * Server via UDP.
 * 
 * @date April 23, 2023
 * @author Colin Wu, daizongw@usc.edu
 */

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

/**
 * Read input file and store users' availabilities in a map
 * 
 * @return a map contains users' availabilities with username as key and availability (string) as value
 */
map<string, string> proc_inp_file() {
	// open input file stream and read file
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
		// remove any space that represents in a line
		for (char curr_char : line)
		{
			if (curr_char != ' ') new_line += curr_char;
		}

		// split line by semicolon to get username and availabilities
		vector<string> usr_aval = split_str(new_line, ";");
		avals[usr_aval[0]] = usr_aval[1];
	}

	file.close();

	return avals;
}

int main(int argc, char *argv[])
{
	cout << "Server " << SERVER_ID << " is up and running using UDP on port " << SERVERA_PORT << "." << endl;

	// initialize UDP connections between Server A and Main Server
	int sock_fd = init_udp_sock(IP_ADDR, SERVERA_PORT);
	struct sockaddr_in addr_serverM = init_dest_addr_udp(IP_ADDR, SERVERM_PORT);

	// read input file
	map<string, string> avals = proc_inp_file();

	// send users of Server A to Main Server
	vector<string> usrs;
	for (auto kv : avals)
	{
		usrs.push_back(kv.first);
	}
	send_msg_udp(sock_fd, addr_serverM, vec_to_str(usrs, ","));
	cout << "Server " << SERVER_ID << " finished sending a list of usernames to Main Server." << endl;

	while (true)
	{
		// receive requested users from Main Server
		string req_usrs_msg = recv_msg(sock_fd).msg;
		cout << "Server " << SERVER_ID << " received the usernames from Main Server using UDP over port " << SERVERA_PORT << "." << endl;

		// find time slots for requested users
		vector<string> req_usrs = split_str(req_usrs_msg, ",");
		vector<string> tss;
		for (string usr : req_usrs)
		{
			tss.push_back(avals[usr]);
		}

		// run algorithm to find overlapped time slots
		string intxns = find_intxn(tss);
		cout << "Found the intersection result: " << intxns << " for " << vec_to_str(req_usrs, ", ") << "." << endl;

		// send them to Main Server
		send_msg_udp(sock_fd, addr_serverM, intxns);
		cout << "Server " << SERVER_ID << " finished sending the response to Main Server." << endl;

		// receive scheduled time slot from Main Server
		string mtg_time = recv_msg(sock_fd).msg;
		cout << "Register a meeting at " << mtg_time << " and update the availability for the following users:" << endl;

		// update each requested user based on scheduled time slot (remove scheduled time slots from availabilities)
		map<string, string> new_avals = update_avals(avals, req_usrs, mtg_time);
		for (string usr : req_usrs)
		{
			cout << usr << ": updated from " << avals[usr] << " to " << new_avals[usr] << endl;
		}
		avals = new_avals;

		// send notifications to Main Server indicating registration completes
		cout << "Notified Main Server that registration has finished." << endl;
		send_msg_udp(sock_fd, addr_serverM, "Registration completed!");
	}

	return 0;
}