#include <iostream>
#include <vector>

int max_revenue_cached(int i, int j);

std::vector<int> coins;
std::vector<std::vector<int>> dp;

int max_revenue(int i, int j) {
	// Base case, only one coin left
	if (j - i == 1) {
		return coins[i];
	}

	if (j - i == 2) {
		return std::max(coins[i], coins[i + 1]);
	}

	int left_revenue = coins[i] + std::min(max_revenue_cached(i + 1, j - 1), max_revenue_cached(i + 2, j));
	int right_revenue = coins[j - 1] + std::min(max_revenue_cached(i, j - 2), max_revenue_cached(i + 1, j - 1));

	return std::max(left_revenue, right_revenue);
}

int max_revenue_cached(int i, int j) {
	if (dp[i][j - 1] == -1) {
		dp[i][j - 1] = max_revenue(i, j);
	}

	return dp[i][j - 1];
}

int testcase() {
	int n; std::cin >> n;
	coins = std::vector<int>(n);
	dp = std::vector<std::vector<int>>(n);
	for (int i = 0; i < n; ++i)
	{
		dp[i] = std::vector<int>(n, -1);
	}

	for (int i = 0; i < n; ++i)
	{
		std::cin >> coins[i];
	}

	return max_revenue(0, n);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}