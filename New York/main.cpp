#include <iostream>
#include <vector>

bool possible(size_t cur, size_t l, size_t min, size_t max, size_t risk, std::vector<size_t> &t, std::vector<std::vector<size_t>> &c) {
	min = std::min(min, t[cur]);
	max = std::max(max, t[cur]);
	if (max - min > risk) return false;

	if (l <= 1) return true;

	for (size_t neighbor : c[cur]) {
		if (possible(neighbor, l - 1, min, max, risk, t, c)) return true;
	}

	return false;
}

void testcase() {
	std::ios_base::sync_with_stdio(false);
	size_t n, m, k; std::cin >> n >> m >> k;

	std::vector<size_t> temperatures(n);
	std::vector<std::vector<size_t>> connections(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> temperatures[i];
		connections[i] = std::vector<size_t>(0);
	}

	for (size_t i = 0; i < n - 1; ++i) {
		size_t u, v; std::cin >> u >> v;
		connections[u].push_back(v);
	}

	bool can_do = false;
	for (size_t i = 0; i < n; ++i) {
		if (possible(i, m, temperatures[i], temperatures[i], k, temperatures, connections)) {
			can_do = true;
			std::cout << i << " ";
		}
	}

	if (!can_do) std::cout << "Abort mission" << std::endl;
	else std::cout << std::endl;
}

int main() {
	size_t t; std::cin >> t;
	while (t--) testcase();
}