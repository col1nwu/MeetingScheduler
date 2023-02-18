#include "utils.h"

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
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

int init_tcp_sock(string ip_addr, int port_num)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
	addr.sin_port = htons(port_num);

	bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr));

	listen(sock_fd, 5);

	return sock_fd;
}

int main(int argc, char *argv[])
{
	int sockfd_udp = init_udp_sock(IP_ADDR, PORT_UDP);
	int sockfd_tcp = init_tcp_sock(IP_ADDR, PORT_TCP);

	struct sockaddr_in addr_serverA = init_dest_addr_udp(IP_ADDR, PORT_SERVERA);

	cout << "Main Server is up and running." << endl;

	string usrs_serverA_msg = recv_msg_udp(sockfd_udp);
	cout << "Main Server received the username list from server A using UDP over port 21092." << endl;
	// string str_serverB = recv_msg_udp(sockfd_udp);

	vector<string> usrs_serverA = split_str(usrs_serverA_msg, ',');
	// map<string, string> usrs_serverB = str_to_map(str_serverB);

	while (true)
	{
		int sockfd_rmt = acpt_tcp_conn(sockfd_tcp);

		while (true)
		{
			vector<string> msg_port = recv_msg_tcp(sockfd_rmt);
			if (msg_port.size() == 0) break;
			cout << "Main Server received the request from client using TCP over port " << msg_port[1] << "." << endl;

			vector<string> usrs = split_str(msg_port[0], ' ');
			vector<string> req_serverA, not_fnd;
			for (string usr: usrs)
			{
				if (find(usrs_serverA.begin(), usrs_serverA.end(), usr) != usrs_serverA.end()) req_serverA.push_back(usr);
				else not_fnd.push_back(usr);
			}
			if (not_fnd.size() > 0) {
				string not_fnd_usrs = vec_to_str(not_fnd, ", ");
				cout << not_fnd_usrs << " do not exist. Send a reply to the client." << endl;
				send_msg_tcp(sockfd_rmt, not_fnd_usrs);
			}
			cout << "Found " << vec_to_str(req_serverA, ", ") << " located at Server A. Send to Server A." << endl;
			send_msg_udp(sockfd_udp, addr_serverA, vec_to_str(req_serverA, ","));
		}
	}

	close(sockfd_udp);
	close(sockfd_tcp);

	return 0;
}