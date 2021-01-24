#include <iostream>
#include <vector>
#include <queue>

struct edge {
	size_t i; long score;
};

long max_score(std::vector<std::vector<edge>> &c, std::vector<std::vector<long>> &dp, int k, size_t i);

long max_score_dp(std::vector<std::vector<edge>> &c, std::vector<std::vector<long>> &dp, int k, size_t i) {
	if (dp[i][k] == 0) dp[i][k] = max_score(c, dp, k, i);

	return dp[i][k];
}

long max_score(std::vector<std::vector<edge>> &c, std::vector<std::vector<long>> &dp, int k, size_t i) {
	if (k == 0) return 0;
	if (c[i].size() == 0) return max_score_dp(c, dp, k, 0);

	long max = 0;
	for (size_t j = 0; j < c[i].size(); ++j)
		max = std::max(max, max_score_dp(c, dp, k - 1, c[i][j].i) + c[i][j].score);

	return max;
}

int testcase() {
	size_t n, m; int k; long x; std::cin >> n >> m >> x >> k;

	std::vector<std::vector<edge>> connections(n);
	std::vector<std::vector<long>> dp(n);
	for (size_t i = 0; i < n; ++i) {
		connections[i] = std::vector<edge>(0);
		dp[i] = std::vector<long>(k + 1);
	}

	for (size_t i = 0; i < m; ++i) {
		size_t u, v; long p; std::cin >> u >> v >> p;
		connections[u].push_back({v, p});
	}

	for (int i = 0; i < k + 1; ++i)
		max_score_dp(connections, dp, i, 0);

	for (size_t i = 0; i < dp[0].size(); ++i)
		if (dp[0][i] >= x) return i;

	return -1;
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