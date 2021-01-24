// For this problem we can use DP but need to make sure to bring our
// data in a suitable form.
//
// 1. For a given start of the attack it is always fixed how many defenders
//  need to be added, thus we can precompute this mapping in linear time
//  to avoid calculating this information over and over again.
//  Our mapping should have the following form, for each location
//  where the start of an attack is possible:
//   - start of the attack
//   - end of the attack
//   - pointer to next attack that is possible, if this attack is taken
//  we can infer this information efficiently using a sliding window approach.
//
// 2. Once we have this information, we can simply make DP over this list of
//  possible attacks: At each point, we either make an attack and calculate
//  the score from where the pointer leads us, or we do not make an attack
//  and calculate the score from the next possible attack location.
#include <iostream>
#include <vector>

struct detail {
	size_t start, end;
	int pointer;
};

int max_score(std::vector<detail> &windows, size_t k, int i, size_t attackers, std::vector<std::vector<int>> &dp);

int max_score_dp(std::vector<detail> &windows, size_t k, int i, size_t attackers, std::vector<std::vector<int>> &dp) {
	if (attackers == 0) return 0;
	if (i == -1 || i == int(windows.size())) return -1;
	
	if (dp[attackers][i] == 0) {
		dp[attackers][i] = max_score(windows, k, i, attackers, dp);
	}

	return dp[attackers][i];
}

int max_score(std::vector<detail> &windows, size_t k, int i, size_t attackers, std::vector<std::vector<int>> &dp) {
	if (attackers == 0) return 0;
	if (i == -1 || i == int(windows.size())) return -1;

	int ignore = max_score_dp(windows, k, i + 1, attackers, dp);
	int take = max_score_dp(windows, k, windows[i].pointer, attackers - 1, dp);

	if (ignore == -1 && take == -1) return -1;
	if (ignore == -1) return take + windows[i].end - windows[i].start;
	if (take == -1) return ignore;
	return std::max(ignore, take + int(windows[i].end - windows[i].start));
}

void testcase() {
	size_t n, m, k; std::cin >> n >> m >> k;
	std::vector<size_t> defenders(n);
	for (size_t i = 0; i < n; ++i)
		std::cin >> defenders[i];

	std::vector<detail> windows(0);
	size_t cur_score = 0;
	size_t start = 0; size_t end = 0;
	while (end <= n) {
		if (cur_score < k) cur_score += defenders[end++];
		else if (cur_score > k) cur_score -= defenders[start++];
		else {
			windows.push_back({start, end});
			cur_score += defenders[end++];
			cur_score -= defenders[start++];
		}
	}

	size_t next = 0;
	for (size_t i = 0; i < windows.size(); ++i) {
		while (next < windows.size() && windows[i].end > windows[next].start) next++;
		if (next == windows.size()) windows[i].pointer = -1;
		else windows[i].pointer = next;
	}

	std::vector<std::vector<int>>dp(m + 1);
	for (size_t i = 0; i < dp.size(); ++i)
		dp[i] = std::vector<int>(n + 1);

	int score = max_score_dp(windows, k, 0, m, dp);

	std::cout << (score == -1 ? "fail" : std::to_string(score)) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}