/**
 * @file client.cpp
 * Implementation of Client
 * 
 * Client is responsible for taking users' inputs and sending them to Main Server via TCP.
 * 
 * @date April 23, 2023
 * @author Colin Wu, daizongw@usc.edu
 */

#include "utils.h"

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

const string IP_ADDR = "127.0.0.1";
const int PORT_TCP = 24092;

/**
 * Initializes TCP socket at client side.
 * 
 * @param ip_addr IP address of client
 * @param port_num Port number of client
 * @return socket descriptor (int) of client
 */
int init_tcp_sock(string ip_addr, int port_num)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	addr.sin_port = htons(port_num);

	connect(sock_fd, (struct sockaddr *) &addr, sizeof(addr));

	return sock_fd;
}

/**
 * Retrieves TCP port number at client side.
 * 
 * @param sock_fd Socket descriptor of TCP at client side
 * @return dynamically assigned port number of TCP at client side
 */
int get_tcp_port(int sock_fd)
{
	sockaddr_in addr;
	socklen_t len_addr = sizeof(addr);
	getsockname(sock_fd, (struct sockaddr *) &addr, &len_addr);
	return ntohs(addr.sin_port);
}

int main(int argc, char *argv[])
{
	// initialize TCP connection between Client and Main Server
	int sock_fd = init_tcp_sock(IP_ADDR, PORT_TCP);

	// retrieve dynamically assigned TCP port number
	int port = get_tcp_port(sock_fd);

	cout << "Client is up and running." << endl;

	while (true)
	{
		// prompt user to enter usernames to request intersections of availabilities
		cout << "Please enter the usernames to check schedule availability:" << endl;
		string inp;
		getline(cin, inp);

		// check if input is valid; if not, prompt again
		vector<string> inp_usrs = split_str(inp, " ");
		bool is_valid_usr = (0 < inp_usrs.size() && inp_usrs.size() <= 10);
		while (!is_valid_usr)
		{
			cout << "The usernames you entered are invalid. Please enter again:" << endl;
			getline(cin, inp);
			inp_usrs = split_str(inp, " ");
			is_valid_usr = (0 < inp_usrs.size() && inp_usrs.size() <= 10);
		}

		// send requested usernames to Main Server
		send_msg_tcp(sock_fd, inp);
		cout << "Client finished sending the usernames to Main Server." << endl;

		// receive response from Main Server about intersections of all requested usernames
		string msg = recv_msg(sock_fd).msg;
		// if there are non-existing usernames, show this message; otherwise show intersections
		if (msg[0] != 'T')
		{
			cout << "Client received the reply from Main Server using TCP over port " << port << ":" << endl;
			cout << msg << " do not exist." << endl;
			msg = recv_msg(sock_fd).msg;

			if (msg == "Start new request") continue;
		}
		cout << "Client received the reply from Main Server using TCP over port " << port << ":" << endl;
		cout << msg << "." << endl;

		// extracted intersections and existing usernames from Main Server's message
		string avals = ext_str(msg, "intervals", "works");
		vector<ts> aval_ts = str_to_ts(avals);
		string usrs = ext_str(msg, "for", "");

		// prompt client to enter meeting time to register
		cout << "Please enter the final meeting time to register a meeting:" << endl;
		string mtg_time;
		getline(cin, mtg_time);
		vector<ts> inp_ts = str_to_ts(mtg_time);

		// check if entered meeting time is valid; if not, prompt again
		if (inp_ts.size() > 0)
		{
			bool is_valid_time = is_valid_ts(inp_ts[0], aval_ts);
			while (!is_valid_time)
			{
				cout << "Time interval " << mtg_time << " is not valid. Please enter again:" << endl;
				getline(cin, mtg_time);
				inp_ts = str_to_ts(mtg_time);
				is_valid_time = is_valid_ts(inp_ts[0], aval_ts);
			}
		}
		else mtg_time = "[]";

		// if input is valid, send to Main Server to register
		send_msg_tcp(sock_fd, mtg_time);
		cout << "Sent the request to register " << mtg_time << " as the meeting time for " << usrs << "." << endl;

		// receive from Main Server that registration completes
		string reg_notif = recv_msg(sock_fd).msg;
		cout << "Received the notification that registration has finished." << endl;

		// start new request
		cout << "-----Start a new request-----" << endl;
	}

	close(sock_fd);

	return 0;
}