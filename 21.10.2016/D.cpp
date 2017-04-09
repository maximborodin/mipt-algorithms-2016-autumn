#include <iostream>
#include <cstdio>
#include <algorithm>

int n; // количество вершин в каждой доле
std::vector<std::vector<int> > g;
std::vector<int> mt;
std::vector<char> used;

bool try_kuhn (int v)
{
	if (used[v])  return false;
	used[v] = true;
	for (size_t i=0; i<g[v].size(); ++i) {
		int to = g[v][i];
		if (mt[to] == -1 || try_kuhn (mt[to])) {
			mt[to] = v;
			return true;
		}
	}
	return false;
}

class Vertex
{
public:
	Vertex(int h_, int m_, int a_, int b_, int c_, int d_) :
	h (h_), m(m_), a(a_), b(b_), c(c_), d(d_)
	{}
	int h, m;
	int a, b, c, d;
};

int main()
{
	std::vector<Vertex> ver;
	std::cin >> n;
	g.resize(n);
	int h, m;
	char charachter;
	int a, b, c, d;
	for (int i = 0;i < n;i++) {
		std::cin >> h >> charachter >> m;
		std::cin >> a >> b >> c >> d;
		ver.push_back(Vertex(h, m, a, b, c, d));
	}
	for (int i = 0;i < n;i++){
		for (int j = 0;j < n;j++){
			if (i != j){
				int time1 = ver[i].h * 60 + ver[i].m;
				int time2 = ver[j].h * 60 + ver[j].m;
				int diff1 = std::abs(ver[i].c - ver[i].a) + std::abs(ver[i].d - ver[i].b);
				int diff2 = std::abs(ver[j].a - ver[i].c) + std::abs(ver[j].b - ver[i].d);
				if (time1 + diff1 + diff2 < time2){
					g[i].push_back(j);
				}}
		}
	}

	int matching = 0;
	mt.assign (n, -1);
	for (int v = 0; v < n; ++v) {
		used.assign (n, false);
		if (try_kuhn (v)){
			++matching;
		}
	}
	std::cout << n - matching << std::endl;
}