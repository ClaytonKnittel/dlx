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
#include <map>

using std::string;
using std::vector;
using std::map;

struct node;
struct item;

struct dlx_struct {
	string file_loc;
	int num_items, num_options, num_nodes;
	node *nodes;
	item *items;
	map<int, int> *optionMap;
	string *options;
	
	dlx_struct(node *nodes, item *items, string *options, map<int, int> *m);
	~dlx_struct();
};

typedef std::shared_ptr<dlx_struct> dlx;

dlx init(const char* file_loc);

size_t countSolns(dlx d);
void solve(dlx d, vector<vector<int>> &solutions, size_t stopAfter=0);
void solveOnce(dlx d, vector<int> &solution);
void solveOptimal(dlx d, vector<int> &solution, float(*cost)(const vector<string> &solution),
				  float costGoal=-1, bool maximize=false);

// decisions is a list of the options chosen (in order given in file)
void makeDecisions(dlx d, vector<int> decisions);
void makeDecisions(dlx d, vector<string> decisions);

void write_to_file(dlx d, const char* save_to_file_loc, vector<int> decisions);

void getOptions(dlx d, vector<string> &results, const vector<int> solution);

#endif /* knuth_h */
