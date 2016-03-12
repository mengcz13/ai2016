#include "tsp.h"
#define DEBUG

bool edge_weight_compare(const Edge& e1, const Edge& e2) {
	return (e1.weight < e2.weight);
}

bool path_compare(const Path& p1, const Path& p2) {
	return (p1.total_len < p2.total_len);
}

void Tsp::load(const char* infilename) {
	fstream infile;
	infile.open(infilename, fstream::in);
	infile >> pointnum;
	for (int i = 0; i < pointnum; ++i) {
		for (int j = 0; j < pointnum; ++j) {
			int wei = 0;
			infile >> wei;
			if (i < j) {
				Edge edge;
				edge.weight = wei;
				edge.sp = i;
				edge.ep = j;
				edge_vec.push_back(edge);
			}
		}
	}
	sort(edge_vec.begin(), edge_vec.end(), edge_weight_compare);
	edgenum = edge_vec.size();
	infile.close();
	table = new int[pointnum];
	memset(table, 0, sizeof(int) * pointnum);
}

void Tsp::solve() {
	if (edgenum < pointnum) {
		cerr << "No solution because there are fewer edges than points." << endl;
		return;
	}
	int pos = 0, epos = edgenum, last_border = edgenum;
	do {
		if (pos == epos) {
			pos = solu_vec.back().current_pos + 1;
			epos = solu_vec.back().end_pos;
			curr_len -= edge_vec[solu_vec.back().current_pos].weight;
			solu_vec.pop_back();
		}
		else {
			Status tempsta = { pos, epos };
			solu_vec.push_back(tempsta);
			curr_len += edge_vec[solu_vec.back().current_pos].weight;
			if (solu_vec.size() < pointnum) {
				if (!ishroute()) {
					pos = solu_vec.back().current_pos + 1;
					epos = solu_vec.back().end_pos;
					curr_len -= edge_vec[solu_vec.back().current_pos].weight;
					solu_vec.pop_back();
				}
				else {
					pos = solu_vec.back().current_pos + 1;
					epos = last_border - (pointnum - (solu_vec.size() + 1));
				}
			}
			else {
				if (opt_len != -1 && curr_len > opt_len) {
					int dist = 1;
					last_border = solu_vec.back().current_pos + 1;
					for (int i = solu_vec.size() - 1; i >= 0; --i) {
						solu_vec[i].end_pos = solu_vec.back().current_pos + dist;
						--dist;
					}
					pos = solu_vec.back().current_pos + 1;
					epos = solu_vec.back().end_pos;
					curr_len -= edge_vec[solu_vec.back().current_pos].weight;
					solu_vec.pop_back();
				}
				else {
					if (ishroute()) {
						opt_len = curr_len;
						int dist = 1;
						last_border = solu_vec.back().current_pos + 1;
						for (int i = solu_vec.size() - 1; i >= 0; --i) {
							solu_vec[i].end_pos = solu_vec.back().current_pos + dist;
							--dist;
						}
						Path path;
						h_route_vec.push_back(path);
						h_route_vec.back().path = solu_vec;
						h_route_vec.back().total_len = curr_len;
						pos = solu_vec.back().current_pos + 1;
						epos = solu_vec.back().end_pos;
						curr_len -= edge_vec[solu_vec.back().current_pos].weight;
						solu_vec.pop_back();
					}
					else {
						pos = solu_vec.back().current_pos + 1;
						epos = solu_vec.back().end_pos;
						curr_len -= edge_vec[solu_vec.back().current_pos].weight;
						solu_vec.pop_back();
					}
				}
			}
		}
	}while (!solu_vec.empty() || pos < epos);
	sort(h_route_vec.begin(), h_route_vec.end(), path_compare);
}

bool Tsp::ishroute() {
	memset(table, 0, sizeof(int) * pointnum);
	for (int i = 0; i < solu_vec.size(); ++i) {
		int sta = edge_vec[solu_vec[i].current_pos].sp;
		int end = edge_vec[solu_vec[i].current_pos].ep;
		table[sta]++;
		table[end]++;
	}
	for (int i = 0; i < pointnum; ++i) {
		if (table[i] > 2)
			return false;
	}
	if (solu_vec.size() == pointnum) {
		for (int i = 0; i < pointnum; ++i) {
			if (table[i] != 2)
				return false;
		}
		memset(table, 0, sizeof(int) * pointnum);
		dfs(0);
		for (int i = 0; i < pointnum; ++i) {
			if (table[i] != 1)
				return false;
		}
	}
	return true;
}

void Tsp::dfs(int sp) {
	table[sp] = 1;
	for (int i = 0; i < solu_vec.size(); ++i) {
		int rank = solu_vec[i].current_pos;
		int next = -1;
		if (edge_vec[rank].sp == sp)
			next = edge_vec[rank].ep;
		else if (edge_vec[rank].ep == sp)
			next = edge_vec[rank].sp;
		if (next >= 0 && table[next] == 0)
			dfs(next);
	}
}

void Tsp::print(const char* outfilename) {
	fstream outfile;
	outfile.open(outfilename, fstream::out);
	for (int i = 0; i < h_route_vec.size(); ++i) {
		for (int j = 0; j < h_route_vec[i].path.size(); ++j) {
			const Edge& tedge = edge_vec[h_route_vec[i].path[j].current_pos];
			outfile << tedge.sp << '-' << tedge.ep << ' ';
		}
		outfile << h_route_vec[i].total_len << endl;
	}
	outfile.close();
}