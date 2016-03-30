#include <iostream>
#include <fstream>
#include "solver.h"
//#define DEBUG
using namespace std;

Solver::Solver() {
	int setans[9] = {1, 2, 3, 8, 0, 4, 7, 6, 5};
	for (int i = 0; i < Playarea::SIZE; ++i)
		for (int j = 0; j < Playarea::SIZE; ++j) {
			ANS.set_value_at(i, j, setans[i * Playarea::SIZE + j]);
		}
}

void Solver::readfrom(char* inname) {
	fstream infile;
	infile.open(inname, fstream::in);
	Playarea firstpa;
	for (int i = 0; i < Playarea::SIZE; ++i) {
		for (int j = 0; j < Playarea::SIZE; ++j) {
			int v = 0;
			infile >> v;
			firstpa.set_value_at(i, j, v);
		}
	}
	infile.close();
	firstpa.parent = -1;
	insert(firstpa);
}

void Solver::solve() {
	while (!playarea_heap.empty()) {
		int choose = get_top();
		if (playarea_array.at(choose).area == ANS.area) {
			ansrank = choose;
			stepnum = playarea_array.at(choose).g_value;
			solved = true;
			return;
		}			
		expand(choose);
	}
	solved = false;
}

void Solver::writeto(char* outname) {
	fstream outfile;
	outfile.open(outname, fstream::out);
	if (solved) {
		outfile << stepnum << endl << endl;
		int rank = ansrank;
		stack<int> path;
		while (rank != -1) {
			path.push(rank);
			rank = playarea_array.at(rank).parent;
		}
		path.pop();
		while (!path.empty()) {
			Playarea& step = playarea_array.at(path.top());
			for (int i = 0; i < Playarea::SIZE; ++i) {
				for (int j = 0; j < Playarea::SIZE; ++j) {
					outfile << step.unit(i, j) << ' ';
				}
				outfile << endl;
			}
			outfile << endl;
			path.pop();
		}
	}
	else {
		outfile << "no solution" << endl;
	}
	outfile.close();
}

void Solver::insert(Playarea& playarea) {
	std::map<unsigned long long, int>::iterator it = map.find(playarea.area);
	if (it == map.end()) { // New expanded node
		if (playarea.parent == -1)
			playarea.g_value = 0;
		else
			playarea.g_value = playarea_array.at(playarea.parent).g_value + 1;
		playarea.h_value = diff_value(playarea);
		playarea.f_value = playarea.g_value + playarea.h_value;
		int rank = playarea_array.size();
		playarea_array.push_back(playarea);
		map.insert(std::pair<unsigned long long, int>(playarea.area, rank));
		playarea_heap.push_back(rank);
		rank_in_heap.push_back(playarea_heap.size() - 1);
		go_up(playarea_heap.size() - 1);
	}
	else {
		int rank = it -> second;
		Playarea& target = playarea_array.at(rank);
		int temp_g = (playarea.parent == -1) ? 0 : (playarea_array.at(playarea.parent).g_value + 1);
		if (temp_g < target.g_value) {
			target.g_value = temp_g;
			target.f_value = target.g_value + target.h_value;
			target.parent = playarea.parent;
			if (rank_in_heap.at(rank) == -1) {
				playarea_heap.push_back(rank);
				rank_in_heap.at(rank) = playarea_heap.size() - 1;
			}
			go_up(rank_in_heap.at(rank));
		}
	}
}

void Solver::expand(int rank) {
	Playarea top = playarea_array.at(rank);
	int zi = 0, zj = 0;
	bool find = false;
	for (zi = 0; zi < Playarea::SIZE; ++zi) {
		for (zj = 0; zj < Playarea::SIZE; ++zj) {
			if (top.unit(zi, zj) == 0) {
				find = true;
				break;
			}
		}
		if (find)
			break;
	}
	int di[4] = {-1, 1, 0, 0};
	int dj[4] = {0, 0, -1, 1};
	for (int i = 0; i < 4; ++i) {
		int ti = zi + di[i], tj = zj + dj[i];
		if (0 <= ti && ti < Playarea::SIZE && 0 <= tj && tj < Playarea::SIZE) {
			Playarea son = top;
			int temp = son.unit(ti, tj);
			son.set_value_at(zi, zj, temp);
			son.set_value_at(ti, tj, 0);
			son.parent = rank;
			insert(son);
		}
	}
}

int Solver::diff_value(Playarea& playarea) {
	int sum = 0;
	unsigned long long pa = playarea.area;
	unsigned long long Aa = ANS.area;
	for (int i = 0; i < Playarea::SIZE * Playarea::SIZE; ++i) {
		if ((pa & 15) ^ (Aa & 15))
			++sum;
		pa >>= 4;
		Aa >>= 4;
	}
	return sum;
}

int Solver::go_up(int hrank) {
	while (hrank > 0) {
		int phrank = ((hrank - 1) >> 1);
		int rank = playarea_heap.at(hrank);
		int prank = playarea_heap.at(phrank);
		Playarea& par = playarea_array.at(prank);
		Playarea& me = playarea_array.at(rank);
		if (par.f_value > me.f_value) {
			swap_in_heap(hrank, phrank);
		}
		else {
			return hrank;
		}
	}
	return hrank;
}

int Solver::go_down(int hrank) {
	while (1) {
		int lhrank = (hrank << 1) + 1;
		int rhrank = lhrank + 1;
		int tarhrank = 0;
		if (lhrank < playarea_heap.size() && rhrank < playarea_heap.size()) {
			int lf = playarea_array.at(playarea_heap.at(lhrank)).f_value;
			int rf = playarea_array.at(playarea_heap.at(rhrank)).f_value;
			tarhrank = (lf < rf) ? lhrank : rhrank;
		}
		else if (lhrank < playarea_heap.size()) {
			tarhrank = lhrank;
		}
		else if (rhrank < playarea_heap.size()) {
			tarhrank = rhrank;
		}
		else {
			tarhrank = -1;
		}
		if (tarhrank == -1)
			return hrank;
		else {
			int tf = playarea_array.at(playarea_heap.at(tarhrank)).f_value;
			int nf = playarea_array.at(playarea_heap.at(hrank)).f_value;
			if (nf <= tf)
				return hrank;
			else {
				swap_in_heap(hrank, tarhrank);
				hrank = tarhrank;
			}
		}
	}
}

int Solver::get_top() {
	int res = playarea_heap.at(0);
	swap_in_heap(0, playarea_heap.size() - 1);
	playarea_heap.pop_back();
	rank_in_heap.at(res) = -1;
	go_down(0);
	return res;
}

void Solver::swap_in_heap(int hr1, int hr2) {
	int temp = playarea_heap.at(hr2);
	playarea_heap.at(hr2) = playarea_heap.at(hr1);
	playarea_heap.at(hr1) = temp;
	rank_in_heap.at(playarea_heap.at(hr1)) = hr1;
	rank_in_heap.at(playarea_heap.at(hr2)) = hr2;
}