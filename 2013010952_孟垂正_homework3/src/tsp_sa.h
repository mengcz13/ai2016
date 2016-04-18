#ifndef TSP_SA_H
#define TSP_SA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

const double eps = 1e-6;

class TspSa {
public:
	TspSa(std::string inputfile, std::string outputfile);
	// ~TspSa();
	void load();
	void save();
	void simulated_annealing();
	void init_random();
	void set_tem0();
	void reverse_between(int first, int last);
	double get_reverse_length(int first, int last);
private:
	int citynum;
	double route_length;
	double temperature;
	double zero_temp;
	double init_temp;
	double alpha;
	double inc_alpha;
	double cri_accept_p;
	std::fstream input;
	std::fstream output;
	std::vector<std::string> name_vec;
	std::vector<std::pair<double, double> > position;
	std::vector<double> dist;
	std::vector<int> route;
};

#endif