#include "playarea.h"
#include "solver.h"

int main(int argc, char** argv) {
	if (argc != 3)
		return -1;
	Solver solver;
	solver.readfrom(argv[1]);
	solver.solve();
	solver.writeto(argv[2]);
	return 0;
}