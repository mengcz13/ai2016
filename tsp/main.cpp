#include "tsp.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "Usage: inputfile outputfile" << endl;
		exit(1);
	}
	Tsp tsp;
	tsp.load(argv[1]);
	tsp.solve();
	tsp.print(argv[2]);
	return 0;
}