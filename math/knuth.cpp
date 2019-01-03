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
#include <algorithm>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;

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

void make_decisions(node *nodes, item *items, map<int, int> &optionMap, vector<int> &decisions) {
	int xl = optionMap[decisions[decisions.size() - 1]];
	decisions.pop_back();
	int p = xl + 1;
	while (1) {
		int j = nodes[p].aux;
		if (j <= 0)
			break;
		else {
			cover(nodes, items, j);
			p ++;
		}
	}
	if (decisions.size() > 0)
		make_decisions(nodes, items, optionMap, decisions);
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

void write_file(const char* save_to_file_loc, node *nodes, item *items, map<int, int> optionMap, const vector<int> &choices, int num_items, int num_options) {
	ofstream f;
	f.open(save_to_file_loc);
	if (!f.is_open()) {
		cout << "unable to open file " << save_to_file_loc << endl;
		return;
	}
	
	ostringstream sizes;
	ostringstream itms;
	ostringstream opts;
	
	// write all items (we will be including the options that
	// have already been determined, so exclude nothing
	sizes << num_items - 1 << " ";
	for (int i = 1; i < num_items; i++) {
		itms << items[i].name;
		if (i < num_items - 1)
			itms << " ";
	}
	
	int new_num_opts = static_cast<int>(choices.size());
	int new_num_choices = 0;
	
	// write the given options
	for (auto it = choices.begin(); it != choices.end(); it++) {
		int opt = optionMap[*it];
		int x = opt + 1;
		int top;
		opts << '\n';
		while ((top = nodes[x].aux) > 0) {
			opts << items[top].name;
			x++;
			new_num_choices++;
			if (nodes[x].aux > 0)
				opts << " ";
		}
	}
	
	// write the remaining available options consistent with the givens
	for (int i = 0; i < num_options; i++) {
		int opt = optionMap[i];
		int x = opt + 1;
		bool is_still_viable = 1;
		while (nodes[x].aux > 0) {
			int itm = nodes[x].aux;
			if (items[items[itm].left].right != itm) {
				is_still_viable = 0;
				break;
			}
			x++;
		}
		if (is_still_viable) {
			opts << '\n';
			new_num_opts++;
			int x = opt + 1;
			int top;
			while ((top = nodes[x].aux) > 0) {
				opts << items[top].name;
				x++;
				new_num_choices++;
				if (nodes[x].aux > 0)
					opts << " ";
			}
		}
	}
	sizes << new_num_opts << " " << new_num_choices;
	f << sizes.str() << '\n' << itms.str() << opts.str();
	f.close();
}

void construct_option_map(node *nodes, int num_items, int num_nodes, map<int, int> &m) {
	for (int i = num_items + 1; i < num_nodes - 1; i++)
		if (nodes[i].aux <= 0)
			m[-nodes[i].aux] = i;
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


dlx_struct::dlx_struct(node *nodes, item *items, map<int, int> *m): nodes(nodes), items(items), optionMap(m) {}

dlx_struct::~dlx_struct() {
	delete [] nodes;
	delete [] items;
	delete optionMap;
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
dlx init(const char* file_loc) {
	ifstream f;
	f.open(file_loc);
	if (!f.is_open()) {
		cout << "unable to open file " << file_loc << endl;
		return nullptr;
	}
	int num_nodes, num_items, num_options;
	get_sizes(f, num_nodes, num_items, num_options);
	node *nodes = new node[num_nodes];
	item *items = new item[num_items];
	if (!create_structs(f, nodes, items, num_nodes, num_items, num_options)) {
		delete [] nodes;
		delete [] items;
		return nullptr;
	}
	map<int, int> *optionMap = new map<int, int>();
	construct_option_map(nodes, num_items, num_nodes, *optionMap);
	dlx d = std::make_shared<dlx_struct>(nodes, items, optionMap);
	d->num_items = num_items;
	d->num_nodes = num_nodes;
	d->num_options = num_options;
	return d;
}

void solve(dlx d, vector<vector<int>> &solutions) {
	vector<int> soln;
	solveAllHelp(d->nodes, d->items, soln, solutions);
}

void solveOnce(dlx d, vector<int> &solution) {
	solveOneHelp(d->nodes, d->items, solution);
}

void makeDecisions(dlx d, vector<int> decisions) {
	vector<int> cpy = decisions;
	make_decisions(d->nodes, d->items, *(d->optionMap), cpy);
}

void makeDecisions(dlx d, vector<string> decisions) {
//	vector<int> cpy = decisions;
//	make_decisions(d->nodes, d->items, *(d->optionMap), cpy);
}

void write_to_file(dlx d, const char* save_to_file_loc, vector<int> decisions) {
	write_file(save_to_file_loc, d->nodes, d->items, *(d->optionMap), decisions, d->num_items, d->num_options);
}

void getOptions(const char* file_loc, vector<string> &results, vector<int> solution) {
	if (solution.size() == 0) {
		cout << "no solution given" << endl;
		return;
	}
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
