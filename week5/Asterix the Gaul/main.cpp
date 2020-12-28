#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef long long Num;

struct movement {
	Num d, t; 
};

bool compare(movement a, movement b) {
	if (a.d == b.d) return a.t < b.t;
	return a.d < b.d;
}

Num search_max_distance(std::vector<movement> &l, Num t) {
	for (size_t i = 0; i < l.size(); ++i) {
		if (l[i].t < t) {
			return l[i].d;
		}
	}

	return 0;
}

Num max_distance(std::vector<movement> &movements, std::vector<movement> &l, int i, Num remainingTime, Num sip) {
	if (i == -1) return search_max_distance(l, remainingTime);

	Num max = max_distance(movements, l, i - 1, remainingTime, sip);
	if (movements[i].t < remainingTime) {
		max = std::max(max, max_distance(movements, l, i - 1, remainingTime - movements[i].t, sip) + movements[i].d + sip);
	}

	return max;
}

std::vector<movement> list(std::vector<movement> &movements, int i, Num sip) {
	if (i == -1) return {{0, 0}};

	std::vector<movement> take = list(movements, i - 1, sip);
	size_t n = take.size();
	for (size_t i = 0; i < n; ++i) {
		take.push_back({take[i].d + movements[i].d + sip, take[i].t + movements[i].t});
	}

	return take;
}

Num max_distance_optimized(std::vector<movement> &movements, Num T, Num sip) {
	int middle = movements.size() / 2;
	std::vector<movement> a(movements.begin(), movements.begin() + middle);
	std::vector<movement> b(movements.begin() + middle, movements.end());
	std::vector<movement> l = list(a, a.size() - 1, sip);

	std::sort(l.begin(), l.end(), compare);
	std::reverse(l.begin(), l.end());
	for (size_t i = 1; i < l.size(); ++i) {
		if (l[i - 1].t <= l[i].t) {
			l.erase(l.begin() + i);
			i--;
		}
	}

	return max_distance(b, l, b.size() - 1, T, sip);
}

int search(std::vector<movement> &movements, std::vector<Num> &sips, Num D, Num T, size_t l, size_t u) {
	if (sips.size() == 0) return -1;
	if (u == l) return max_distance_optimized(movements, T, sips[l]) >= D ? l : -1;

	if (u - 1 == l) {
		if (search(movements, sips, D, T, l, l) != -1) return l;
		if (u < sips.size() && search(movements, sips, D, T, u, u) != -1) return u;
		return -1;
	}

	size_t middle = (l + u - 1) / 2;
	Num cur = max_distance_optimized(movements, T, sips[middle]);

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

	std::vector<Num> sips(m + 1);
	sips[0] = 0;
	for (size_t i = 0; i < m; ++i) {
		std::cin >> sips[i + 1];
	}

	return search(movements, sips, D, T, 0, sips.size());
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