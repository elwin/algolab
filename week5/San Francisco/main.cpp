#include <iostream>
#include <vector>
#include <queue>

struct edge {
	size_t i; long score;
};

int min_moves(std::vector<std::vector<edge>> &c, size_t k, long score, size_t i) {
	if (score <= 0) return k;
	if (k == 0) return -1;
	if (c[i].size() == 0) return min_moves(c, k, score, 0);

	int min = -1;
	for (size_t j = 0; j < c[i].size(); ++j)
		min = std::max(min, min_moves(c, k - 1, score - c[i][j].score, c[i][j].i));

	return min;
}

int testcase() {
	size_t n, m, k; long x; std::cin >> n >> m >> x >> k;

	std::vector<std::vector<edge>> connections(n);
	for (size_t i = 0; i < n; ++i) {
		connections[i] = std::vector<edge>(0);
	}

	for (size_t i = 0; i < m; ++i) {
		size_t u, v; long p; std::cin >> u >> v >> p;
		connections[u].push_back({v, p});
	}

	int moves = min_moves(connections, k, x, 0);
	if (moves == -1) return -1;
	return k - moves;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) {
		int out = testcase();
		if (out > 0) {
			std::cout << out << std::endl;
		} else {
			std::cout << "Impossible" << std::endl;
		}
	}
}