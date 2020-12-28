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

	Moves lm2(0);
	lm2.push_back(lm[0]);
	for (size_t i = 1; i < lm.size(); ++i) {
		if (lm2[lm2.size() - 1].t > lm[i].t) {
			lm2.push_back(lm[i]);
		}
	}

	// for (size_t i = 1; i < lm.size(); ++i) {
	// 	if (lm[i - 1].t <= lm[i].t) {
	// 		lm.erase(lm.begin() + i);
	// 		i--;
	// 	}
	// }

	return lm2;
}

Num find_max_rec(Moves &lm, Num remainingTime, size_t l, size_t u) {
	if (lm.size() == 0) return 0;
	if (u == l) return lm[u].d;

	size_t middle = (l + u - 1) / 2;
	Num cur = lm[middle].d;

	if (cur < remainingTime) {
		return find_max_rec(lm, remainingTime, middle + 1, u);
	}

	return find_max_rec(lm, remainingTime, l, middle + 1);
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
	std::random_shuffle(m.begin(), m.end());

	size_t middle = m.size() / 2;
	Moves a(m.begin(), m.begin() + middle);
	Moves b(m.begin() + middle, m.end());

	Moves lm = max_distance_list(a, sip);

	return max_distance_rec(b, lm, sip, remainingTime, b.size() - 1);
}

int search_rec(Moves &m, std::vector<Num> &sips, Num D, Num T, size_t l, size_t u) {
	if (sips.size() == 0) return -1;
	if (u == l) return max_distance(m, sips[l], T) >= D ? l : -1;

	if (u - 1 == l) {
		if (search_rec(m, sips, D, T, l, l) != -1) return l;
		if (u < sips.size() && search_rec(m, sips, D, T, u, u) != -1) return u;
		return -1;
	}

	size_t middle = (l + u - 1) / 2;
	Num cur = max_distance(m, sips[middle], T);

	if (cur < D) {
		return search_rec(m, sips, D, T, middle + 1, u);
	}

	return search_rec(m, sips, D, T, l, middle + 1);
}

int search(Moves &m, std::vector<Num> &sips, Num D, Num T) {
	// if (max_distance(m, sips[0], T) >= D) return 0;

	return search_rec(m, sips, D, T, 0, sips.size());
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
	std::ios_base::sync_with_stdio(false);
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