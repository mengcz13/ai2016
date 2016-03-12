#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;

struct Edge {
	int weight;
	int sp;
	int ep;
};

struct Status {
	int current_pos;
	int end_pos;
};

struct Path {
	vector<Status> path;
	int total_len;
};

bool edge_weight_compare(const Edge&, const Edge&);
bool path_compare(const Path&, const Path&);

class Tsp {
public:
	Tsp() : pointnum(0), edgenum(0), opt_len(-1), curr_len(0), table(NULL) {};
	~Tsp() {delete []table;}
	void load(const char*);
	void solve();
	bool ishroute();
	void dfs(int);
	void print(const char*);
private:
	vector<Edge> edge_vec;
	vector<Status> solu_vec;
	vector<Path> h_route_vec;
	int pointnum;
	int edgenum;
	int opt_len;
	int curr_len;
	int* table;
};

#endif