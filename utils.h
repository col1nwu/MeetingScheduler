#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <vector>

using namespace std;

vector<string> split_str(string str, char x);
string map_to_str(map<string, string> map);
map<string, string> str_to_map(string str);
string vec_to_str(vector<string> strs);

#endif
