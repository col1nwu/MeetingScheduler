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

using namespace std;

const string IP_ADDR = "127.0.0.1";
const int SERVERM_PORT = 23092;
const int SERVERA_PORT = 21092;


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

		int i = 0;
		for (char curr_char : new_line)
		{
			if (curr_char == ';')
			{
				string username = new_line.substr(0, i);
				string aval = new_line.substr(i + 1, new_line.length() - 1);
				avals[username] = aval;
				break;
			}
			++i;
		}
	}

	file.close();

	return avals;
}

int main(int argc, char *argv[])
{
	map<string, string> avals = proc_inp_file();
	cout << avals["eliana"] << endl;
}