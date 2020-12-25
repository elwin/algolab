#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef long long Num;

struct movement {
	Num d, t; 
};

Num max_distance(std::vector<movement> &movements, int i, Num remainingTime, Num sip) {
	if (i == -1) return 0;

	Num max = max_distance(movements, i - 1, remainingTime, sip);
	if (movements[i].t < remainingTime) {
		max = std::max(max, max_distance(movements, i - 1, remainingTime - movements[i].t, sip) + movements[i].d + sip);
	}

	return max;
}


int testcase() {
	size_t n, m; Num D, T; std::cin >> n >> m >> D >> T;
	
	std::vector<movement> movements(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> movements[i].d >> movements[i].t;
	}

	std::vector<Num> sips(m);
	for (size_t i = 0; i < m; ++i) {
		std::cin >> sips[i];
	}

	if (max_distance(movements, movements.size() - 1, T, 0) >= D) {
		return 0;
	}

	for (size_t i = 0; i < m; ++i) {
		if (max_distance(movements, movements.size() - 1, T, sips[i]) >= D) {
			return i + 1;
		}
	}

	return -1;
}


int main() {
	size_t t; std::cin >> t;
	while (t--) {
		int out = testcase();
		if (out == -1) {
			std::cout << "Panoramix captured" << std::endl;
		} else {
			std::cout << out << std::endl;
		}
	}
}