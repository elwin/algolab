#include <iostream>
#include <vector>

int max_score(std::vector<size_t> &defenders, size_t i, size_t attackers, size_t k, std::vector<std::vector<int>> &dp);

int max_score_dp(std::vector<size_t> &defenders, size_t i, size_t attackers, size_t k, std::vector<std::vector<int>> &dp) {
	if (dp[attackers][i] == 0) {
		dp[attackers][i] = max_score(defenders, i, attackers, k, dp);
	}

	return dp[attackers][i];
}


int max_score(std::vector<size_t> &defenders, size_t i, size_t attackers, size_t k, std::vector<std::vector<int>> &dp) {
	if (attackers == 0) return 0;
	if (i == defenders.size()) return -1;

	size_t cur_score = 0; size_t j = i;
	while (cur_score < k && j < defenders.size()) {
		cur_score += defenders[j++];
	}

	int ignore = max_score_dp(defenders, i + 1, attackers, k, dp);

	if (cur_score != k) {
		return ignore;
	}

	int take = max_score_dp(defenders, j, attackers - 1, k, dp);

	if (take == -1 && ignore == -1) return -1;
	if (take == -1) return ignore;
	if (ignore == -1) return j - i + take;

	return std::max(int(j - i) + take, ignore);
} 

void testcase() {
	size_t n, m, k; std::cin >> n >> m >> k;
	std::vector<size_t> defenders(n);
	for (size_t i = 0; i < n; ++i)
		std::cin >> defenders[i];

	std::vector<std::vector<int>>dp(m + 1);
	for (size_t i = 0; i < dp.size(); ++i)
		dp[i] = std::vector<int>(n + 1);

	int score = max_score_dp(defenders, 0, m, k, dp);

	std::cout << (score == -1 ? "fail" : std::to_string(score)) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}