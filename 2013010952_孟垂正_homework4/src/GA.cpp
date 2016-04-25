#include "GA.h"
using namespace std;

double dist(const Point& a, const Point& b) {
	return (sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

void GATspSolver::load(string filename) {
	ifstream input(filename.c_str());
	input >> citynum;
	city_name_list.reserve(citynum);
	city_pos_list = new Point[citynum];
	distance_table.assign(citynum * citynum, 0);
	vector<int> route(citynum + 1, 0);
	GApool.assign(pool_size, route);
	crossed.assign(pool_size, false);
	hash_for_crossover.assign(citynum, false);
	sonroute1.assign(citynum + 1, 0);
	sonroute2.assign(citynum + 1, 0);

	backupGApool = GApool;
	fit_value_pool.assign(pool_size, 0);

	for (int i = 0; i < citynum; ++i) {
		string cityname;
		double cx = 0, cy = 0;
		input >> cityname >> cx >> cy;
		city_name_list.push_back(cityname);
		city_pos_list[i] = Point(cx, cy);
	}

	for (int i = 0; i < citynum; ++i) {
		for (int j = 0; j < citynum; ++j) {
			if (i < j)
				distance_table.at(i * citynum + j) = dist(city_pos_list[i], city_pos_list[j]);
			else
				distance_table.at(i * citynum + j) = distance_table.at(j * citynum + i);
		}
	}

	input.close();
	delete []city_pos_list;
}

void GATspSolver::GA() {
	// pool_size shuffled
	for (int i = 0; i < pool_size; ++i) {
		for (int j = 0; j < citynum; ++j) {
			GApool.at(i).at(j) = j;
		}
		GApool.at(i).at(citynum) = 0;
		shuffle(GApool.at(i));
	}

	int gennum = 0;
	while (gennum < max_gen) {
		// random select ...
		random_select();

		// crossover
		crossed.assign(pool_size, false);
		for (int i = 0; i < pool_size; ++i) {
			for (int j = 0; j < pool_size; ++j) {
				if (i != j && !crossed.at(i) && !crossed.at(j)) {
					double p = rand() / (double)RAND_MAX;
					if (p < crossover_rate) {
						crossover(GApool.at(i), GApool.at(j));
						crossed.at(i) = crossed.at(j) = true;
						double f1 = fit_value(GApool.at(i));
						double f2 = fit_value(GApool.at(j));
						if (f1 > max_fit_value) {
							max_fit_value = f1;
							shortest_route = GApool.at(i);
						}
						if (f2 > max_fit_value) {
							max_fit_value = f2;
							shortest_route = GApool.at(j);
						}
					}
				}
			}
		}

		// mutation
		for (int i = 0; i < pool_size; ++i) {
			double p = rand() / (double)RAND_MAX;
			if (p < mutation_rate) {
				mutation(GApool.at(i));
			}
		}

		++gennum;
	}
}

void GATspSolver::save(string filename) {
	ofstream output(filename.c_str());
	for (int i = 0; i < citynum + 1; ++i)
		output << city_name_list.at(shortest_route.at(i)) << ' ';
	output << route_length(shortest_route);
	output.close();
}

double GATspSolver::route_length(vector<int>& route) {
	double len = 0;
	for (int i = 0; i < citynum; ++i) {
		len += distance_between(route.at(i), route.at(i + 1));
	}
	return len;
}

void GATspSolver::random_select() {
	fit_value_pool.assign(pool_size, 0);
	double prefix_sum = 0;
	for (int i = 0; i < pool_size; ++i) {
		double cfv = fit_value(GApool.at(i));
		prefix_sum += cfv;
		fit_value_pool.at(i) = prefix_sum;
		if (cfv > max_fit_value) {
			max_fit_value = cfv;
			shortest_route = GApool.at(i);
		}
	}
	for (int i = 0; i < pool_size; ++i) {
		fit_value_pool.at(i) /= prefix_sum;
	}
	fit_value_pool.at(pool_size - 1) = 1;

	for (int i = 0; i < pool_size; ++i) {
		double sp = rand() / (double)RAND_MAX;
		backupGApool.at(i) = GApool.at(bsearch(sp, fit_value_pool));
	}

	GApool = backupGApool;
}

void GATspSolver::crossover(vector<int>& route1, vector<int>& route2) {
	int crossstart = rand() % (citynum - 2) + 1;

	hash_for_crossover.assign(citynum, false);
	sonroute1.at(0) = sonroute1.at(citynum) = 0;
	for (int i = 1; i < crossstart; ++i) {
		sonroute1.at(i) = route1.at(i);
		hash_for_crossover.at(route1.at(i)) = true;
	}
	int p = 1;
	for (int i = crossstart; i < citynum; ++i) {
		while (hash_for_crossover.at(route2.at(p)))
			++p;
		sonroute1.at(i) = route2.at(p);
		++p;
	}

	hash_for_crossover.assign(citynum, false);
	sonroute2.at(0) = sonroute2.at(citynum) = 0;
	for (int i = 1; i < crossstart; ++i) {
		sonroute2.at(i) = route2.at(i);
		hash_for_crossover.at(route2.at(i)) = true;
	}
	p = 1;
	for (int i = crossstart; i < citynum; ++i) {
		while (hash_for_crossover.at(route1.at(p)))
			++p;
		sonroute2.at(i) = route1.at(p);
		++p;
	}

	route1 = sonroute1;
	route2 = sonroute2;
}

void GATspSolver::mutation(vector<int>& route) {
	int p = rand() % (citynum - 1) + 1;
	int q = p;
	while (q == p) {
		q = rand() % (citynum - 1) + 1;
	}
	if (p > q)
		swap(p, q);
	while (p < q) {
		swap(route.at(p), route.at(q));
		++p;
		--q;
	}
}

void GATspSolver::shuffle(vector<int>& route) {
	for (int i = 1; i < citynum; ++i) {
		int pos = rand() % (citynum - i) + i;
		swap(route.at(i), route.at(pos));
	}
}