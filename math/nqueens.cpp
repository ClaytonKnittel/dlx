//
//  nqueens.cpp
//  math
//
//  Created by Clayton Knittel on 1/7/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "nqueens.hpp"
#include "knuth.h"

#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ofstream;
using std::istringstream;


void write(const char* file_loc, int nn) {
	ofstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to create file at " << file_loc << endl;
		return;
	}
	char n = nn + '0';
	
	f << nn * nn + 2 * nn + 2 * (2 * nn - 1) << " " << nn * nn << " " << nn * nn * 5 << "\n";
	
	for (char i = '0'; i < n; i++) {
		f << "r" << i << " c" << i << " ";
	}
	f << "|";
	
	for (char i = '0'; i < n; i++) {
		for (char j = '0'; j < n; j++) {
			f << " t" << i << j;
		}
	}
	for (char i = '0'; i < 2 * nn - 1 + '0'; i++) {
		f << " dl" << i << " dr" << i;
	}
	
	for (char i = '0'; i < n; i++) {
		for (char j = '0'; j < n; j++) {
			f << "\nt" << i << j << " r" << i << " c" << j << " dl" << static_cast<char>(n + j - i - 1) << " dr" << static_cast<char>(i + j - '0');
		}
	}
	f.close();
}

void print(vector<string> options, std::ostream &o=cout) {
	int width = static_cast<int>(options.size());
	o << "\u250f";
	for (int i = 0; i < width - 1; i++)
		o << "\u2501\u2501\u2501\u252f";
	o << "\u2501\u2501\u2501\u2513\n";
	
	bool board[width * width];
	for (int i = 0; i < width * width; i++)
		board[i] = false;
	
	for (string s : options) {
		int i = s[1] - '0';
		int j = s[2] - '0';
		board[i + width * j] = true;
	}
	
	for (int i = 0; i < width * width; i++) {
		if (i % width == 0)
			o << "\u2503";
		else
			o << "\u2502";
		if (board[i] == 0)
			o << "   ";
		else
			o << " q ";
		if (i % width == width - 1) {
			o << "\u2503\n";
			if (i != width * width - 1) {
				o << "\u2520";
				for (int i = 0; i < width - 1; i++)
					o << "\u2500\u2500\u2500\u253c";
				o << "\u2500\u2500\u2500\u2528\n";
			}
		}
	}
	o << "\u2517";
	for (int i = 0; i < width - 1; i++)
		o << "\u2501\u2501\u2501\u2537";
	o << "\u2501\u2501\u2501\u251b\n";
}

void nqueens() {
	write("/users/claytonknittel/documents/xcode/math/math/nqueens/pieces.txt", 8);
	dlx d = init("/users/claytonknittel/documents/xcode/math/math/nqueens/pieces.txt");

	vector<vector<int>> solns;
	solve(d, solns);

	cout << "number of solutions: " << solns.size() << endl;

//	for (auto soln : solns) {
//		vector<string> c;
//		getOptions(d, c, soln);
//		print(c);
//	}
}
