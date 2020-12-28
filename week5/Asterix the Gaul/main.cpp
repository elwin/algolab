#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef long long Num;

struct movement {
	Num d, t; 
};

typedef std::vector<movement> Moves;

bool compare(movement a, movement b) {
	if (a.d == b.d) return a.t < b.t;
	return a.d < b.d;
}

Moves list(Moves &m, Num sip, int i) {
	if (i == -1) return {{0, 0}};

	Moves prev = list(m, sip, i - 1);
	size_t n = prev.size();
	for (size_t j = 0; j < n; ++j) {
		prev.push_back({prev[j].d + m[i].d + sip, prev[j].t + m[i].t});
	}

	return prev;
}

Moves max_distance_list(Moves &m, Num sip) {
	Moves lm = list(m, sip, m.size() - 1);
	std::sort(lm.begin(), lm.end(), compare);
	std::reverse(lm.begin(), lm.end());

	for (size_t i = 1; i < lm.size(); ++i) {
		if (lm[i - 1].t <= lm[i].t) {
			lm.erase(lm.begin() + i);
			i--;
		}
	}

	return lm;
}

Num find_max(Moves &lm, Num remainingTime) {
	for (size_t i = 0; i < lm.size(); ++i) {
		if (lm[i].t < remainingTime) return lm[i].d;
	}

	return 0;
}

Num max_distance_rec(Moves &m, Moves &lm, Num sip, Num remainingTime, int i) {
	if (i == -1) return find_max(lm, remainingTime);

	Num ignore = max_distance_rec(m, lm, sip, remainingTime, i - 1);
	if (m[i].t < remainingTime) {
		ignore = std::max(ignore, max_distance_rec(m, lm, sip, remainingTime - m[i].t, i - 1) + m[i].d + sip);
	}

	return ignore;
}

Num max_distance(Moves &m, Num sip, Num remainingTime) {
	// std::random_shuffle(m.begin(), m.end());

	size_t middle = m.size() / 2;
	Moves a(m.begin(), m.begin() + middle);
	Moves b(m.begin() + middle, m.end());

	Moves lm = max_distance_list(a, sip);
	// for (size_t i = 0; i < lm.size(); ++i) {
	// 	std::cout << lm[i].d << " " << lm[i].t << std::endl;
	// }

	return max_distance_rec(b, lm, sip, remainingTime, b.size() - 1);
}

Num search(Moves &m, std::vector<Num> &sips, Num D, Num T) {
	for (size_t i = 0; i < sips.size(); ++i) {
		if (max_distance(m, sips[i], T) >= D) return i;
	}

	return -1;
}


int testcase() {
	size_t n, m; Num D, T; std::cin >> n >> m >> D >> T;

	Moves movements(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> movements[i].d >> movements[i].t;
	}

	std::vector<Num> sips(m + 1);
	sips[0] = 0;
	for (size_t i = 0; i < m; ++i) {
		std::cin >> sips[i + 1];
	}

	return search(movements, sips, D, T);
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