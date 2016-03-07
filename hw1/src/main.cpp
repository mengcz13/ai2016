#include <cstdio>
#include <cstring>
#include <cstdlib>

const int SUDOKUSIZE = 9;
const int SUDOKUUNITSIZE = 3;

class Sudoku {
	public:
		Sudoku();
		~Sudoku();
		void load(char*, int, int);
		void print(char*);
		void solve();
		void solve_unit(int, int);
		bool iflegal(int, int);
		char& value(int x, int y) { return area[x * size + y]; }
	private:
		int size;
		int unitsize;
		char* area;
		char* ans;
};

Sudoku::Sudoku() : size(0), unitsize(0), area(NULL), ans(NULL) {};

Sudoku::~Sudoku() {
	if (area != NULL) {
		delete []area;
	}
	if (ans != NULL) {
		delete []ans;
	}
}

void Sudoku::load(char* infilename, int insize, int inunitsize) {
	size = insize;
	unitsize = inunitsize;
	FILE* infile = fopen(infilename, "r");
	if (infile == NULL) {
		puts("File not existed!");
		exit(1);
	}
	fseek(infile, 0, SEEK_END);
	int filesize = ftell(infile);
	rewind(infile);
	char* buffer = new char[filesize];
	fread(buffer, sizeof(char), filesize, infile);
	
	area = new char[filesize];
	memset(area, 0, sizeof(char) * filesize);
	ans = new char[filesize];
	memset(ans, 0, sizeof(char) * filesize);
	char* pbuffer = buffer;
	char* parea = area;
	for (int i = 0; i < size; ++i) {
		memcpy(parea, pbuffer, sizeof(char) * size);
		if (i < size - 1) {
			pbuffer += size;
			parea += size;
			while (!('0' <= *pbuffer && *pbuffer <= '9'))
				++pbuffer;
		}
	}

	delete []buffer;
}

void Sudoku::print(char* outfilename) {
	FILE* outfile = fopen(outfilename, "w");
	char* buffer = new char[size * (size + 1) + 1];
	memset(buffer, 0, sizeof(char) * (size * (size + 1) + 1));
	char* pbuffer = buffer;
	for (int i = 0; i < size; ++i) {
		memcpy(pbuffer, ans + i * size, sizeof(char) * size);
		pbuffer += size;
		*pbuffer = '\n';
		++pbuffer;
	}
	*pbuffer = '\0';
	fputs(buffer, outfile);
}

void Sudoku::solve() {
	solve_unit(0, 0);
}

void Sudoku::solve_unit(int x, int y) {
	if (x == size) {
		memcpy(ans, area, sizeof(char) * size * size);
		return;
	}
	else if (value(x, y) != '0') {
		if (y == size - 1)
			solve_unit(x + 1, 0);
		else
			solve_unit(x, y + 1);
		return;
	}
	else {
		for (int i = 0; i <= 9; ++i) {
			value(x, y) = '0' + i;
			if (iflegal(x, y)) {
				if (y == size - 1)
					solve_unit(x + 1, 0);
				else
					solve_unit(x, y + 1);
			}
			value(x, y) = '0';
		}
		return;
	}
}

bool Sudoku::iflegal(int x, int y) {
	//Row
	for (int j = 0; j < size; ++j) {
		if (j != y && value(x, j) == value(x, y))
			return false;
	}
	//Column
	for (int i = 0; i < size; ++i) {
		if (i != x && value(i, y) == value(x, y)) 
			return false;
	}
	//Big unit
	int uxs = x / unitsize * unitsize;
	int uys = y / unitsize * unitsize;
	for (int i = 0; i < unitsize; ++i) {
		for (int j = 0; j < unitsize; ++j) {
			if (!(i + uxs == x && j + uys == y) && value(i + uxs, j + uys) == value(x, y))
				return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		puts("Format: main.out input_filename output_filename");
		exit(1);
	}
	char* infilename = argv[1];
	char* outfilename = argv[2];
	Sudoku sudoku;
	sudoku.load(infilename, SUDOKUSIZE, SUDOKUUNITSIZE);
	sudoku.solve();
	sudoku.print(outfilename);
	return 0;
}
