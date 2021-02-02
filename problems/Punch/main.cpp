#include <iostream>
#include <vector>
#include <limits>

// <cost, volume>
typedef std::vector<std::pair<size_t, size_t>> D;

// <cost, variation>
typedef std::pair<int, int> Res;
typedef std::vector<std::vector<Res>> DP;

Res min_cost(D &drinks, size_t i, size_t cur, size_t k, DP &dp);

Res min_cost_dp(D &drinks, size_t i, size_t cur, size_t k, DP &dp) {
	if (cur >= k) return min_cost(drinks, i, cur, k, dp);
	if (dp[i][cur].first == 0) {
		dp[i][cur] = min_cost(drinks, i, cur, k, dp);
	}

	return dp[i][cur];
}

Res min_cost(D &drinks, size_t i, size_t cur, size_t k, DP &dp) {
	if (cur >= k) return {0, 0};
	if (i == drinks.size()) return {-1, 0};

	auto d = drinks[i];
	Res min = {std::numeric_limits<int>::max(), 0};
	for (size_t j = 0; ; ++j) {
		Res next = min_cost_dp(drinks, i + 1, cur + d.second * j, k, dp);
		if (next.first == -1) continue;
		
		next.first += int(d.first * j);
		if (j > 0) next.second--;

		min = std::min(min, next);

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

	DP dp = DP(n + 1, std::vector<Res>(k + 1));

	Res r = min_cost_dp(drinks, 0, 0, k, dp);

	std::cout << r.first << " " << -r.second << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) {
		testcase();
	}
}