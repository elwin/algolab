// This problem can be solved somewhat greedily:
// Once both the friends and boxes are sorted in descending order,
// we notice that a weaker friend can lift a subset of the boxes
// that a stronger friend could carry.
// With that in mind, we try to keep as many friends occupied as
// possible, i.e. each person carries k = #boxes / #friends many boxes
// (modulo remainder). This is only possible when the strongest friend
// is able to carry the first k boxes, which can be easily checked
// using the weight of the first (i.e. heaviest) box. The second
// friend should carry the next k boxes, which can be once again
// checked using the k-th box and so on.
// If this is not possible, we need to increase k, i.e. the stronger
// will carry more boxes, leading to the weaker receiveing lighter
// boxes (or not having to carry at all, when part of a remainder).
// The worst case is when k = #boxes, i.e. the strongest friend
// needs to carry all boxes.
// This k can be found using binary search (linear suffices
// as well, surprisingly).
#include <iostream>
#include <vector>
#include <algorithm>

bool valid(std::vector<size_t> &friends, std::vector<size_t> &boxes, size_t k) {
	for (size_t i = 0; i < friends.size(); i++) {
		if (i * k >= boxes.size()) return true;
		if (friends[i] < boxes[i * k]) return false;
	}

	return true;
}

void testcase() {
	size_t f, b;
	std::cin >> f >> b;
	std::vector<size_t> friends(f);
	std::vector<size_t> boxes(b);

	for (size_t i = 0; i < f; i++)
		std::cin >> friends[i];

	for (size_t i = 0; i < b; i++)
		std::cin >> boxes[i];

	std::sort(friends.begin(), friends.end(), std::greater<size_t>());
	std::sort(boxes.begin(), boxes.end(), std::greater<size_t>());

	if (friends[0] < boxes[0]) {
		std::cout << "impossible" << std::endl;
		return;
	}

	for (size_t k = std::max(b / f, size_t(1)); ; k++) {
		if (valid(friends, boxes, k)) {
			std::cout << (k * 3 - 1) << std::endl;
			return;
		}
	}
}

int main() {
	size_t t; std::cin >> t;
	while (t--) testcase();
}