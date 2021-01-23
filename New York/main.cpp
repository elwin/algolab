// The graph can be treated as a tree, thus we can start at the leaves
// and work our way upwards. This has the benefit that we will not
// encounter any branches (and perhaps some other benefits, not
// entirely sure though).
// To pass all test-sets we need to run in roughly linear time. This
// can be done by using a sliding window approach and keep track of
// the length (i.e. only start sliding once we reached the desired
// lenght) and all the temperatures of the window. Apparently,
// multiset (and perhaps sets in general) are sorted, thus
// we can use this to efficiently retrieve the min and max
// temperature.

#include <iostream>
#include <vector>
#include <set>

void testcase() {
	size_t n, m, k; std::cin >> n >> m >> k;

	std::vector<size_t> temperatures(n);
	std::vector<size_t> reverse(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> temperatures[i];
	}

	std::vector<bool> leaf(n, true);
	for (size_t i = 0; i < n - 1; ++i) {
		size_t u, v; std::cin >> u >> v;
		reverse[v] = u + 1; // We treat the 0-value as non-existent
		leaf[u] = false;
	}

	std::vector<bool> possible_start(n);
	for (size_t i = 0; i < n; ++i) {
		if (!leaf[i]) continue;

		std::multiset<size_t> temperature_window;
		size_t start = i;
		size_t end = i;
		temperature_window.insert(temperatures[end]);
		while (reverse[end] > 0 && temperature_window.size() < m) {
			end = reverse[end] - 1;
			temperature_window.insert(temperatures[end]);
		}

		if (temperature_window.size() != m) continue;

		{
			size_t min = *(temperature_window.begin());
			size_t max = *(--temperature_window.end());
			if (max - min <= k) possible_start[end] = true;
		}

		while (reverse[end] > 0) {
			temperature_window.erase(temperature_window.find(temperatures[start]));
			start = reverse[start] - 1;

			end = reverse[end] - 1;
			temperature_window.insert(temperatures[end]);

			size_t min = *(temperature_window.begin());
			size_t max = *(--temperature_window.end());
			if (max - min > k) continue;

			possible_start[end] = true;
		}
	}

	bool can_do = false;
	for (size_t i = 0; i < n; ++i) {
		if (possible_start[i]) {
			std::cout << i << " ";
			can_do = true;
		}
	}

	if (!can_do) std::cout << "Abort mission" << std::endl;
	else std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}