// I solved this exercise using DP. Usually this is straightforward,
// however, here we have up to 6 different variables:
// - i (index)
// - imbalance (a number between -12 and +12)
// - 4 different types of combinations:
//   - Last round / 2 rounds before
//   - North / South entrance
// While we could encode this in a 6 dimensional array, I'm not sure
// if this would actually work out. Thus we'll map 5 of the 6
// variables to a single integer (see `encode` function).
// Also, this code is a mess, have mercy, I have only
// like 3 days left until the exam.
#include <iostream>
#include <vector>
#include <set>


// 2 ^ 12 > 3000, thus imbalance cannot be larger than that, otherwise we'd cause a riot
const int max_imbalance = 12 * 2;

// We encode -1 for not set, 0-3 will be used for different types
const int max_types = 5;

// Space we need to encode imbalance and 4 types
const int max_encoder = max_imbalance * max_types * max_types * max_types * max_types;

// Jesus who got this idea
struct param {
	int imbalance;
	int last_figher_north, last_figher_south;
	int last_last_figher_north, last_last_figher_south;
};

// Map all possible values to an unique index
size_t encode(param p) {
	size_t imbalance = p.imbalance + max_imbalance / 2;
	size_t last_figher_north = p.last_figher_north + 1;
	size_t last_figher_south = p.last_figher_south + 1;
	size_t last_last_figher_north = p.last_last_figher_north + 1;
	size_t last_last_figher_south = p.last_last_figher_south + 1;

	return imbalance + max_imbalance * (last_figher_north + max_types * (last_figher_south + max_types * (last_last_figher_north + max_types * last_last_figher_south)));
}

long max_score(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp, bool three);

long max_score_dp(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp, bool three) {	
	if (i == fighters.size()) return 0;
	if (abs(p.imbalance) > max_imbalance / 2) return -1;

	size_t pi = encode(p);

	if (pi > dp[0].size()) std::cout << "ALARM!" << std::endl;

	if (dp[i][pi] == 0) {
		dp[i][pi] = max_score(fighters, i, p, dp, three);
	}

	return dp[i][pi];
}

long max_score_two(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp) {
	if (i == fighters.size()) return 0;

	long cur_max = -1;

	long cur_north = 0;
	cur_north += (p.last_figher_north == -1 || p.last_figher_north == fighters[i] ? 1000 : 2000);
	cur_north -= 1 << abs(p.imbalance - 1);
	long cur_north_next = max_score_dp(fighters, i + 1, {p.imbalance - 1, fighters[i], p.last_figher_south}, dp, false);		
	if (cur_north >= 0 && cur_north_next != -1) {
		cur_max = std::max(cur_max, cur_north + cur_north_next);
	}

	long cur_south = 0;
	cur_south += (p.last_figher_south == -1 || p.last_figher_south == fighters[i] ? 1000 : 2000);
	cur_south -= 1 << abs(p.imbalance + 1);
	long cur_south_next = max_score_dp(fighters, i + 1, {p.imbalance + 1, p.last_figher_north, fighters[i]}, dp, false);		
	if (cur_south >= 0 && cur_south_next != -1) {
		cur_max = std::max(cur_max, cur_south + cur_south_next);
	}

	return cur_max;
}

long calculate_score(int first, int second, int third) {
	std::set<int> x;
	if (first != -1) x.insert(first);
	if (second != -1) x.insert(second);
	if (third != -1) x.insert(third);

	return x.size() * 1000;
}

// This function is essentially a copy of max_score_two,
// but adjusted to work with (and only with) m == 3.
// Who needs refactoring anyway.
long max_score_three(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp) {
	if (i == fighters.size()) return 0;

	long cur_max = -1;

	long cur_north = 0;
	cur_north += calculate_score(p.last_last_figher_north, p.last_figher_north, fighters[i]);
	cur_north -= 1 << abs(p.imbalance - 1);
	param north_p = {
		p.imbalance - 1,
		fighters[i], p.last_figher_south,
		p.last_figher_north, p.last_last_figher_south
	};
	long cur_north_next = max_score_dp(fighters, i + 1, north_p, dp, true);		
	if (cur_north >= 0 && cur_north_next != -1) {
		cur_max = std::max(cur_max, cur_north + cur_north_next);
	}

	long cur_south = 0;
	cur_south += calculate_score(p.last_last_figher_south, p.last_figher_south, fighters[i]);
	cur_south -= 1 << abs(p.imbalance + 1);
	param south_p = {
		p.imbalance + 1,
		p.last_figher_north, fighters[i],
		p.last_last_figher_north, p.last_figher_south
	};
	long cur_south_next = max_score_dp(fighters, i + 1, south_p, dp, true);		
	if (cur_south >= 0 && cur_south_next != -1) {
		cur_max = std::max(cur_max, cur_south + cur_south_next);
	}

	return cur_max;
}

long max_score(std::vector<int> &fighters, size_t i, param p, std::vector<std::vector<long>> &dp, bool three) {
	if (!three) {
		return max_score_two(fighters, i, p, dp);
	}
	
	return max_score_three(fighters, i, p, dp);
}

long testcase() {
	size_t n, k, m; std::cin >> n >> k >> m;
	std::vector<int> fighters(n);
	for (size_t i = 0; i < n; ++i)
		std::cin >> fighters[i];

	std::vector<std::vector<long>> dp(n, std::vector<long>(max_encoder, 0));

	return max_score_dp(fighters, 0, {0, -1, -1, -1, -1}, dp, m == 3);
}

int main() {
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;;
}