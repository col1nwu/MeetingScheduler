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

int get_tcp_port(int sock_fd)
{
	sockaddr_in addr;
	socklen_t len_addr = sizeof(addr);
	getsockname(sock_fd, (struct sockaddr *) &addr, &len_addr);
	return ntohs(addr.sin_port);
}

int main(int argc, char *argv[])
{
	int sock_fd = init_tcp_sock(IP_ADDR, PORT_TCP);
	int port = get_tcp_port(sock_fd);

	cout << "Client is up and running." << endl;

	while (true)
	{
		cout << "Please enter the usernames to check schedule availability:" << endl;
		string inp;
		getline(cin, inp);

		send_msg_tcp(sock_fd, inp);
		cout << "Client finished sending the usernames to Main Server." << endl;

		string msg = recv_msg(sock_fd);
		if (msg[0] != '[')
		{
			cout << "Client received the reply from Main Server using TCP over port " << port << ": ";
			cout << msg << " do not exist." << endl;
			msg = recv_msg(sock_fd);
		}
		cout << "Client received the reply from Main Server using TCP over port " << port << ": ";

		cout << "-----Start a new request-----" << endl;
	}

	close(sock_fd);

	return 0;
}