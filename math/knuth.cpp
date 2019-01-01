//
//  knuth.cpp
//  math
//
//  Created by Clayton Knittel on 12/29/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>

#include "knuth.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::map;

struct node {
	int up, down, aux;
};

struct item {
	char name[8];
	int left, right;
};

void hide(node *nodes, int p) {
	int q = p + 1;
	while (q != p) {
		int x = nodes[q].aux;
		int u = nodes[q].up;
		int d = nodes[q].down;
		if (x <= 0) {
			q = u;
		}
		else {
			nodes[u].down = d;
			nodes[d].up = u;
			nodes[x].aux --;
			q++;
		}
	}
}

void cover(node *nodes, item *items, int i) {
	int p = nodes[i].down;
	while (p != i) {
		hide(nodes, p);
		p = nodes[p].down;
	}
	int l = items[i].left;
	int r = items[i].right;
	items[l].right = r;
	items[r].left = l;
}

void unhide(node *nodes, int p) {
	int q = p - 1;
	while (q != p) {
		int x = nodes[q].aux;
		int u = nodes[q].up;
		int d = nodes[q].down;
		if (x <= 0) {
			q = d;
		}
		else {
			nodes[u].down = q;
			nodes[d].up = q;
			nodes[x].aux ++;
			q--;
		}
	}
}

void uncover(node *nodes, item *items, int i) {
	int l = items[i].left;
	int r = items[i].right;
	items[l].right = i;
	items[r].left = i;
	int p = nodes[i].up;
	while (p != i) {
		unhide(nodes, p);
		p = nodes[p].up;
	}
}

int get_index(node *nodes, int xl) {
	while (nodes[xl].aux > 0)
		xl--;
	return -nodes[xl].aux;
//	return xl;
}

//void recover_option(vector<string> &res, node *nodes, item *items, int spacerIndex) {
//	int top;
//	while ((top = nodes[++spacerIndex].aux) > 0)
//		res.push_back(items[top].name);
//}
//
//void recover_option(vector<vector<string>> &res, node *nodes, item *items, vector<int> spacerIndices) {
//	for (auto it = spacerIndices.begin(); it != spacerIndices.end(); it++) {
//		res.push_back(vector<string>());
//		recover_option(res[res.size() - 1], nodes, items, *it);
//	}
//}

void print_update(vector<vector<int>> &solns) {
	static int tot = 0;
	tot++;
	if (tot % 10000 == 0) {
		cout << "Solution " << tot << endl;
		auto &last = solns[solns.size() - 1];
		for (auto it = last.begin(); it != last.end(); it++)
			cout << *it << " ";
		cout << endl;
	}
}

void solveAllHelp(node *nodes, item *items, vector<int> &soln, vector<vector<int>> &solns) {
	if (items[0].right == 0) {
		solns.push_back(vector<int>(soln));
		print_update(solns);
		return;
	}
	int minlen = -1;
	int min_index = 0;
	int find = items[0].right;
	while (find != 0) {
		int m = nodes[find].aux;
		if (m < minlen || minlen == -1) {
			minlen = m;
			min_index = find;
		}
		find = items[find].right;
	}
	int i = min_index;
	cover(nodes, items, i);
	int xl = nodes[i].down;
	while (xl != i) {
		int p = xl + 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].up;
			else {
				cover(nodes, items, j);
				p ++;
			}
		}
		soln.push_back(get_index(nodes, xl));
		solveAllHelp(nodes, items, soln, solns);
		soln.pop_back();
		p = xl - 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].down;
			else {
				uncover(nodes, items, j);
				p --;
			}
		}
		xl = nodes[xl].down;
	}
	uncover(nodes, items, i);
}

bool solveOneHelp(node *nodes, item *items, vector<int> &soln) {
	if (items[0].right == 0)
		return true;
	int minlen = -1;
	int min_index = 0;
	int find = items[0].right;
	while (find != 0) {
		int m = nodes[find].aux;
		if (m < minlen || minlen == -1) {
			minlen = m;
			min_index = find;
		}
		find = items[find].right;
	}
	int i = min_index;
	cover(nodes, items, i);
	int xl = nodes[i].down;
	while (xl != i) {
		int p = xl + 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].up;
			else {
				cover(nodes, items, j);
				p ++;
			}
		}
		if (solveOneHelp(nodes, items, soln)) {
			soln.push_back(get_index(nodes, xl));
			return true;
		}
		p = xl - 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].down;
			else {
				uncover(nodes, items, j);
				p --;
			}
		}
		xl = nodes[xl].down;
	}
	uncover(nodes, items, i);
	return false;
}

bool create_structs(ifstream &f, node *nodes, item *items, int num_nodes, int num_items, int num_options) {
	items[0].left = num_items - 1;
	items[num_items - 1].right = 0;

	string itemNames;
	getline(f, itemNames);
	istringstream is(itemNames);

	map<string, int> itemIndex;

	int nodeIndex = 1;
	for (int i = 1; i < num_items; i++) {
		items[i - 1].right = i;
		items[i].left = i - 1;
		is >> items[i].name;
		
		// So later on we can find the node corresponding to a given option
		itemIndex[items[i].name] = i;
		
		nodes[nodeIndex].up = i;
		nodes[nodeIndex].down = i;
		nodes[nodeIndex].aux = 0; // length
		nodeIndex++;
	}
	
	int spacerCount = 0;
	int lastSpacer = nodeIndex;
	nodes[nodeIndex].aux = -spacerCount++;
	nodeIndex++;
	
//    for (int i = 0; i < num_items; i++)
//        cout << items[i].name << " " << items[i].left << " " << items[i].right << endl;
	
	string option;
	for (int i = 0; i < num_options; i++) {
		if (!getline(f, option)) {
			cout << "num_options incorrect" << endl;
			f.close();
			return false;
		}
		istringstream opt(option);
		int firstInRow = -1;
		
		string itm;
		while (opt >> itm) {
			if (nodeIndex >= num_nodes) {
				cout << "too few nodes predicted, check numNodes and numOptions" << endl;
				f.close();
				return false;
			}
			
			int x = itemIndex[itm];
			
			if (firstInRow == -1) {
				firstInRow = nodeIndex;
			}
			
			nodes[nodeIndex].down = x;
			nodes[nodeIndex].up = nodes[x].up;
			nodes[x].up = nodeIndex;
			nodes[nodes[nodeIndex].up].down = nodeIndex;
			nodes[nodeIndex].aux = x; // top
			nodes[x].aux ++; // increase length of item
			nodeIndex++;
		}
		if (nodeIndex >= num_nodes) {
			cout << "too few nodes predicted, check numNodes and numOptions" << endl;
			f.close();
			return false;
		}
		nodes[lastSpacer].down = nodeIndex - 1;
		nodes[nodeIndex].up = firstInRow;
		nodes[nodeIndex].aux = -spacerCount++;
		lastSpacer = nodeIndex;
		nodeIndex++;
	}
	f.close();
	
//	for (int i = 0; i < num_nodes; i++) {
//		cout << i << ": " << nodes[i].aux << " " << nodes[i].up << " " << nodes[i].down << endl;
//		if (i % 8 == 7)
//			cout << endl;
//	}
	return true;
}

void get_sizes(ifstream &f, int &num_nodes, int &num_items, int &num_options) {
	string buf;
	getline(f, buf);
	istringstream is(buf);
	int numChoices;
	is >> num_items >> num_options >> numChoices;
	num_items ++; // for the first item (items[0]) which is always empty
	num_nodes = num_options + num_items + numChoices + 1;
}

/**
 * File protocol:
 * num_items num_options num_choices
 * items... | optional items...
 * option1
 * option2
 * ...
 *
 * num choices is sum of all items in all options
 */
void solve(const char* file_loc, vector<vector<int>> &solutions) {
	ifstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to open file " << file_loc << endl;
		return;
	}
	int num_nodes, num_items, num_options;
	get_sizes(f, num_nodes, num_items, num_options);
	node nodes[num_nodes];
	item items[num_items];
	if (!create_structs(f, nodes, items, num_nodes, num_items, num_options))
		return;
	vector<int> soln;
	solveAllHelp(nodes, items, soln, solutions);
}

void solveOnce(const char* file_loc, vector<int> &solution) {
	ifstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to open file " << file_loc << endl;
		return;
	}
	int num_nodes, num_items, num_options;
	get_sizes(f, num_nodes, num_items, num_options);
	node nodes[num_nodes];
	item items[num_items];
	if (!create_structs(f, nodes, items, num_nodes, num_items, num_options))
		return;
	solveOneHelp(nodes, items, solution);
}

void getOptions(const char* file_loc, vector<string> &results, vector<int> solution) {
	ifstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to open file " << file_loc << endl;
		return;
	}
	std::sort(solution.begin(), solution.end());
	string buf;
	int loc = 0;
	getline(f, buf); // skip first two lines
	getline(f, buf);
	for (int i = 0; 1; i++) {
		if (!getline(f, buf)) {
			cout << "error in solution or file, option " << solution[loc] << " does not exist" << endl;
			f.close();
			return;
		}
		if (i != solution[loc])
			continue;
		loc++;
		results.push_back(buf);
		if (loc >= static_cast<int>(solution.size()))
			break;
	}
	f.close();
}
