//
//  main.cpp
//  math
//
//  Created by Clayton Knittel on 9/16/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <iostream>
//#include "kanoodle.hpp"
// #include "sudoku.hpp"
#include "nqueens.hpp"
//#include "wordsearch.hpp"
//#include "countingCastles.hpp"

#include <ctime>


int main(int argc, const char * argv[]) {
	
//	int a, b;
//	printf("type:\n");
//	int ret = sscanf("1 2", "%d %d", &a, &b);
//	printf("%d, %d\nreturned %d\n", a, b, ret);
//
//	for (int a = 0; a <= 14; a++)
//		printf("a: %d ; %d\n", a, func4(a, 0, 14));
//	printf("ex: %d\n", 0x2d);
//	run();
	//rudoku();

    time_t start = clock();
	nqueens(15);
    time_t end = clock();
    printf("Time to solve: %fs\n", static_cast<float>(end - start) / CLOCKS_PER_SEC);
//	wordsearch();
//	counting();
    return 0;
}
