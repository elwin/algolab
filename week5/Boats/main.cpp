#include <iostream>
#include <vector>
#include <algorithm>

struct boat {
	int length, ring;
};

bool compare(boat &a, boat &b) {
	return a.ring < b.ring;
}

size_t testcase() {
	size_t n; std::cin >> n;

	std::vector<boat> boats(n);

	for (size_t i = 0; i < n; ++i) {
		std::cin >> boats[i].length >> boats[i].ring;
	}

	if (n == 0) return 0;

	std::sort(boats.begin(), boats.end(), compare);

	// position ring on the right end of the boat
	int current_end = boats[0].ring;
	size_t count = 1;

	for (size_t i = 1; i < n; ++i) {
		next:

		// can't even place the current boat, just skip
		if (boats[i].ring < current_end) {
			continue;
		}

		// current boat is so far that we can position the ring
		// once again on the right end of the boat
		if (boats[i].ring - boats[i].length >= current_end) {
			current_end = boats[i].ring;
			count++;

			continue;
		}

		// Check for following boats (until we reach a point
		// where the rings are placed further away than the
		// end of the current boat) to figure out whether
		// there is a boat that can be placed closer
		// than the current boat. If so, jump to
		// this boat, otherwise we'll proceed
		// to place the current boat.
		if (i + 1 < n) {
			size_t end = current_end + boats[i].length;

			size_t j = i + 1;
			while (current_end < boats[j].ring && j < n) {
				size_t next_end = std::max(current_end + boats[j].length, boats[j].ring);

				if (next_end < end) {
					i = j;
					goto next;
				}

				j++;
			}
			
		}

		// apparently we shall place the current boat -
		// do it is as far left as possible
		current_end += boats[i].length;
		count++;
	}

	return count;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	size_t n; std::cin >> n;
	for (size_t i = 0; i < n; ++i) {
		std::cout << testcase() << std::endl;
	}

	return 0;
}