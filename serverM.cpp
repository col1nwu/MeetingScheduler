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
const int PORT_UDP = 23092;
const int PORT_TCP = 24092;

int init_udp_sock() {
	int sockdp = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockdp < 0)
	{
		cerr << "[ServerM] Cannot initialize UDP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr_serverM_udp;
	memset(&addr_serverM_udp, 0, sizeof(addr_serverM_udp));
	addr_serverM_udp.sin_family = AF_INET;
	addr_serverM_udp.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	addr_serverM_udp.sin_port = htons(PORT_UDP);

	int bind_res = bind(sockdp, (struct sockaddr *) &addr_serverM_udp, sizeof(addr_serverM_udp));
	if (bind_res < 0)
	{
		cerr << "[ServerM] Cannot bind UDP socket to address..." << endl;
		exit(1);
	}

	return sockdp;
}

int init_tcp_sock() {
	int sockdp = socket(AF_INET, SOCK_STREAM, 0);
	if (sockdp < 0)
	{
		cerr << "[ServerM] Cannot initailze TCP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr_serverM_tcp;
	memset(&addr_serverM_tcp, 0, sizeof(addr_serverM_tcp));
	addr_serverM_tcp.sin_family = AF_INET;
	addr_serverM_tcp.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	addr_serverM_tcp.sin_port = htons(PORT_TCP);

	int bind_res = bind(sockdp, (struct sockaddr *) &addr_serverM_tcp, sizeof(addr_serverM_tcp));
	if (bind_res < 0) {
		cerr << "[ServerM] Cannot bind TCP socket to address..." << endl;
		exit(1);
	}

	int lsen_res = listen(sockdp, 5);
	if (lsen_res < 0) {
		cerr << "[ServerM] Cannot listen for connections..." << endl;
		exit(1);
	}

	return sockdp;
}

int main(int argc, char *argv[])
{
	int sockdp_udp = init_udp_sock();
	int sockdp_tcp = init_tcp_sock();

	cout << "The main server is up and running" << endl;

	while (true)
	{
		struct sockaddr_in addr_cli;
		socklen_t len_cli = sizeof(addr_cli);
		int sock_cli = accept(sockdp_tcp, (struct sockaddr *) &addr_cli, &len_cli);
		if (sock_cli < 0) {
			cerr << "[ServerM] Cannot accept incoming connections..." << endl;
			continue;
		}

		char buffer_cli[1024];
		struct sockaddr_in addr_sdr;
		socklen_t len_sdr = sizeof(addr_sdr);
		int len_resp = recvfrom(sock_cli, buffer_cli, sizeof(buffer_cli), 0, (struct sockaddr *) &addr_sdr, &len_sdr);
		if (len_resp < 0)
		{
			cerr << "[ServerM] Cannot receive data from client..." << endl;
			close(sock_cli);
			continue;
		}

		buffer_cli[len_resp] = '\0';
		cout << "Received message: " << buffer_cli << endl;

		close(sock_cli);
	}

	close(sockdp_udp);

	return 0;
}