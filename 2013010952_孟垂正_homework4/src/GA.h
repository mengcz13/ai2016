#ifndef GA_H
#define GA_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

namespace std {
	struct Point {
		double x;
		double y;
		Point() : x(0), y(0) {}
		Point(double xx, double yy) : x(xx), y(yy) {}
	};

	class GATspSolver {
	public:
		GATspSolver(double cr, double mr, int n, int maxgen) : crossover_rate(cr), mutation_rate(mr), pool_size(n), max_gen(maxgen), max_fit_value(0) { srand(time(NULL)); }
		// ~GATspSolver();
		void load(string filename);
		void GA();
		void save(string filename);
		double route_length(vector<int>& route);

	private:
		int citynum;
		double crossover_rate;
		double mutation_rate;
		int pool_size;
		int max_gen;
		vector<string> city_name_list;
		Point* city_pos_list;
		vector<double> distance_table;
		vector< vector<int> > GApool;
		vector<bool> crossed;

		vector<bool> hash_for_crossover;
		vector<int> sonroute1;
		vector<int> sonroute2;

		vector< vector<int> > backupGApool;
		vector<double> fit_value_pool;
		double max_fit_value;
		vector<int> shortest_route;

		double fit_value(vector<int>& route) { 
			double rl = route_length(route);
			return 100.0 / rl; 
		}
		double distance_between(int i, int j) { return distance_table.at(i * citynum + j); }
		void random_select();
		void crossover(vector<int>& route1, vector<int>& route2);
		void mutation(vector<int>& route);
		void shuffle(vector<int>& route);

		int bsearch(double sp, vector<double> fvp) {
			int start = 0, end = fvp.size();
			while (start < end) {
				int mid = ((start + end) >> 1);
				if (sp < fvp.at(mid))
					end = mid;
				else
					start = mid + 1;
			}
			return start; // Return the first value more than sp
		}
	};
}
#endif