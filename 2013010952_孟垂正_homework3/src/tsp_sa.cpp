#include "tsp_sa.h"
using namespace std;

/*
   初始温度选取:升温法.以inc_alpha为比率升高温度,当接受率>cri_accept_p时停止到当前温度作为初始温度.
   状态被接受的条件:Metropolis准则,即总长度减小时接受,总长度增大时以概率exp(-delta/temperature)接受
   同一温度内结束条件:交换100*citynum次后结束
   降温算法:等比例下降,比例为alpha
   算法结束条件:零度法,温度下降至低于zero_temp后结束
*/

TspSa::TspSa(string inputfile, string outputfile) : citynum(0), route_length(0), zero_temp(1e-8), init_temp(100), alpha(0.99), inc_alpha(1.05), cri_accept_p(0.9) {
	srand(time(NULL));
	// temperature = init_temp;
	input.open(inputfile.c_str(), fstream::in);
	output.open(outputfile.c_str(), fstream::out);
}

void TspSa::load() {
	input >> citynum;
	string cityname;
	double x, y;
	for (int i = 0; i < citynum; ++i) {
		input >> cityname >> x >> y;
		name_vec.push_back(cityname);
		position.push_back(pair<double, double>(x, y)); 
	}
	input.close();
	dist.assign(citynum * citynum, 0);
	route.assign(citynum + 1, 0);
	for (int i = 0; i < citynum; ++i) {
		for (int j = 0; j < citynum; ++j) {
			if (i < j) {
				double dx = position.at(i).first - position.at(j).first;
				double dy = position.at(i).second - position.at(j).second;
				dist.at(i * citynum + j) = sqrt(dx * dx + dy * dy);
			}
			else {
				dist.at(i * citynum + j) = dist.at(j * citynum + i);
			}
		}
	}
	// Generate A x x x ... A route
	int a_rank = 0;
	for (int i = 0; i < citynum; ++i) {
		if (name_vec.at(i) == string("A")) {
			a_rank = i;
			break;
		}
	}
	for (int i = 0; i < citynum; ++i) {
		route.at(i) = i;
	}
	swap(route.at(a_rank), route.at(0));
	route.at(citynum) = a_rank;
}

void TspSa::save() {
	output.close();
}

void TspSa::simulated_annealing() {
	init_random();
	set_tem0();
	while (temperature > zero_temp) {
		for (int i = 0; i < 100 * citynum; ++i) {
			int start = 0, end = 0;
			while (start == end) {
				start = rand() % (citynum - 1) + 1;
				end = rand() % (citynum - 1) + 1;
			}
			if (start > end) {
				swap(start, end);
			}
			double new_res = get_reverse_length(start, end);
			double dres = new_res - route_length;
			bool accept = false;
			if (dres < 0) {
				accept = true;
			}
			else {
				double critical_p = exp(- dres / temperature);
				double p = (double)rand() / RAND_MAX;
				if (p < critical_p)
					accept = true;
			}
			if (accept) {
				reverse_between(start, end);
				route_length = new_res;
			}
		}
		temperature *= alpha;
		// output temp route
		for (int i = 0; i < citynum + 1; ++i)
			output << name_vec.at(route.at(i)) << ' ';
		output << route_length << endl;
	}
}

void TspSa::init_random() {
	for (int i = 1; i < citynum; ++i) {
		int choose = rand() % (citynum - i);
		swap(route.at(i), route.at(i + choose));
	}
	route_length = 0;
	for (int i = 0; i < citynum; ++i) {
		int start = route.at(i), end = route.at(i + 1);
		route_length += dist.at(start * citynum + end);
	}
}

void TspSa::set_tem0() {
	temperature = 1;
	double accept_p = 0;
	while (accept_p < cri_accept_p) {
		int n = 0, m = 0;
		for (int i = 0; i < 100 * citynum; ++i) {
			++n;
			int start = 0, end = 0;
			while (start == end) {
				start = rand() % (citynum - 1) + 1;
				end = rand() % (citynum - 1) + 1;
			}
			double new_result = get_reverse_length(start, end);
			double d = new_result - route_length;
			if (d < 0)
				++m;
			else {
				double p = (double)rand() / RAND_MAX;
				if (p < exp(- d / temperature))
					++m;
			}
		}
		accept_p = (double)m / (double)n;
		temperature *= inc_alpha;
	}
}

void TspSa::reverse_between(int first, int last) {
	int i = first, j = last;
	while (i < j) {
		swap(route.at(i++), route.at(j--));
	}
}

inline double TspSa::get_reverse_length(int first, int last) {
	int e1 = route.at(first - 1), s2 = route.at(first), e2 = route.at(last), s3 = route.at(last + 1);
	return route_length - dist.at(e1 * citynum + s2) - dist.at(e2 * citynum + s3) + dist.at(e1 * citynum + e2) + dist.at(s2 * citynum + s3);
}
