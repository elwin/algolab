#include <iostream>
#include <vector>
#include <algorithm>

struct segment {
	size_t a, b;
};

bool compare(segment x, segment y) {
	return x.b < y.b;
}

size_t length(segment x, size_t m) {
	if (x.a < x.b) return x.b - x.a;

	return m - x.a + x.b;
}

bool overlap(segment x, segment y) {
	// both segments are normal
	if (x.a < x.b && y.a < y.b) {
		return (x.a <= y.a && x.b >= y.a) || (x.a <= y.b && x.b >= y.b);
	}

	// both segments wrap around
	if (x.a > x.b && y.a > y.b) {
		return true;
	}

	// x wraps around
	if (x.a > x.b && y.a < y.b) {
		return x.a <= y.b || x.b >= y.a;
	}

	// y wraps around
	return y.a <= x.b || y.b >= x.a;
}

size_t do_something(std::vector<segment> &segments, size_t offset) {
	size_t count = 1;
	segment covered = segments[offset];

	for (size_t i_off = 1; i_off < segments.size(); ++i_off) {
		size_t i = (i_off + offset) % segments.size();

		if (overlap(segments[i], covered)) {
			continue;
		}

		count++;
		covered.b = segments[i].b;
	}

	return count;
}

size_t testcase() {
	size_t n, m; std::cin >> n >> m;
	std::vector<segment> segments(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> segments[i].a >> segments[i].b;
		segments[i].a--; segments[i].b--;
	}

	if (n == 0) return 0;

	std::sort(segments.begin(), segments.end(), compare);

	size_t j = 0;
	for (size_t i = 1; i < n; ++i) {
		if (length(segments[i], m) < length(segments[j], m)) {
			j = i;
		}
	}

	size_t max = 0;
	size_t i = j;
	do {
		max = std::max(max, do_something(segments, i));
		i = (i + 1) % n;
	} while (overlap(segments[j], segments[i]) && i != j);

	return max;
}

int main() {
	size_t n; std::cin >> n;
	for (size_t i = 0; i < n; ++i) {
		std::cout << testcase() << std::endl;
	}

	return 0;
}