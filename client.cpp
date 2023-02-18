#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

const char* IP_ADDR = "127.0.0.1";
const int PORT_TCP = 24092;

int init_tcp_sock() {
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		cerr << "[Client] Cannot initailze TCP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr_serverM_tcp;
	memset(&addr_serverM_tcp, 0, sizeof(addr_serverM_tcp));
	addr_serverM_tcp.sin_family = AF_INET;
	addr_serverM_tcp.sin_addr.s_addr = inet_addr(IP_ADDR);
	addr_serverM_tcp.sin_port = htons(PORT_TCP);

	int conn_fd = connect(sock_fd, (struct sockaddr *) &addr_serverM_tcp, sizeof(addr_serverM_tcp));
	if (conn_fd < 0)
	{
		cerr << "[Client] Cannot connect to Main Server..." << endl;
		exit(1);
	}

	return sock_fd;
}

void send_msg(int sock_fd, string msg)
{
	int nbytes = send(sock_fd, msg.c_str(), msg.length(), 0);
	if (nbytes < 0)
	{
		cerr << "[Client] Cannot send message to Main Server..." << endl;
		close(sock_fd);
	}
	cout << "Client finished sending the usernames to Main Server." << endl;
}

int main(int argc, char *argv[])
{
	int sock_fd = init_tcp_sock();

	cout << "Client is up and running." << endl;

	while (true)
	{
		cout << "Please enter the usernames to check schedule availability:" << endl;
		string inp;
		getline(cin, inp);

		send_msg(sock_fd, inp);

		cout << "-----Start a new request-----" << endl;
	}

	close(sock_fd);

	return 0;
}