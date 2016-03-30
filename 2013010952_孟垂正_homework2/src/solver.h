#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <map>
#include <stack>
#include "playarea.h"

class Solver {
public:
	Solver();
	void readfrom(char* inname);
	void solve();
	void writeto(char* outname);
private:
	std::vector<Playarea> playarea_array;
	std::vector<int> rank_in_heap;
	std::map<unsigned long long, int> map;
	std::vector<int> playarea_heap;
	bool solved;
	int stepnum;
	int ansrank;
	Playarea ANS;

	void insert(Playarea&);
	void expand(int);
	int diff_value(Playarea&);
	int go_up(int);
	int go_down(int);
	int get_top();
	void swap_in_heap(int, int);
};

#endif