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

		bool is_valid_usr = (inp != "" && split_str(inp, " ").size() > 10);
		while (!is_valid_usr)
		{
			cout << "The usernames you entered are invalid. Please enter again:" << endl;
			getline(cin, inp);
			is_valid_usr = (inp != "" && split_str(inp, " ").size() > 10);
		}

		send_msg_tcp(sock_fd, inp);
		cout << "Client finished sending the usernames to Main Server." << endl;

		string msg = recv_msg(sock_fd).msg;
		if (msg[0] != 'T')
		{
			cout << "Client received the reply from Main Server using TCP over port " << port << ":" << endl;
			cout << msg << " do not exist." << endl;
			msg = recv_msg(sock_fd).msg;
		}
		cout << "Client received the reply from Main Server using TCP over port " << port << ":" << endl;
		cout << msg << "." << endl;

		string avals = ext_str(msg, "intervals", "works");
		vector<ts> aval_ts = str_to_ts(avals);
		string usrs = ext_str(msg, "for", "");

		cout << "Please enter the final meeting time to register a meeting:" << endl;
		string mtg_time;
		getline(cin, mtg_time);
		vector<ts> inp_ts = str_to_ts(mtg_time);

		bool is_valid_time = is_valid_ts(inp_ts[0], aval_ts);
		while (!is_valid_time)
		{
			cout << "Time interval " << mtg_time << " is not valid. Please enter again:" << endl;
			getline(cin, mtg_time);
			inp_ts = str_to_ts(mtg_time);
			is_valid_time = is_valid_ts(inp_ts[0], aval_ts);
		}

		send_msg_tcp(sock_fd, mtg_time);
		cout << "Sent the request to register " << mtg_time << " as the meeting time for " << usrs << "." << endl;

		string reg_notif = recv_msg(sock_fd).msg;
		cout << "Received the notification that registration has finished." << endl;
		
		cout << "-----Start a new request-----" << endl;
	}

	close(sock_fd);

	return 0;
}