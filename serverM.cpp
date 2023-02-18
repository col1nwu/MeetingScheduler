#include "utils.h"

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

int init_udp_sock() {
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0)
	{
		cerr << "[ServerM] Cannot initialize UDP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	addr.sin_port = htons(PORT_UDP);

	int bind_res = bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr));
	if (bind_res < 0)
	{
		cerr << "[ServerM] Cannot bind UDP socket to address..." << endl;
		exit(1);
	}

	return sock_fd;
}

int init_tcp_sock() {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		cerr << "[ServerM] Cannot initailze TCP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	addr.sin_port = htons(PORT_TCP);

	int bind_res = bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr));
	if (bind_res < 0)
	{
		cerr << "[ServerM] Cannot bind TCP socket to address..." << endl;
		exit(1);
	}

	int lsen_res = listen(sock_fd, 5);
	if (lsen_res < 0)
	{
		cerr << "[ServerM] Cannot listen for connections..." << endl;
		exit(1);
	}

	return sock_fd;
}

string recv_msg_udp(int sock_fd)
{
	char buffer[1024];
	struct sockaddr_in addr_sndr;
	socklen_t len_sndr = sizeof(addr_sndr);
	int len_resp = recvfrom(sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_sndr, &len_sndr);
	if (len_resp < 0)
	{
		cerr << "[ServerM] Cannot receive data via UDP..." << endl;
		close(sock_fd);
	}
	buffer[len_resp] = '\0';

	return string(buffer);
}

int main(int argc, char *argv[])
{
	int sock_fd_udp = init_udp_sock();
	int sock_fd_tcp = init_tcp_sock();

	cout << "Main Server is up and running." << endl;

	string str_serverA = recv_msg_udp(sock_fd_udp);
	cout << "Main Server received the username list from server A using UDP over port 21092." << endl;
	// string str_serverB = recv_msg_udp(sock_fd_udp);

	map<string, string> usr_serverA = str_to_map(str_serverA);
	// map<string, string> usr_serverB = str_to_map(str_serverB);

	while (true)
	{
		struct sockaddr_in addr_cli;
		socklen_t len_cli = sizeof(addr_cli);
		int sock_cli = accept(sock_fd_tcp, (struct sockaddr *) &addr_cli, &len_cli);
		if (sock_cli < 0) {
			cerr << "[ServerM] Cannot accept incoming connections..." << endl;
		}

		while (true)
		{
			char buffer[1024];
			struct sockaddr_in addr_sndr;
			socklen_t len_sndr = sizeof(addr_sndr);
			int len_resp = recvfrom(sock_cli, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr_sndr, &len_sndr);
			if (len_resp < 0)
			{
				cerr << "[ServerM] Cannot receive data from Client..." << endl;
				close(sock_cli);
			}
			else if (len_resp > 0)
			{
				char ip_cli[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &addr_cli.sin_addr, ip_cli, INET_ADDRSTRLEN);
				int port_cli = ntohs(addr_cli.sin_port);
				cout << "Main Server received the request from client using TCP over port " << port_cli << "." << endl;
			}
			buffer[len_resp] = '\0';

			string resp = string(buffer);
			vector<string> usrs = split_str(resp);

			vector<string> req_serverA;
			
		}
	}

	close(sock_fd_udp);
	close(sock_fd_tcp);

	return 0;
}