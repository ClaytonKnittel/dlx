//
//  sudoku.cpp
//  math
//
//  Created by Clayton Knittel on 1/3/19.
//  Copyright © 2019 Clayton Knittel. All rights reserved.
//

#include "sudoku.hpp"

#include <iostream>

struct sudoku {
	int board[81];
	
	sudoku() {
		for (int i = 0; i < 81; i++) {
			board[i] = 0;
		}
		board[2] = 2;
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


void rudoku() {
	sudoku s;
	s.print();
}
