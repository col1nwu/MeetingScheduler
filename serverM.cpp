/**
 * @file serverM.cpp
 * Implementation of Main Server
 * 
 * Main server is the heart in this system. It communicates with client via TCP and backend servers
 * via UDP. Main server coordinates the communication between client and backend servers, and it is
 * critical.
 * 
 * @date April 23, 2023
 * @author Colin Wu, daizongw@usc.edu
 */

#include "utils.h"

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <set>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

const string IP_ADDR = "127.0.0.1";
const int PORT_UDP = 23092;
const int PORT_TCP = 24092;
const int PORT_SERVERA = 21092;
const int PORT_SERVERB = 22092;

/**
 * Initialize TCP socket at server side.
 * 
 * @param ip_addr IP address of server
 * @param port_num Port number of server
 * @return socket descriptor (int) of server
 */
int init_tcp_sock(string ip_addr, int port_num)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	addr.sin_port = htons(port_num);

	bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr));

	listen(sock_fd, 1);

	return sock_fd;
}

int main(int argc, char *argv[])
{
	// initialize UDP and TCP sockets
	int sockfd_udp = init_udp_sock(IP_ADDR, PORT_UDP);
	int sockfd_tcp = init_tcp_sock(IP_ADDR, PORT_TCP);

	// retrieve UDP address for backend servers
	struct sockaddr_in addr_serverA = init_dest_addr_udp(IP_ADDR, PORT_SERVERA);
	struct sockaddr_in addr_serverB = init_dest_addr_udp(IP_ADDR, PORT_SERVERB);

	cout << "Main Server is up and running." << endl;

	// receive username list from backend servers
	set<string> usrs_serverA, usrs_serverB;
	for (int i = 0; i < 2; i++)
	{
		recv_struct msg_udp = recv_msg(sockfd_udp);
		vector<string> usrs_server_vec = split_str(msg_udp.msg, ",");
		if (msg_udp.port_num == PORT_SERVERA)
		{
			cout << "Main Server received the username list from server A using UDP over port " << PORT_UDP << "." << endl;
			usrs_serverA = vec_to_set(usrs_server_vec);
		}
		else if (msg_udp.port_num == PORT_SERVERB)
		{
			cout << "Main Server received the username list from server B using UDP over port " << PORT_UDP << "." << endl;
			usrs_serverB = vec_to_set(usrs_server_vec);
		}
	}

	while (true)
	{
		// accept TCP connections from client
		int sockfd_rmt = acpt_tcp_conn(sockfd_tcp);

		while (true)
		{
			// receive request, containing username list, from client
			string msg = recv_msg(sockfd_rmt).msg;
			if (msg.length() == 0) break;
			cout << "Main Server received the request from client using TCP over port " << PORT_TCP << "." << endl;

			// find which server contains availabilities of requested usernames
			vector<string> usrs = split_str(msg, " ");
			vector<string> req_serverA, req_serverB, not_fnd;
			for (string usr: usrs)
			{
				if (usrs_serverA.find(usr) != end(usrs_serverA)) req_serverA.push_back(usr);
				else if (usrs_serverB.find(usr) != end(usrs_serverB)) req_serverB.push_back(usr);
				else not_fnd.push_back(usr);
			}

			// inform client if any of requested usernames do not exist at backend
			if (not_fnd.size() > 0)
			{
				string not_fnd_usrs = vec_to_str(not_fnd, ", ");
				cout << not_fnd_usrs << " do not exist. Send a reply to the client." << endl;
				send_msg_tcp(sockfd_rmt, not_fnd_usrs);
			}

			// contact backend servers if users are found in that server
			// do nothing if no user is found in that server
			bool is_serverA = false, is_serverB = false;
			if (req_serverA.size() > 0)
			{
				cout << "Found " << vec_to_str(req_serverA, ", ") << " located at Server A. Send to Server A." << endl;
				send_msg_udp(sockfd_udp, addr_serverA, vec_to_str(req_serverA, ","));
				is_serverA = true;
			}
			if (req_serverB.size() > 0)
			{
				cout << "Found " << vec_to_str(req_serverB, ", ") << " located at Server B. Send to Server B." << endl;
				send_msg_udp(sockfd_udp, addr_serverB, vec_to_str(req_serverB, ","));
				is_serverB = true;
			}

			// wait for backend servers to send back availabilities of users on that server
			int flag = 0;
			if (is_serverA && is_serverB) flag = 2;
			else if (is_serverA || is_serverB) flag = 1;

			string intxns_serverA, intxns_serverB;
			for (int i = 0; i < flag; i++)
			{
				recv_struct intxns_msg = recv_msg(sockfd_udp);
				if (intxns_msg.port_num == PORT_SERVERA)
				{
					cout << "Main Server received from server A the intersection result using UDP over port " << PORT_UDP << ":" << endl;
					intxns_serverA = intxns_msg.msg;
					cout << intxns_serverA << "." << endl;
				}
				else if (intxns_msg.port_num == PORT_SERVERB)
				{
					cout << "Main Server received from server B the intersection result using UDP over port " << PORT_UDP << ":" << endl;
					intxns_serverB = intxns_msg.msg;
					cout << intxns_serverB << "." << endl;
				}
			}

			// find intersection of results from two backend servers
			// if users are all at one server, output result from backend server directly
			// output "[]" if no user is entered or empty result
			string intxns = "[]";
			if (is_serverA && is_serverB)
			{
				intxns = find_intxn(intxns_serverA, intxns_serverB);
				cout << "Found the intersection between the results from server A and B:" << endl;
				cout << intxns << "." << endl;
			}
			else if (is_serverA)
			{
				intxns = intxns_serverA;
				cout << "Found the intersection between the results from server A:" << endl;
				cout << intxns << "." << endl;
			}
			else if (is_serverB)
			{
				intxns = intxns_serverB;
				cout << "Found the intersection between the results from server B:" << endl;
				cout << intxns << "." << endl;
			}

			// send intersection to client
			req_serverA.insert(req_serverA.end(), req_serverB.begin(), req_serverB.end());
			string res = "Time intervals " + intxns + " works for " + vec_to_str(req_serverA, ", ");
			send_msg_tcp(sockfd_rmt, res);
			cout << "Main Server sent the result to the client." << endl;

			// receive meeting time from client
			string mtg_time = recv_msg(sockfd_rmt).msg;

			// register meeting time for requested users
			if (is_serverA) send_msg_udp(sockfd_udp, addr_serverA, mtg_time);
			if (is_serverB) send_msg_udp(sockfd_udp, addr_serverB, mtg_time);

			// wait for backend servers to send back registration confirmation
			for (int i = 0; i < flag; i++)
			{
				string reg_notif = recv_msg(sockfd_udp).msg;
			}
			send_msg_tcp(sockfd_rmt, "Registration completed at backend Servers.");
		}
	}

	close(sockfd_udp);
	close(sockfd_tcp);

	return 0;
}