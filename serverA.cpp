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

int init_udp_sock() {
	int sockdp = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockdp < 0)
	{
		cerr << "[ServerA] Cannot initialize UDP socket..." << endl;
		exit(1);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	addr.sin_port = htons(SERVERA_PORT);

	int bind_res = bind(sockdp, (struct sockaddr *) &addr, sizeof(addr));
	if (bind_res < 0)
	{
		cerr << "[ServerA] Cannot bind UDP socket to address..." << endl;
		exit(1);
	}

	return sockdp;
}

struct sockaddr_in init_serverM_addr()
{
	struct sockaddr_in serverM_addr;
	memset(&serverM_addr, 0, sizeof(serverM_addr));
	serverM_addr.sin_family = AF_INET;
	serverM_addr.sin_addr.s_addr = inet_addr(IP_ADDR.c_str());
	serverM_addr.sin_port = htons(SERVERM_PORT);

	return serverM_addr;
}

void send_msg(int sock_fd, struct sockaddr_in addr_dest, string msg)
{
	struct sockaddr_in addr_cli;
	socklen_t len_addr_cli = sizeof(addr_cli);
	int nbytes = sendto(sock_fd, msg.c_str(), msg.length(), 0, (struct sockaddr *) &addr_dest, sizeof(addr_dest));
	if (nbytes < 0)
	{
		cerr << "[ServerA] Cannot send message to Main Server..." << endl;
		close(sock_fd);
	}
}

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
	cout << "Server A is up and running using UDP on port 21092." << endl;

	struct sockaddr_in addr_serverM = init_serverM_addr();
	int sock_fd = init_udp_sock();

	map<string, string> avals = proc_inp_file();

	send_msg(sock_fd, addr_serverM, map_to_str(avals));

	return 0;
}