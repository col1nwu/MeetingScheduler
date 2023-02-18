#include "utils.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> split_str(string str, char x)
{
	vector<string> words;
	string word;
	istringstream iss(str);

	while (getline(iss, word, x))
	{
		words.push_back(word);
	}

	return words;
}

string map_to_str(map<string, string> map)
{
	string str;
	for (auto kv : map)
	{
		str += kv.first + ";";
		str += kv.second + " ";
	}
	return str;
}

map<string, string> str_to_map(string str)
{
	map<string, string> map;

	vector<string> usr_avals = split_str(str, ' ');
	for (string usr_aval : usr_avals)
	{
		vector<string> tmp = split_str(usr_aval, ';');
	}
	return map;
}

string vec_to_str(vector<string> strs)
{
	string a;

	size_t i = 0;
	for (string str : strs)
	{
		a += str;
		++i;
		if (i < strs.size()) a += " ";
	}

	return a;
}

// int main()
// {
// 	vector<string> words;
// 	words.push_back("123");
// 	words.push_back("324");
// 	cout << vec_to_str(words);
// 	return 0;
// }