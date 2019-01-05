//
//  sudoku.cpp
//  math
//
//  Created by Clayton Knittel on 1/3/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "sudoku.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "knuth.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::istringstream;
using std::ostringstream;

struct sudoku {
	int board[81];
	
	sudoku() {
		for (int i = 0; i < 81; i++) {
			board[i] = 0;
		}
	}
	
	void play(int i, int j, int num) {
		if (i < 0 || i > 8)
			return;
		if (j < 0 || j > 8)
			return;
		board[i + 9 * (8 - j)] = num;
	}
	
	int get(int i, int j) const {
		if (i < 0 || i > 8)
			return -1;
		if (j < 0 || j > 8)
			return -1;
		return board[i + 9 * (8 - j)];
	}
	
	void clear() {
		for (int i = 0; i < 81; i++)
			board[i] = 0;
	}
	
	void fill(const vector<string> &options) {
		clear();
		for (string s : options) {
			istringstream is(s);
			char sq[3];
			char val[3];
			is >> sq;
			is >> val;
			int i = sq[1] - '0';
			int j = sq[2] - '0';
			int v = val[2] - '0' + 1;
			if (get(i, j) != 0)
				cout << "Duplicate option at tile (" << i << ", " << j << ")" << endl;
			play(i, j, v);
		}
	}
	
	string getOption(int i, int j, int val) {
		ostringstream os;
		os << "p" << i << j << " r" << i << val - 1 << " c" << j << val - 1 << " b" << j / 3 * 3 + (i / 3) % 3 << val - 1;
		return os.str();
	}
	
	bool getOptions(const char* file_loc, vector<string> &options) {
		ifstream f;
		f.open(file_loc);
		if (!f.is_open()) {
			cout << "could not open file at " << file_loc << endl;
			return false;
		}
		
		string line;
		int j = 8;
		while (getline(f, line)) {
			if (line.size() != 9) {
				cout << "corrupt input file, line " << j << " has " << line.size() << " characters, but expect 9" << endl;
				return false;
			}
			for (int i = 0; i < 9; i++) {
				if (line[i] == '_')
					continue;
				int v = line[i] - '0';
				if (v < 1 || v > 9) {
					cout << "invalid character " << line[i] << " in line " << 9 - j;
					return false;
				}
				options.push_back(getOption(i, j, v));
			}
			j--;
		}
		if (j != -1) {
			cout << "corrupt input file, expect 9 lines, but has " << 8 - j << endl;
			return false;
		}
		return true;
	}
	
	void print(std::ostream &o=std::cout) {
		o << "\u250f\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u2533\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u252f\u2501\u2501\u2501\u2513\n";
		for (int i = 0; i < 81; i++) {
			if (i % 3 == 0) {
				o << "\u2503";
			} else {
				o << "\u2502";
			}
			if (board[i] == 0)
				o << " _ ";
			else
				o << " " << board[i] << " ";
			if (i % 9 == 8) {
				o << "\u2503\n";
				if (i != 80) {
					if ((i + 1) % 27 == 0) {
						o << "\u2523\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u254b\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u254b\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u253f\u2501\u2501\u2501\u252b\n";
					}
					else
						o << "\u2520\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u2542\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u2542\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u253c\u2500\u2500\u2500\u2528\n";
				}
			}
		}
		o << "\u2517\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u253b\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u2537\u2501\u2501\u2501\u251b\n";
	}
};


void write_possibilities(const char* file_loc) {
	ofstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "could not open file at " << file_loc << endl;
		return;
	}
	
	f << 81 * 4 << " " << 9 * 81 << " " << 9 * 81 * 4 << "\n";
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			f << "p" << i << j << " ";
			f << "r" << i << j << " ";
			f << "c" << i << j << " ";
			f << "b" << i << j;
			if (i != 8 || j != 8)
				f << " ";
		}
	}
	
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			for (int n = 0; n < 9; n++)
				f << "\np" << i << j << " r" << i << n << " c" << j << n << " b" << j / 3 * 3 + (i / 3) % 3 << n;
	f.close();
}


void rudoku() {
//	write_possibilities("/users/claytonknittel/documents/xcode/math/math/sudoku/input.txt");
	
	dlx d = init("/users/claytonknittel/documents/xcode/math/math/sudoku/input.txt");
	
	vector<vector<int>> vs;
	
	vector<string> decisions;
	sudoku s;
	if (!s.getOptions("/users/claytonknittel/documents/xcode/math/math/sudoku/example.txt", decisions))
		return;
	makeDecisions(d, decisions);
	solve(d, vs);
	
	cout << "Number of solutions: " << vs.size() << endl;
	
	getOptions(d, decisions, vs[0]);
	
//	for (string s : decisions)
//		cout << s << endl;
	
	s.fill(decisions);
	s.print();
}
