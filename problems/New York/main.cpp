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
// Also, remember from which points we already started and
// abort when encountering such points.
// Slightly inspired by daniCh8's solution

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
		reverse[v] = u;
		leaf[u] = false;
	}

	std::vector<bool> visited(n);
	std::vector<bool> startable(n);

	for (size_t i = 0; i < n; ++i) {
		if (!leaf[i] || visited[i]) continue;
		visited[i] = true;

		size_t start = i; size_t end = start;
		size_t min = temperatures[end]; size_t max = temperatures[end];
		std::multiset<size_t> temp_window;
		temp_window.insert(temperatures[end]);

		while (true) {
			if (max - min <= k && temp_window.size() == m)
				startable[end] = true;

			if (max - min > k || temp_window.size() == m) {
				visited[start] = true;

				temp_window.erase(temp_window.find(temperatures[start]));
				start = reverse[start];
				if (visited[start]) break;
			}

			if (end == 0) break;
			
			end = reverse[end];
			temp_window.insert(temperatures[end]);

			min = *(temp_window.begin());
			max = *(--temp_window.end());
		}
	}

	bool can_do = false;
	for (size_t i = 0; i < n; ++i) {
		if (startable[i]) {
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