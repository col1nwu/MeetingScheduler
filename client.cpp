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
	int sockdp = socket(AF_INET, SOCK_STREAM, 0);
	if (sockdp < 0)
	{
		cerr << "[Client] Cannot initailze TCP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr_serverM_tcp;
	memset(&addr_serverM_tcp, 0, sizeof(addr_serverM_tcp));
	addr_serverM_tcp.sin_family = AF_INET;
	addr_serverM_tcp.sin_addr.s_addr = inet_addr(IP_ADDR);
	addr_serverM_tcp.sin_port = htons(PORT_TCP);

	int conn_fd = connect(sockdp, (struct sockaddr *) &addr_serverM_tcp, sizeof(addr_serverM_tcp));
	if (conn_fd < 0) {
		cerr << "[Client] Cannot connect to serverM..." << endl;
		exit(1);
	}

	return sockdp;
}

int main(int argc, char *argv[])
{
	int sockdp_cli = init_tcp_sock();

	cout << "Client is up and running." << endl;

	while (true)
	{
		cout << "Please enter the usernames to check schedule availability:" << endl;
		string inp;
		getline(cin, inp);

		int nbytes = send(sockdp_cli, inp.c_str(), inp.length(), 0);
		if (nbytes < 0) {
			cerr << "[Client] Cannot send message to server..." << endl;
			close(sockdp_cli);
			return 1;
		}
		cout << "Client finished sending the usernames to Main Server." << endl;


		cout << "-----Start a new request-----" << endl;
	}

	close(sockdp_cli);

	return 0;
}