//
//  kanoodle.cpp
//  math
//
//  Created by Clayton Knittel on 1/1/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "kanoodle.hpp"
#include "knuth.h"

#include <fstream>
#include <string>

using std::ifstream;
using std::string;


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
	for (int i = 0; i < 55; i++)
		board[i] = '_';
	for (const string &s : res) {
		string name;
		istringstream is(s);
		is >> name;
		string tile;
		while (is >> tile) {
			vector<string> v;
			int x, y;
			v = split(tile, ",");
			istringstream is1(v[0]);
			istringstream is2(v[1]);
			is1 >> x;
			is2 >> y;
			board[x + 11 * y] = name[0];
		}
	}
	for (int i = 0; i < 55; i++) {
		cout << board[i] << " ";
		if (i % 11 == 10)
			cout << endl;
	}
}

void print3DBoard(const vector<string> &res) {
	char *board[5];
	int lens[5] = {25, 16, 9, 4, 1};
	for (int i = 0; i < 5; i++) {
		board[i] = new char[lens[i]];
		for (int j = 0; j < lens[i]; j++)
			board[i][j] = '_';
	}
	for (const string &s : res) {
		string name;
		istringstream is(s);
		is >> name;
		string tile;
		while (is >> tile) {
			vector<string> v;
			int x, y, z;
			v = split(tile, ",");
			istringstream is1(v[0]);
			istringstream is2(v[1]);
			istringstream is3(v[2]);
			is1 >> x;
			is2 >> y;
			is3 >> z;
			board[z][x + (5 - z) * y] = name[0];
		}
	}
	
	for (int j = 4; j >= 0; j--) {
		for (int i = 0; i < lens[j]; i++) {
			cout << board[j][i] << " ";
			if ((i + 1) % (5 - j) == 0)
				cout << endl;
		}
		cout << endl;
	}
}

void write_file(const char* setup_file, const char* pieces_file="/users/claytonknitte/documents/xcode/math/math/kanoodle/kan.txt") {
	vector<int> decisions;
	
}

void run() {
	dlx d = init("/users/claytonknittel/documents/xcode/math/math/kanoodle/pyrvert.txt");
	if (d == nullptr)
		return;
	vector<vector<int>> v;
	solve(d, v);
	
	cout << "num solutions: " << v.size() << endl;
	
	vector<string> best;
	int maxDifZ = 12;
	for (auto it = v.begin(); it != v.end(); it++) {
		vector<string> decs;
		getOptions(d, decs, *it);
		int difZCount = 0;
		for (const string &s : decs) {
			int zz = -1;
			string name;
			istringstream is(s);
			is >> name;
			string tile;
			while (is >> tile) {
				int z;
				istringstream is3(tile.substr(4, 5));
				is3 >> z;
				
				if (zz == -1)
					zz = z;
				else if (zz != z) {
					difZCount++;
					break;
				}
			}
		}
		if (difZCount < maxDifZ) {
			maxDifZ = difZCount;
			best = decs;
		}
	}
	
	cout << "best difz: " << maxDifZ << endl;
	
	print3DBoard(best);
	
//	vector<int> decs = {1316, 1188, 904, 1380, 1469, 954, 650, 470, 13, 270};//, 97, 775};
//	dlx d = init("/users/claytonknittel/documents/xcode/math/math/kanoodle/kan.txt");
//	makeDecisions(d, decs);
//	write_to_file(d, "/users/claytonknittel/documents/xcode/math/math/kanoodle/kan2.txt", decs);
	
//	enumerate_possibilities("/users/claytonknittel/documents/xcode/math/math/kanoodle/pieces.txt");
	
//	vector<int> v;
//	dlx d = init("/users/claytonknittel/documents/xcode/math/math/kanoodle/kan.txt");
//	vector<string> dec = {"F 8,0 8,1 8,2 7,1 9,1", "K 6,1 6,2 5,2 4,2 4,1"};
//	makeDecisions(d, dec);
//	solveOnce(d, v);
//	cout << v << endl;
//	getOptions("/users/claytonknittel/documents/xcode/math/math/kanoodle/kan.txt", dec, v);
//	printBoard(dec);
}
