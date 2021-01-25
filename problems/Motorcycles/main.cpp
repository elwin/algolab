// For this problem we notice that when comparing two riders that
// ride in the same direction (i.e. up or down on the y-axis),
// the one with the steeper angle will eventually crash into
// the other. Thus we can order the riders according to their
// angle in relation to the x-axis and greedily add riders
// that won't crash. Regardless whether a rider will crash
// or not, we keep track of the leftmost (or rather highest)
// and rightmost (lowest) rider, since those will be the
// only relevant riders for the future ones.
// Nasty: long didn't cut it since multiplcation in this case
// would lead to overflow, thus weird 128 bit numbers.

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <boost/multiprecision/cpp_int.hpp>

struct biker {
	boost::multiprecision::int128_t y0, x1, y1;
	size_t id;
};

// -1: Riding up
//  1: Riding down
//  0: Riding horizontal
int direction(biker a) {
	if (a.y1 > a.y0) return -1;
	if (a.y1 < a.y0) return  1;
	return 0;
}

// Order according to absolute angle, 
// on equality order according to direction,
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

void testcase() {
	size_t n; std::cin >> n;
	std::vector<biker> bikers(n);
	std::vector<bool> winners(n);
	for (size_t i = 0; i < n; ++i) {
		biker b;
		std::cin >> b.y0 >> b.x1 >> b.y1;
		b.id = i;
		bikers[i] = b;
	}

	std::sort(bikers.begin(), bikers.end(), smaller_angle_abs);

	biker leftmost = bikers[0];
	biker rightmost = bikers[0];
	winners[bikers[0].id] = true;

	for (size_t i = 1; i < bikers.size(); ++i) {
		biker b = bikers[i];
		if (direction(b) == 0) {
			winners[b.id] = true;
		} else if (direction(b) == -1) {
			if (b.y0 > leftmost.y0) {
				winners[b.id] = true;
			}
		} else if (direction(b) == 1) {
			if (b.y0 < rightmost.y0) {
				winners[b.id] = true;
			}
		}
		
		if (b.y0 > leftmost.y0) leftmost = b;
		if (b.y0 < rightmost.y0) rightmost = b;
	}

	for (size_t i = 0; i < n; i++) {
		if (winners[i])
			std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}