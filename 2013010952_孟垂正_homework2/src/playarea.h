#ifndef PLAYAREA_H
#define PLAYAREA_H

struct Playarea {
	static const int SIZE = 3;
	unsigned long long area;
	int f_value;
	int g_value;
	int h_value;
	int parent;
	int unit(int i, int j) {
		return ((area >> ((i * SIZE + j) << 2)) & 15);
	}
	void set_value_at(int i, int j, int v) {
		area = ((area & (~((unsigned long long)15 << ((i * SIZE + j) << 2)))) | ((unsigned long long)v << ((i * SIZE + j) << 2)));
	}
	Playarea(): area(0), f_value(0), g_value(0), h_value(0), parent(-1) {}
};

#endif