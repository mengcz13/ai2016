#include "tsp_sa.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
	if (argc != 3)
		return -1;
	TspSa tsp_sa(argv[1], argv[2]);
	tsp_sa.load();
	tsp_sa.simulated_annealing();
	tsp_sa.save();
	return 0;
}