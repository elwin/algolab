#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

bool contains(std::vector<K::Point_2> &edge_points, K::Point_2 p) {
	for (size_t i = 0; i < 6; i += 2) {
		if (CGAL::left_turn(edge_points[i], edge_points[i + 1], p)) return false;
	}

	return true;
}

bool valid(std::vector<std::vector<size_t>> &maps, size_t m, size_t i, size_t j) {
	std::vector<bool> covered(m);
	for (; i < j; ++i) {
		for (size_t k = 0; k < maps[i].size(); ++k) {
			covered[maps[i][k]] = true;
		}
	}

	for (size_t k = 0; k < covered.size(); ++k) {
		if (!covered[k]) {
			return false;
		}
	}

	return true;
}

size_t testcase() {
	size_t m, n; std::cin >> m >> n;
	std::vector<K::Point_2> leg_points(m);
	for (size_t i = 0; i < m; ++i) {
		int x, y; std::cin >> x >> y;
		leg_points[i] = K::Point_2(x, y);
	}

	std::vector<std::vector<size_t>> maps(n);
	for (size_t i = 0; i < n; ++i) {
		std::vector<K::Point_2> edge_points(6);
		for (size_t j = 0; j < 6; ++j) {
			int x, y; std::cin >> x >> y;
			edge_points[j] = K::Point_2(x, y);
		}

		for (size_t j = 0; j < 6; j += 2) {
			if (!CGAL::right_turn(edge_points[j], edge_points[j + 1], edge_points[(j + 2) % 6])) {
				std::swap(edge_points[j], edge_points[j + 1]);
			}
		}

		maps[i] = std::vector<size_t>(0);
		for (int j = 0; j < leg_points.size() - 1; ++j) {
			if (contains(edge_points, leg_points[j]) && contains(edge_points, leg_points[j + 1])) {
				maps[i].push_back(j);
			}
		}

		// for (size_t j = 0; j < edge_points.size(); ++j) {
		// 	std::cout << edge_points[j] << " ";
		// }
		// std::cout << std::endl;
	}

	// for (size_t i = 0; i < maps.size(); i++) {
	// 	std::cout << i << ": "; 
	// 	for (size_t j = 0; j < maps[i].size(); j++) {
	// 		std::cout << maps[i][j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	size_t min_length = n;
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = i + 1; j < n + 1; ++j) {
			if (valid(maps, m - 1, i, j)) {
				min_length = std::min(min_length, j - i);
			}
		}
	}

	return min_length;


}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}