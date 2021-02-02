#include <iostream>
#include <vector>

// <cost, volume>
typedef std::vector<std::pair<size_t, size_t>> D;
typedef std::vector<std::vector<int>> DP;

int min_cost(D drinks, size_t i, size_t cur, size_t k, DP &dp);

int min_cost_dp(D drinks, size_t i, size_t cur, size_t k, DP &dp) {
	if (cur > k) return min_cost(drinks, i, cur, k, dp);
	if (dp[i][cur] == 0) {
		dp[i][cur] = min_cost(drinks, i, cur, k, dp);
	}

	return dp[i][cur];
}

int min_cost(D drinks, size_t i, size_t cur, size_t k, DP &dp) {
	if (cur >= k) return 0;
	if (i == drinks.size()) return -1;

	auto d = drinks[i];
	int min = 10000;
	for (size_t j = 0; ; ++j) {
		// std::cout << i << " " << j << std::endl;

		int next = min_cost_dp(drinks, i + 1, cur + d.second * j, k, dp);
		if (next == -1) continue;
		
		min = std::min(min, next + int(d.first * j));

		if (cur + d.second * j > k) break;
	}

	return min;
}

void testcase() {
	size_t n, k; std::cin >> n >> k;
	D drinks(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> drinks[i].first >> drinks[i].second;
	}

	DP dp = DP(n + 1, std::vector<int>(k + 1));

	std::cout << min_cost_dp(drinks, 0, 0, k, dp) << std::endl;
}

int main() {
	size_t t; std::cin >> t;
	while (t--) testcase();
}