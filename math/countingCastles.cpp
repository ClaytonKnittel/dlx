//
//  countingCastles.cpp
//  math
//
//  Created by Clayton Knittel on 1/13/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "countingCastles.hpp"
#include <iostream>

using std::cout;
using std::endl;


size_t pow(size_t a, size_t b) {
	if (b == 0)
		return 1;
	if ((b & 1) == 0) {
		size_t c = pow(a, b / 2);
		return c * c;
	}
	return a * pow(a, b - 1);
}

long double dpow(long double a, size_t b) {
	if (b == 0)
		return 1.;
	if ((b & 1) == 0) {
		long double c = dpow(a, b / 2);
		cout << "C: " << c << endl;
		return c * c;
	}
	return a * dpow(a, b - 1);
}

std::pair<size_t, size_t> bruteForce(size_t w, size_t h) {
	size_t ar[w];
	size_t pows[w];
	
	size_t min = 1;
	size_t max = min + (w + 1) / 2 * (h - 1);
	size_t counts[max - min + 1];
	
	for (size_t i = 0; i <= max - min; i++)
		counts[i] = 0;
	
	for (size_t i = 0; i < w; i++)
		pows[i] = pow(h, w - i - 1);
	size_t nperms = pow(h, w);
	size_t count = 0;
	const size_t totalValid = nperms - pow(h - 1, w);
	
	for (size_t perm = 0; perm < nperms; perm++) {
		for (size_t i = 0; i < w; i++)
			ar[i] = perm / pows[i] % h;
		size_t numBlocks = 1;
		size_t maxe = ar[0];
		for (size_t i = 1; i < w; i++) {
			if (ar[i] > maxe)
				maxe = ar[i];
			if (ar[i] < ar[i - 1])
				numBlocks += ar[i - 1] - ar[i];
		}
		numBlocks += ar[w - 1];
		if ((numBlocks & 1) == 0 && maxe == h - 1)
			count++;
		counts[numBlocks - min]++;
	}
//	for (size_t c = min; c <= max; c++)
//		cout << c << ": " << counts[c - min] << endl;
//	cout << count << " of " << totalValid << " (" << nperms << ")" << endl;
	return {count, totalValid};
}

size_t totalValid(size_t w, size_t h) {
	return pow(h, w) - pow(h - 1, w);
}

size_t dtotalValid(size_t w, size_t h) {
	return dpow(static_cast<long double>(h), w) - dpow(static_cast<long double>(h - 1), w);
}

void counting() {
	cout << totalValid(1000000000000, 100) + totalValid(10000, 10000) + totalValid(100, 1000000000000) << endl;
	cout << dtotalValid(1000000000000, 100) << " " << dtotalValid(10000, 10000) << " " << dtotalValid(100, 1000000000000) << endl;
	cout << static_cast<size_t>(0) - 1 << endl << endl;
	cout << sizeof(long double) << endl;
	
//	cout << "w x h\n";
//	cout << "\t";
//	for (size_t h = 2; h < 6; h++)
//		cout << "  " << h << "\t\t";
//	cout << endl;
//	for (size_t w = 2; w < 6; w++) {
//		cout << w << "\t";
//		for (size_t h = 2; h < 6; h++) {
//			auto p = bruteForce(w, h);
//			cout << "(" << p.first << " " << p.second << ")\t";
//		}
//		cout << endl;
//	}
}

/*
 
 F(4,2):
 
	 h = 2-1 = 1
	 w = 4+1 = 5	(max # of numbers)
 
	 1 -1
	 5c2 = 10
 
	 1 -1 1 -1
	 5c4 = 5 (omit bc odd)
 
 
 F(4,3):
 
 	h = 3-1 = 2
 	w = 4+1 = 5
 
 	1 -1
 	2 -2 (odd)
 	5c2 = 10
 
 	1 -1 1 -1 (odd)
 
 leftover: 1
 
	1 -1 2 -2
 
 
 	2 -2 1 -1
 
 
 	2 -1 1 -2
 
 
 
 
 */
