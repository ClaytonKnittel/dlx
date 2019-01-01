//
//  knuth.h
//  math
//
//  Created by Clayton Knittel on 12/27/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef knuth_h
#define knuth_h

#include <stdio.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

void solve(const char* file_loc, vector<vector<int>> &solutions);
void solveOnce(const char* file_loc, vector<int> &solution);

void getOptions(const char* file_loc, vector<string> &results, vector<int> solution);

#endif /* knuth_h */
