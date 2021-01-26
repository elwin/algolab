#include <iostream>
#include <vector>

const int max_imbalance = 13 * 2;
const int max_types = 6;

struct param {
	int imbalance;
	int last_figher_north, last_figher_south;
};

size_t encode(param p) {
	size_t imbalance_pos = p.imbalance + max_imbalance / 2;
	size_t last_figher_north_pos = p.last_figher_north + 2;
	size_t last_figher_south_pos = p.last_figher_south + 2;

	return imbalance_pos + max_imbalance * (last_figher_north_pos + max_types * last_figher_south_pos);
}

long max_score(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp);

long max_score_dp(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp) {	
	if (i == fighters.size()) return 0;
	if (abs(p.imbalance) > max_imbalance / 2) return -1;

	size_t pi = encode(p);

	if (dp[i][pi] == 0) {
		dp[i][pi] = max_score(fighters, i, p, dp);
	}

	return dp[i][pi];
}

long max_score(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp) {
	if (i == fighters.size()) return 0;

	long cur_max = -1;

	long cur_north = 0;
	cur_north += (p.last_figher_north == -1 || p.last_figher_north == fighters[i] ? 1000 : 2000);
	cur_north -= 1 << abs(p.imbalance - 1);
	long cur_north_next = max_score_dp(fighters, i + 1, {p.imbalance - 1, fighters[i], p.last_figher_south}, dp);		
	if (cur_north >= 0 && cur_north_next != -1) {
		cur_max = std::max(cur_max, cur_north + cur_north_next);
	}

	long cur_south = 0;
	cur_south += (p.last_figher_south == -1 || p.last_figher_south == fighters[i] ? 1000 : 2000);
	cur_south -= 1 << abs(p.imbalance + 1);
	long cur_south_next = max_score_dp(fighters, i + 1, {p.imbalance + 1, p.last_figher_north, fighters[i]}, dp);		
	if (cur_south >= 0 && cur_south_next != -1) {
		cur_max = std::max(cur_max, cur_south + cur_south_next);
	}

	return cur_max;
}

long testcase() {
	size_t n, k, m; std::cin >> n >> k >> m;
	std::vector<int> fighters(n);
	for (size_t i = 0; i < n; ++i)
		std::cin >> fighters[i];

	std::vector<std::vector<long>> dp(n, std::vector<long>(10000, 0));

	return max_score_dp(fighters, 0, {0, -1, -1}, dp);
}

int main() {
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;;
}