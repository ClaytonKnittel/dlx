//
//  main.cpp
//  math
//
//  Created by Clayton Knittel on 9/16/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <iostream>
#include "knuth.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::istringstream;

std::ostream &operator<<(std::ostream &o, vector<int> &v) {
	o << "[";
	for (auto it = v.begin(); it != v.end(); it++) {
		o << *it;
		if (it + 1 != v.end())
			o << ", ";
	}
	o << "]";
	return o;
}

std::ostream &operator<<(std::ostream &o, vector<string> &v) {
	o << "[";
	for (auto it = v.begin(); it != v.end(); it++) {
		o << *it;
		if (it + 1 != v.end())
			o << ",\n";
	}
	o << "]";
	return o;
}

vector<string> split(string s, string del) {
	size_t pos = 0;
	string token;
	vector<string> ret;
	while ((pos = s.find(del)) != string::npos) {
		token = s.substr(0, pos);
		ret.push_back(token);
		s.erase(0, pos + del.length());
	}
	ret.push_back(s);
	return ret;
}

void printBoard(const vector<string> &res) {
	char board[55];
	for (const string &s : res) {
		char c;
		string name;
		istringstream is(s);
		is >> name;
		if (name == "w0")
			c = 'a';
		else if (name == "w1")
			c = 'b';
		else if (name == "w2")
			c = 'c';
		else if (name == "w3")
			c = 'd';
		else if (name == "w4")
			c = 'e';
		else if (name == "w5")
			c = 'f';
		else if (name == "w6")
			c = 'g';
		else if (name == "w7")
			c = 'h';
		else if (name == "w8")
			c = 'i';
		else if (name == "w9")
			c = 'j';
		else if (name == "w10")
			c = 'k';
		else if (name == "w11")
			c = 'l';
		else
			c = '_';
		string tile;
		while (is >> tile) {
			vector<string> v;
			int x, y;
			v = split(tile, ",");
			istringstream is1(v[0]);
			istringstream is2(v[1]);
			is1 >> x;
			is2 >> y;
			board[x + 11 * y] = c;
		}
	}
	for (int i = 0; i < 55; i++) {
		cout << board[i] << " ";
		if (i % 11 == 10)
			cout << endl;
	}
}

int main(int argc, const char * argv[]) {
	vector<int> v;
    solveOnce("/users/claytonknittel/documents/xcode/math/math/kan.txt", v);
	cout << v << endl;
	vector<string> res;
	getOptions("/users/claytonknittel/documents/xcode/math/math/kan.txt", res, v);
	printBoard(res);
    return 0;
}
