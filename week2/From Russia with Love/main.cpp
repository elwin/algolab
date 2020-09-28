#include <iostream>
#include <vector>

int max_revenue(int turn, int i, int j);

std::vector<int> values;
int k; int m;

// turn (0, m - 1), i, j
std::vector<std::vector<std::vector<int>>> dp;
std::vector<std::vector<std::vector<bool>>> dp_seen;

int max_revenue_cached(int turn, int i, int j) {
	turn = turn % m;

	if (!dp_seen[turn][i][j - 1]) {
		dp[turn][i][j - 1] = max_revenue(turn, i, j);
		dp_seen[turn][i][j - 1] = true;
	}

	return dp[turn][i][j - 1];
}

// revenue from value[i] up to value[j - 1]
int max_revenue(int turn, int i, int j) {
	turn = turn % m;

	if (i + 1 == j) {
		if (turn == k) {
			return values[i];
		} else {
			return 0;
		}
	}

	int pick_left = max_revenue_cached(turn + 1, i + 1, j);
	int pick_right = max_revenue_cached(turn + 1, i, j - 1);

	if (turn == k) {
		int left = pick_left + values[i];
		int right = pick_right + values[j - 1];
		return std::max(left, right);
	}

	return std::min(pick_left, pick_right);
}

int testcase() {
	int n; std::cin >> n;
	std::cin >> m;
	std::cin >> k;

	dp = std::vector<std::vector<std::vector<int>>>(m);
	dp_seen = std::vector<std::vector<std::vector<bool>>>(m);
	for (int i = 0; i < m; ++i) {
		dp[i] = std::vector<std::vector<int>>(n);
		dp_seen[i] = std::vector<std::vector<bool>>(n);
		for (int j = 0; j < n; ++j) {
			dp[i][j] = std::vector<int>(n);
			dp_seen[i][j] = std::vector<bool>(n);
		}
	}

	values = std::vector<int>(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> values[i];
	}

	return max_revenue_cached(0, 0, values.size());
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		std::cout << testcase() << '\n';
	}

	std::cout << std::flush;
}