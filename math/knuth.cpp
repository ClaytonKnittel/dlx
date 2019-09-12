//
//  knuth.cpp
//  math
//
//  Created by Clayton Knittel on 12/29/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>

#include "knuth.hpp"
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
	int up, down, aux, color;
};

struct item {
	char name[8];
	int left, right;
};

void hide(node *nodes, int p) {
	int q = p + 1;
	while (q != p) {
		if (nodes[q].color < 0) {
			q++;
			continue;
		}
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

void purify(node *nodes, int p) {
	int c = nodes[p].color;
	int i = nodes[p].aux;
	int q = nodes[i].down;
	while (q != i) {
		if (nodes[q].color != c)
			hide(nodes, q);
		else if (q != p)
			nodes[q].color = -1;
		q = nodes[q].down;
	}
}

void commit(node *nodes, item *items, int p, int j) {
	if (nodes[p].color == 0) {
		cover(nodes, items, j);
	} else if (nodes[p].color > 0) {
		purify(nodes, p);
	}
}

void unhide(node *nodes, int p) {
	int q = p - 1;
	while (q != p) {
		if (nodes[q].color < 0) {
			q--;
			continue;
		}
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

void unpurify(node *nodes, int p) {
	int c = nodes[p].color;
	int i = nodes[p].aux;
	int q = nodes[i].up;
	while (q != i) {
		if (nodes[q].color < 0)
			nodes[q].color = c;
		else if (q != p)
			unhide(nodes, q);
		q = nodes[q].up;
	}
}

void uncommit(node *nodes, item *items, int p, int j) {
	if (nodes[p].color == 0)
		uncover(nodes, items, j);
	else if (nodes[p].color > 0)
		unpurify(nodes, p);
}

int get_index(node *nodes, int xl) {
	while (nodes[xl].aux > 0)
		xl--;
	return -nodes[xl].aux;
//	return xl;
}

void print_update(vector<int> &soln) {
	static int tot = 0;
	tot++;
	if (tot % 1000000 == 0) {
		cout << "Solution " << tot << endl;
		for (auto it = soln.begin(); it != soln.end(); it++)
			cout << *it << " ";
		cout << endl;
	}
}

size_t countSolnsHelp(node *nodes, item *items) {
	if (items[0].right == 0) {
		return 1;
	}
	size_t solns = 0;
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
				commit(nodes, items, p, j);
				p ++;
			}
		}
		solns += countSolnsHelp(nodes, items);
		p = xl - 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].down;
			else {
				uncommit(nodes, items, p, j);
				p --;
			}
		}
		xl = nodes[xl].down;
	}
	uncover(nodes, items, i);
	return solns;
}

bool solveAllHelp(node *nodes, item *items, vector<int> &soln, vector<vector<int>> &solns, size_t stopAfter) {
	if (items[0].right == 0) {
		solns.push_back(vector<int>(soln));
		print_update(solns[solns.size() - 1]);
		if (stopAfter == 0)
			return false;
		if (solns.size() >= stopAfter)
			return true;
		return false;
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
				commit(nodes, items, p, j);
				p ++;
			}
		}
		soln.push_back(get_index(nodes, xl));
		if (solveAllHelp(nodes, items, soln, solns, stopAfter))
			return true;
		soln.pop_back();
		p = xl - 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].down;
			else {
				uncommit(nodes, items, p, j);
				p --;
			}
		}
		xl = nodes[xl].down;
	}
	uncover(nodes, items, i);
	return false;
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
				commit(nodes, items, p, j);
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
				uncommit(nodes, items, p, j);
				p --;
			}
		}
		xl = nodes[xl].down;
	}
	uncover(nodes, items, i);
	return false;
}

bool solveOptimalHelp(dlx d, vector<int> &soln, vector<int> &best, float bestCost, float(*cost)(const vector<string> &solution), float costGoal, bool maximize) {
	item *items = d->items;
	node *nodes = d->nodes;
	if (items[0].right == 0) {
		print_update(soln);
		if (best.size() == 0) {
			best = soln;
			vector<string> s;
			getOptions(d, s, soln);
			bestCost = cost(s);
		}
		else {
			vector<string> s;
			getOptions(d, s, soln);
			float c = cost(s);
			if (maximize) {
				if (c > bestCost) {
					bestCost = c;
					best = soln;
				}
			} else {
				if (c < bestCost) {
					bestCost = c;
					best = soln;
				}
			}
		}
		if (bestCost == -1)
			return false;
		if (maximize) {
			if (bestCost >= costGoal)
				return true;
		}
		else {
			if (bestCost <= costGoal)
				return true;
		}
		return false;
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
				commit(nodes, items, p, j);
				p ++;
			}
		}
		soln.push_back(get_index(nodes, xl));
		if (solveOptimalHelp(d, soln, best, bestCost, cost, costGoal, maximize))
			return true;
		soln.pop_back();
		p = xl - 1;
		while (p != xl) {
			int j = nodes[p].aux;
			if (j <= 0)
				p = nodes[p].down;
			else {
				uncommit(nodes, items, p, j);
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

bool create_structs(ifstream &f, node *nodes, item *items, string *&options, int num_nodes, int num_items, int num_options) {
	options = new string[num_options];
	
	items[0].left = num_items - 1;
	items[0].right = 1;

	string itemNames;
	getline(f, itemNames);
	istringstream is(itemNames);
	
	int optionalArgsLoc = -1;

	map<string, int> itemIndex;
	
	const string optionalDivider = "|";
	int divided = 0;

	int nodeIndex = 1;
	for (int i = 1; i < num_items; i++) {
		if (divided) {
			items[i].right = i;
			items[i].left = i;
		}
		else {
			items[i].right = i + 1;
			items[i].left = i - 1;
		}
		if (!(is >> items[i].name)) {
			cout << "expected " << num_items << " items, but found less" << endl;
			return false;
		}
		if (items[i].name == optionalDivider) {
			if (divided) {
				cout << "too many dividers \"|\"" << endl;
				return false;
			}
			divided = 1;
			optionalArgsLoc = i;
			i--;
			items[i].right = 0;
			items[0].left = i;
			continue;
		}
		
		
		// So later on we can find the node corresponding to a given option
		itemIndex[items[i].name] = i;
		
		nodes[nodeIndex].up = i;
		nodes[nodeIndex].down = i;
		nodes[nodeIndex].aux = 0; // length
		nodeIndex++;
	}
	
	items[num_items - 1].right = 0;
	
	if (is >> itemNames) {
		cout << "expected " << num_items << " items, but found more" << endl;
		return false;
	}
	
	int spacerCount = 0;
	int lastSpacer = nodeIndex;
	nodes[nodeIndex].aux = -spacerCount++;
	nodes[nodeIndex].color = 0;
	nodeIndex++;
	
//    for (int i = 0; i < num_items; i++)
//        cout << items[i].name << " " << items[i].left << " " << items[i].right << endl;
	
	string option;
	for (int i = 0; i < num_options; i++) {
		if (!getline(f, option)) {
			cout << "num_options too low" << endl;
			f.close();
			return false;
		}
		options[i] = option;
		istringstream opt(option);
		int firstInRow = -1;
		
		string itm;
		while (opt >> itm) {
			if (nodeIndex >= num_nodes) {
				cout << "too few nodes predicted, check numNodes and numOptions" << endl;
				f.close();
				return false;
			}
			
			size_t loc = itm.find(':');
			// has a color value
			if (loc + 1 != 0) {
				if (!divided) {
					cout << "no optional args given, so cannot specify color value" << endl;
					return false;
				}
				string color = itm.substr(loc + 1);
				itm = itm.substr(0, loc);
				if (color.size() != 1) {
					cout << "color value must be single char, but got string " << color << endl;
					return false;
				}
				nodes[nodeIndex].color = static_cast<int>(color[0]);
			}
			else
				nodes[nodeIndex].color = 0;
			
			auto it = itemIndex.find(itm);
			if (it == itemIndex.end()) {
				cout << "key " << itm << " not in item list" << endl;
				return false;
			}
			int x = it->second;
			
			if (optionalArgsLoc > x) {
				if (nodes[nodeIndex].color > 0) {
					cout << "option " << itm << " is not an optional arg, so cannot have color value" << endl;
					return false;
				}
			}
			
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
		nodes[nodeIndex].color = 0;
		lastSpacer = nodeIndex;
		nodeIndex++;
	}
	if (nodeIndex != num_nodes) {
		cout << "expected " << num_nodes << " nodes, but have " << nodeIndex << endl;
		return false;
	}
	if (getline(f, option)) {
		cout << "num_options too high" << endl;
		return false;
	}
	f.close();
	
//	for (int i = 0; i < num_nodes; i++) {
//		cout << i << ": " << nodes[i].aux << " " << nodes[i].up << " " << nodes[i].down << " " << nodes[i].color << endl;
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
	if (!(is >> num_items >> num_options >> numChoices)) {
		cout << "first arguments in file aren't properly formatted" << endl;
		return;
	}
	num_items ++; // for the first item (items[0]) which is always empty
	num_nodes = num_options + num_items + numChoices + 1;
}


dlx_struct::dlx_struct(node *nodes, item *items, string *options, map<int, int> *m): nodes(nodes), items(items), options(options), optionMap(m) {}

dlx_struct::~dlx_struct() {
	delete [] nodes;
	delete [] items;
	delete [] options;
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
	string *options;
	if (!create_structs(f, nodes, items, options, num_nodes, num_items, num_options)) {
		delete [] nodes;
		delete [] items;
		delete [] options;
		return nullptr;
	}
	map<int, int> *optionMap = new map<int, int>();
	construct_option_map(nodes, num_items, num_nodes, *optionMap);
	dlx d = std::make_shared<dlx_struct>(nodes, items, options, optionMap);
	d->num_items = num_items;
	d->num_nodes = num_nodes;
	d->num_options = num_options;
	d->file_loc = file_loc;
	return d;
}

void solve(dlx d, vector<vector<int>> &solutions, size_t stopAfter) {
	vector<int> soln;
	solveAllHelp(d->nodes, d->items, soln, solutions, stopAfter);
}

size_t countSolns(dlx d) {
	return countSolnsHelp(d->nodes, d->items);
}

void solveOnce(dlx d, vector<int> &solution) {
	solveOneHelp(d->nodes, d->items, solution);
}

void solveOptimal(dlx d, vector<int> &solution, float(*cost)(const vector<string> &solution), float costGoal, bool maximize) {
	if (costGoal < 0 && costGoal != -1) {
		cout << "Costs must be non-negative, so cost goal must also be (-1 means no goal)" << endl;
		return;
	}
	vector<int> temp;
	solveOptimalHelp(d, temp, solution, -1, cost, costGoal, maximize);
}

void makeDecisions(dlx d, vector<int> decisions) {
	vector<int> cpy = decisions;
	make_decisions(d->nodes, d->items, *(d->optionMap), cpy);
}

void makeDecisions(dlx d, vector<string> decisions) {
	item *items = d->items;
	for (auto it = decisions.begin(); it != decisions.end(); it++) {
		istringstream is(*it);
		string buf;
		int found = 0;
		while (is >> buf) {
			for (int p = 1; p != 0; p = items[p].right) {
				if (buf == items[p].name) {
					found = 1;
					cover(d->nodes, items, p);
					break;
				}
			}
			if (!found)
				cout << "decision " << buf << " was not found" << endl;
		}
	}
}

void write_to_file(dlx d, const char* save_to_file_loc, vector<int> decisions) {
	write_file(save_to_file_loc, d->nodes, d->items, *(d->optionMap), decisions, d->num_items, d->num_options);
}

void getOptions(dlx d, vector<string> &results, const vector<int> solution) {
	if (solution.size() == 0) {
		cout << "no solution given" << endl;
		return;
	}
	for (int s : solution)
		results.push_back(d->options[s]);
}
