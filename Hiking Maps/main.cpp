// This problem can be split into two subproblems
// 1. For each map, figure out which legs are covered by it:
//     Legs, for which both start- and end-point are within the triangle,
//     are covered by said map. This can be tested using predicates only
//     by asserting that the points are on the right side of all edges,
//     assuming the edges are all oriented clockwise.
// 2. Optimize the selection that covers all legs with minimal cost:
//     This can be done efficiently by using the sliding window algorithm.

#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

bool contains(std::vector<K::Point_2> &edge_points, K::Point_2 p) {
	for (size_t i = 0; i < 6; i += 2) {
		if (CGAL::left_turn(edge_points[i], edge_points[i + 1], p)) return false;
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
			// Make sure that the points of the edges are all in clockwise order
			if (!CGAL::right_turn(edge_points[j], edge_points[j + 1], edge_points[(j + 2) % 6])) {
				std::swap(edge_points[j], edge_points[j + 1]);
			}
		}

		maps[i] = std::vector<size_t>(0);
		for (size_t j = 0; j < leg_points.size() - 1; ++j) {
			// Only if both points of the leg are within the triangle, add a reference to the map
			if (contains(edge_points, leg_points[j]) && contains(edge_points, leg_points[j + 1]))
				maps[i].push_back(j);
		}
	}

	size_t min_length = n;

	// i corresponds to the left edge (inclusive),
	// j to the right edge (exclusive)
	size_t i = 0; size_t j = 0;

	// histogram contains the number of coverages we have
	// for each leg
	std::vector<size_t> histogram(m - 1);

	// covered keeps track how many legs are currently covered;
	// once it is equal to m - 1, all legs are covered
	size_t covered = 0;
	while (true) {
		// The current segment is invalid, thus we need to
		// increment the right side
		if (covered < m - 1) {

			// Stop once we can't increment the right side anymore
			if (j == n) break;

			for (size_t k = 0; k < maps[j].size(); ++k)
				if (histogram[maps[j][k]]++ == 0) covered++;

			j++;
		}

		// The current segment is valid, thus we're able
		// to increment the left size
		else {
			min_length = std::min(min_length, j - i);

			for (size_t k = 0; k < maps[i].size(); ++k)
				if (--histogram[maps[i][k]] == 0) covered--;

			i++;
		}
	}

	return min_length;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}