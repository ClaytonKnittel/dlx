//
//  nqueens.cpp
//  math
//
//  Created by Clayton Knittel on 1/7/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "nqueens.hpp"
#include "knuth.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

using std::cout;
using std::endl;
using std::ofstream;
using std::istringstream;

typedef unsigned char uchar;
const uchar start = ':' + 1;

void write(const char* file_loc, uchar nn) {
	ofstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to create file at " << file_loc << endl;
		return;
	}
	
	f << nn * nn + 2 * nn + 2 * (2 * nn - 1) << " " << nn * nn << " " << nn * nn * 5 << "\n";
	
	for (uchar i = 0; i < nn; i++) {
		if (static_cast<uchar>(start + i) < start) {
			cout << "out of bounds!" << endl;
			return;
		}
		f << "r" << static_cast<uchar>(start + i) << " c" << static_cast<uchar>(start + i) << " ";
	}
	f << "|";
	
	for (uchar i = 0; i < nn; i++) {
		for (uchar j = 0; j < nn; j++) {
			f << " t" << static_cast<uchar>(start + i) << static_cast<uchar>(start + j);
		}
	}
	
	for (uchar i = 0; i < 2 * nn - 1; i++) {
		f << " dl" << static_cast<uchar>(start + i) << " dr" << static_cast<uchar>(start + i);
	}
	
	for (uchar i = 0; i < nn; i++) {
		for (uchar j = 0; j < nn; j++) {
			f << "\nt" << static_cast<uchar>(start + i) << static_cast<uchar>(start + j) << " r" << static_cast<uchar>(start + i) << " c" << static_cast<uchar>(start + j) << " dl" << static_cast<uchar>(start + (nn + j - i - 1)) << " dr" << static_cast<uchar>(start + (i + j));
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
		int i = s[1] - start;
		int j = s[2] - start;
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

void nqueens(int size) {
	write("/users/claytonknittel/documents/xcode/math/math/nqueens/pieces.txt", size);
	dlx d = init("/users/claytonknittel/documents/xcode/math/math/nqueens/pieces.txt");

	printf("Number of solutions: %lu\n", countSolns(d));

	// vector<vector<int>> solns;
	// solve(d, solns);
	// vector<int> soln;
	
	// printf("begin\n");
	// clock_t t = clock();
	// solveOnce(d, soln);
	// t = clock() - t;
	// printf("%f ms.\n", 1000.f * static_cast<float>(t) / CLOCKS_PER_SEC);
	
	// vector<string> res;
	// getOptions(d, res, soln);
	// print(res);

	// cout << "number of solutions: " << solns.size() << endl;

	// for (auto soln : solns) {
	// 	vector<string> c;
	// 	getOptions(d, c, soln);
	// 	print(c);
	// }
}
