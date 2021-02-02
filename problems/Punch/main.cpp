#include <iostream>
#include <vector>
#include <limits>

// <cost, volume>
typedef std::vector<std::pair<size_t, size_t>> D;

// <cost, variation>
typedef std::pair<int, int> Res;

// index -> remaining -> <subsequent, non_subsequent>
typedef std::vector<std::vector<std::pair<Res, Res>>> DP;

Res min_cost(D &drinks, size_t i, int remaining, bool subsequent, DP &dp) {
	if (remaining <= 0) return {0, 0};
	if (i == drinks.size()) return {10000, 0};

	auto d = drinks[i];

	Res take = min_cost(drinks, i, remaining - d.second, true, dp);
	take.first += d.first;
	if (!subsequent) take.second--;

	Res ignore = min_cost(drinks, i + 1, remaining, false, dp);

	return std::min(take, ignore);
}

void testcase() {
	size_t n, k; std::cin >> n >> k;
	D drinks(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> drinks[i].first >> drinks[i].second;
	}

	DP dp(n, std::vector<std::pair<Res, Res>>(k + 1));

	Res r = min_cost(drinks, 0, k, false, dp);

	std::cout << r.first << " " << -r.second << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) {
		testcase();
	}
}