// More or less typical DP problem that can be solved in psuedo-polynomial time:
// At drink index i, the minimum we need to pay is
// 1) We ignore the current drink and thus equal to the minimum
//    for i + 1
// 2) We take one unit of the drink i, and thus equal to the minimum
//    for i with the reduced amount
// A bit special in this exercise is that we need to keep track
// how many different drinks we've used so far, which is a bit 
// cumbersome in the implementation. So for the result we use a pair:
// The first item corresponds to the cost and the second to the
// number of different drinks, but the negative value - this allows
// us to easily take the minimum using std::min, which, in this case,
// minimizes the cost and maximizes the number of distinct drinks.

#include <iostream>
#include <vector>
#include <limits>

// <cost, volume>
typedef std::vector<std::pair<size_t, size_t>> D;

// <cost, variation>
typedef std::pair<int, int> Res;

// index -> remaining -> <subsequent, non_subsequent>
typedef std::vector<std::vector<std::pair<Res, Res>>> DP;

Res min_cost(D &drinks, size_t i, int remaining, bool subsequent, DP &dp);

Res min_cost_dp(D &drinks, size_t i, int remaining, bool subsequent, DP &dp) {
	if (i == drinks.size() || remaining <= 0)
		return min_cost(drinks, i, remaining, subsequent, dp);

	// Is the previous drink the same as this one?
	Res *relevant = subsequent ? &(dp[i][remaining].first) : &(dp[i][remaining].second);

	if (relevant->first == 0) {
		*relevant = min_cost(drinks, i, remaining, subsequent, dp);
	}

	return *relevant;
}


Res min_cost(D &drinks, size_t i, int remaining, bool subsequent, DP &dp) {
	if (remaining <= 0) return {0, 0};

	// Initialize impossible case using a large number, but not too large
	// such that adding to it overflows.
	if (i == drinks.size()) return {std::numeric_limits<int>::max() / 2, 0};

	auto d = drinks[i];

	Res take = min_cost_dp(drinks, i, remaining - d.second, true, dp);
	take.first += d.first;

	// Only if the previous drink is different we're allowed to increase
	// the number of distinct drinks, or rather decrease (see explanation above).
	if (!subsequent) take.second--;

	Res ignore = min_cost_dp(drinks, i + 1, remaining, false, dp);

	return std::min(take, ignore);
}

void testcase() {
	size_t n, k; std::cin >> n >> k;
	D drinks(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> drinks[i].first >> drinks[i].second;
	}

	DP dp(n + 1, std::vector<std::pair<Res, Res>>(k + 1));

	Res r = min_cost_dp(drinks, 0, k, false, dp);

	std::cout << r.first << " " << -r.second << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) {
		testcase();
	}
}