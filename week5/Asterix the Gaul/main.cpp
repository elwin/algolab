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

int search(std::vector<movement> &movements, std::vector<Num> &sips, Num D, Num T, size_t l, size_t u) {
	if (sips.size() == 0) return -1;
	if (u == l) return max_distance(movements, movements.size() - 1, T, sips[l]) >= D ? l : -1;

	if (u - 1 == l) {
		if (search(movements, sips, D, T, l, l) != -1) return l;
		if (u < sips.size() && search(movements, sips, D, T, u, u) != -1) return u;
		return -1;
	}

	size_t middle = (l + u - 1) / 2;
	Num cur = max_distance(movements, movements.size() - 1, T, sips[middle]);

	if (cur < D) {
		return search(movements, sips, D, T, middle + 1, u);
	}

	return search(movements, sips, D, T, l, middle + 1);
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

	// for (size_t i = 0; i < m; ++i) {
	// 	if (max_distance(movements, movements.size() - 1, T, sips[i]) >= D) {
	// 		return i + 1;
	// 	}
	// }

	int i = search(movements, sips, D, T, 0, sips.size());

	return i > -1 ? i + 1 : -1;
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