#include "GA.h"
using namespace std;

const double CR = 0.9;
const double MR = 0.01;
const int POOLSIZE = 100;
const int MAXGEN = 100000;

int main(int argc, char** argv) {
	if (argc != 3)
		return -1;
	GATspSolver solver(CR, MR, POOLSIZE, MAXGEN);
	solver.load(argv[1]);
	solver.GA();
	solver.save(argv[2]);
	return 0;
}