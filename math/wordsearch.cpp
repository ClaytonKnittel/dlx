//
//  wordsearch.cpp
//  math
//
//  Created by Clayton Knittel on 1/7/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#define WIDTH 15
#define HEIGHT 15

#include "wordsearch.hpp"
#include "knuth.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;

bool palendrome(const string &s) {
	for (size_t i = 0; i < s.size() / 2; i++) {
		if (s[i] != s[s.size() - 1 - i])
			return false;
	}
	return true;
}


void write(const char* file_loc, int width, int height, const vector<string> &words) {
	ofstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "could not write to file loc " << file_loc << endl;
		return;
	}
	
	f << width * height + words.size() << " ";
	
	ostringstream str;
	
	for (int i = 0; i < static_cast<int>(words.size()); i++)
		str << i << " ";
	str << "|";
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			str << " t" << i << "," << j;
		}
	}
	
	int options = 0;
	int choices = 0;
	
	for (int l = 0; l < static_cast<int>(words.size()); l++) {
		const string &s = words[l];
		bool palen = palendrome(s);
		int size = static_cast<int>(s.size());
		for (int i = 0; i < width - size + 1; i++) {
			for (int j = 0; j < height; j++) {
				options++;
				choices += size + 1;
				str << "\n" << l;
				for (int in = 0; in < size; in++)
					str << " t" << i + in << "," << j << ":" << s[in];
				if (!palen) {
					options++;
					choices += size + 1;
					str << "\n" << l;
					for (int in = 0; in < size; in++)
						str << " t" << i + size - in - 1 << "," << j << ":" << s[in];
				}
			}
		}
		for (int j = 0; j < height - size + 1; j++) {
			for (int i = 0; i < width; i++) {
				options++;
				choices += size + 1;
				str << "\n" << l;
				for (int in = 0; in < size; in++)
					str << " t" << i << "," << j + in << ":" << s[in];
				if (!palen) {
					options++;
					choices += size + 1;
					str << "\n" << l;
					for (int in = 0; in < size; in++)
						str << " t" << i << "," << j + size - in - 1 << ":" << s[in];
				}
			}
		}
		for (int j = 0; j < height - size + 1; j++) {
			for (int i = 0; i < width - size; i++) {
				options++;
				choices += size + 1;
				str << "\n" << l;
				for (int in = 0; in < size; in++)
					str << " t" << i + in << "," << j + in << ":" << s[in];
				if (!palen) {
					options++;
					choices += size + 1;
					str << "\n" << l;
					for (int in = 0; in < size; in++)
						str << " t" << i + size - in - 1 << "," << j + size - in - 1 << ":" << s[in];
				}
				
				options++;
				choices += size + 1;
				str << "\n" << l;
				for (int in = 0; in < size; in++)
					str << " t" << i + size - in - 1 << "," << j + in << ":" << s[in];
				if (!palen) {
					options++;
					choices += size + 1;
					str << "\n" << l;
					for (int in = 0; in < size; in++)
						str << " t" << i + in << "," << j + size - in - 1 << ":" << s[in];
				}
			}
		}
	}
	
	f << options << " " << choices << "\n";
	f << str.str();
	
	f.close();
}

void print(vector<string> options, int width, int height, std::ostream &o=cout) {
	char board[width * height];
	for (int i = 0; i < width * height; i++)
//		board[i] = 'a' + i % 26;
		board[i] = '_';
	for (const string &s : options) {
		istringstream is(s);
		string buf;
		is >> buf;
		while (is >> buf) {
			size_t colon = buf.find(':');
			size_t comma = buf.find(',');
			int i, j;
			istringstream ii(buf.substr(1, comma));
			istringstream ij(buf.substr(comma + 1, colon));
			ii >> i;
			ij >> j;
			char letter = buf.substr(colon + 1)[0];
			board[i + width * j] = letter;
		}
	}
	
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			o << board[i + width * j] << " ";
		}
		o << "\n";
	}
}

int pow(int a, int b) {
	if (b == 0)
		return 1;
	if (b % 2 == 0) {
		int p = pow(a, b / 2);
		return p * p;
	}
	return a * pow(a, b - 1);
}

float cost(const vector<string> &soln) {
	int totalDifferent = 0;
	int width = WIDTH;
	int height = HEIGHT;
	int board[width * height];
	for (int i = 0; i < width * height; i++)
		board[i] = 0;
	for (const string &s : soln) {
		istringstream is(s);
		string buf;
		is >> buf;
		while (is >> buf) {
			size_t colon = buf.find(':');
			size_t comma = buf.find(',');
			int i, j;
			istringstream ii(buf.substr(1, comma));
			istringstream ij(buf.substr(comma + 1, colon));
			ii >> i;
			ij >> j;
			if (board[i + width * j] != 0)
				totalDifferent += pow(2, board[i + width * j]);
			board[i + width * j]++;
		}
	}
	return totalDifferent;
}

void wordsearch() {
	const int w = WIDTH, h = HEIGHT;
	vector<string> words = {"abel", "bertrand", "borel", "cantor", "catalan", "frobenius",
		"glaisher", "gram", "hadamard", "hensel", "hermite", "hilbert", "hurwitz", "jensen",
		"kirchhoff", "knopp", "landau", "markoff", "mellin", "minkowski", "netto", "perron",
		"runge", "stern", "stieltjes", "sylvester", "weierstrauss"
	};
//	vector<string> words = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
	write("/users/claytonknittel/documents/xcode/math/math/wordsearch/p1.txt", w, h, words);
	dlx d = init("/users/claytonknittel/documents/xcode/math/math/wordsearch/p1.txt");

	vector<vector<int>> solns;
//	solve(d, solns, 1000000);
//	vector<int> soln;
//	solveOptimal(d, soln, cost, 6, true);
	
//	ofstream f;
//	f.open("/users/claytonknittel/documents/xcode/math/math/wordsearch/kn.txt");
//	for (int q = 0; q < solns.size(); q++) {
//		for (int i = 0; i < solns[q].size(); i++) {
//			f << solns[q][i];
//			if (i < solns[q].size() - 1)
//				f << " ";
//		}
//		if (q < solns.size() - 1)
//			f << "\n";
//	}
//	f.close();
	
	
	ifstream f;
	f.open("/users/claytonknittel/documents/xcode/math/math/wordsearch/kn.txt");
	string buf;
	while (getline(f, buf)) {
		solns.push_back(vector<int>());
		istringstream is(buf);
		int b;
		while (is >> b)
			solns[solns.size() - 1].push_back(b);
	}
	f.close();
	
	cout << "# solns: " << solns.size() << endl;
	
	if (solns.size() == 0)
		return;
	
	vector<string> m;
	float max = 0;
	for (size_t i = 0; i < solns.size(); i++) {
		vector<string> ops;
		getOptions(d, ops, solns[i]);
		float c = cost(ops);
		if (c > max) {
			max = c;
			m = ops;
		}
		if (i % 10000 == 0)
			cout << i << endl;
	}
	cout << max << endl;

	print(m, w, h);
}
