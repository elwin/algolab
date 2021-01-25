#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <boost/multiprecision/cpp_int.hpp>


struct biker {
	boost::multiprecision::int128_t y0, x1, y1;
	size_t id;
};

int direction(biker a) {
	if (a.y1 > a.y0) return -1;
	if (a.y1 < a.y0) return  1;
	return 0;
}


bool smaller_angle_abs(biker a, biker b) {
	if ((a.y1 - a.y0) * b.x1 == (b.y1 - b.y0) * a.x1) {
		if (direction(a) == -1)
			return a.y0 < b.y0;
		else if (direction(a) == 1)
			return a.y0 > b.y0;
	}

	if (boost::multiprecision::abs(a.y1 - a.y0) * b.x1 == boost::multiprecision::abs(b.y1 - b.y0) * a.x1)
		return a.y1 - a.y0 > b.y1 - b.y0;

	return boost::multiprecision::abs(a.y1 - a.y0) * b.x1 < boost::multiprecision::abs(b.y1 - b.y0) * a.x1;
}

bool equal_angle(biker a, biker b) {
	return (a.y1 - a.y0) * b.x1 == (b.y1 - b.y0) * a.x1;
}

void testcase() {
	size_t n; std::cin >> n;
	std::vector<biker> bikers(n);
	for (size_t i = 0; i < n; ++i) {
		biker b;
		std::cin >> b.y0 >> b.x1 >> b.y1;
		b.id = i;
		bikers[i] = b;
	}

	std::sort(bikers.begin(), bikers.end(), smaller_angle_abs);

	// for (size_t i = 1; i < bikers.size(); ++i) {
	// 	if (!equal_angle(bikers[i - 1], bikers[i])) {
	// 		std::cout << "not equal" << std::endl;
	// 	} else {
	// 		std::cout << "equal" << std::endl;
	// 	}
	// }


	// for (size_t i; i < bikers.size(); i++) {
	// 	biker b = bikers[i]
	// 	// std::cout << b.y0 << " " << b.x1 << " " << b.y1 << std::endl;
	// 	auto angle = b.y1.convert_to<double>() - b.y0.convert_to<double>() / b.x1.convert_to<double>();
	// 	std::cout << angle << std::endl;
	// }

	std::map<boost::multiprecision::int128_t, biker> winners;

	for (biker b : bikers) {
		if (winners.size() == 0 || direction(b) == 0) {
			winners[b.y0] = b;
			continue;
		}

		if (direction(b) == -1) {
			biker c = (--winners.end())->second;
			// if (direction(b) != direction(c)) {
			// 	winners[b.y0] = b;
			// 	continue;
			// }

			// if (b.y0 > c.y0 || equal_angle(b, c)) {
			// 	winners[b.y0] = b;
			// 	continue;
			// }


			if (b.y0 > c.y0) {
				winners[b.y0] = b;
				continue;
			}
		}

		if (direction(b) == 1) {
			biker c = winners.begin()->second;
			// if (b.y0 < c.y0 && direction(b) != direction(c)) {
			// 	winners[b.y0] = b;
			// 	continue;
			// }

			if (b.y0 < c.y0) {
				winners[b.y0] = b;
				continue;
			}

			// if (b.y0 < c.y0 || equal_angle(b, c)) {
			// 	winners[b.y0] = b;
			// 	continue;
			// }
		}


		// for (auto it = winners.begin(); it != winners.end(); it++) {
		// 	std::cout << it->second.id << " " ;
		// }
		// std::cout << std::endl;
	}

	std::vector<size_t> winner_sorted;
	for (auto it = winners.begin(); it != winners.end(); it++) {
		winner_sorted.push_back(it->second.id);
	}
	std::sort(winner_sorted.begin(), winner_sorted.end());

	for (size_t b : winner_sorted) {
		std::cout << b << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}