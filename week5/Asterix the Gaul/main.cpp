#include <iostream>
#include <vector>
#include <algorithm>

typedef long long Num;

struct movement {
	// TODO: d / t can be 2^50
	Num d, t; 
};

bool greater(movement a, movement b) {
	// std::cout << a.d << " " << a.t << " - " << a.d << " " << a.t << std::endl;

	if (a.d * b.t == b.d * a.t) {
		return a.d > b.d;
	}

	return a.d * b.t > b.d * a.t;
}

// bool greater(movement a, movement b) {
// 	return !less(a, b);
// }


int testcase() {
	// TODO: D / T can be 2^50
	size_t n, m; Num D, T; std::cin >> n >> m >> D >> T;
	
	std::vector<movement> movements(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> movements[i].d >> movements[i].t;
	}
	// TODO: s can be 2^50
	std::vector<Num> sips(m);
	for (size_t i = 0; i < m; ++i) {
		std::cin >> sips[i];
	}

	std::sort(movements.begin(), movements.end(), greater);
	// for (size_t i = 0; i < n; ++i) {
	// 	std::cout << movements[i].d << " " << movements[i].t << std::endl;
	// }

	for (size_t i = 0; i < n; ++i) {
		// std::cout << D << " " << T << std::endl;
		if (D <= 0) return 0;
		if (movements[i].t >= T) continue;
		T -= movements[i].t;
		D -= movements[i].d;
	}
		// std::cout << D << " " << T << std::endl;


	if (D <= 0) return 0;

	int current = 0;
	for (size_t i = 0; i < m; ++i) {
		current += sips[i];
		if (current >= D) return i + 1;
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