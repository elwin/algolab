#include <iostream>
#include <vector>
#include <limits>

size_t min_cost(std::vector<std::vector<size_t>> &neighbors, std::vector<size_t> &costs, size_t i, size_t status, std::vector<std::vector<int>> &dp);

size_t min_cost_dp(std::vector<std::vector<size_t>> &neighbors, std::vector<size_t> &costs, size_t i, size_t status, std::vector<std::vector<int>> &dp) {
	if (dp[status][i] < 0) {
		dp[status][i] = min_cost(neighbors, costs, i, status, dp);
	}

	return dp[status][i];
}


// status
// 0: must repair
// 1: must repair itself or one neighbor
// 2: does not have to repair
size_t min_cost(std::vector<std::vector<size_t>> &neighbors, std::vector<size_t> &costs, size_t i, size_t status, std::vector<std::vector<int>> &dp) {
	size_t cost_repair = costs[i];
	for (size_t neighbor : neighbors[i])
		cost_repair += min_cost_dp(neighbors, costs, neighbor, 2, dp);

	if (status == 0)
		return cost_repair;

	size_t neighbor_repair = std::numeric_limits<size_t>::max();
	for (size_t selected_neighbor : neighbors[i]) {
		size_t temp_neighbor_repair = 0;
		for (size_t neighbor : neighbors[i])
			temp_neighbor_repair += min_cost_dp(neighbors, costs, neighbor, (selected_neighbor == neighbor ? 0 : 1), dp);

		neighbor_repair = std::min(neighbor_repair, temp_neighbor_repair);
	}

	if (status == 1)
		return std::min(cost_repair, neighbor_repair);

	size_t cost_no_repair = 0;
	for (size_t neighbor : neighbors[i])
		cost_no_repair += min_cost_dp(neighbors, costs, neighbor, 1, dp);

	return std::min(std::min(cost_repair, cost_no_repair), neighbor_repair);
}

size_t testcase() {
	size_t n; std::cin >> n;
	std::vector<std::vector<size_t>> neighbors(n);
	std::vector<size_t> costs(n);
	for (size_t i = 0; i < n; ++i) {
		neighbors[i] = std::vector<size_t>(0);
	}

	for (size_t i = 0; i < n - 1; ++i) {
		size_t from, to; std::cin >> from >> to;
		neighbors[from].push_back(to);
	}

	for (size_t i = 0; i < n; ++i) {
		std::cin >> costs[i];
	}

	std::vector<std::vector<int>> dp(3, std::vector<int>(n, -1));

	return min_cost_dp(neighbors, costs, 0, 1, dp);
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}