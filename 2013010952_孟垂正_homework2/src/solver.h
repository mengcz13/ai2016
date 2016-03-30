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
	std::vector<Playarea> playarea_array; // Vector to store situations that have ever happened
	std::vector<int> rank_in_heap; // Vector to store the rank of one situation in the heap for O(1) access, useful when modify f_value of a existed situation
	std::map<unsigned long long, int> map; // Map to link situations and their ranks in playarea_array, allowing O(logn) time to get the rank in playarea_array
	std::vector<int> playarea_heap; // Min f_value heap of "open" situations, O(logn) pick and insert, functioning as openlist
	bool solved;
	int stepnum;
	int ansrank;
	Playarea ANS; //The target, initialized in constructor

	void insert(Playarea&); // Insert an expanded situation to openlist. If it has never happened, add it to the playarea_array and to the heap, else just judge if its f_value needs to be modified and adjust its position in the heap
	void expand(int); // Expand all next step of one situation and insert them
	int diff_value(Playarea&); // Calculate how many digits are not in proper positions
	int go_up(int); // Up in heap, return the final position
	int go_down(int); // Down in heap, return the final position
	int get_top(); // Pick the top of the heap and modify the heap, return the top situation's rank in the array
	void swap_in_heap(int, int); // Swap two element in the heap and modify their ranks in heap that are stored in rank_in_heap
};

#endif