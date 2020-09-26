#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <set>

struct identifier {
	int position, letter;
};

bool operator < (const identifier& x, const identifier& y) {
    return x.position < y.position;
}

std::vector<identifier> positions;
std::vector<int> letter_cache;
std::set<int> missing;

bool valid() {
	return missing.size() == 0;
}

int testcase() {
	int n; std::cin >> n;
	std::vector<int> word_counts(n);
	positions = std::vector<identifier>(0);

	for (int i = 0; i < n; ++i) {
		std::cin >> word_counts[i];
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j< word_counts[i]; ++j) {
			int position; std::cin >> position;
			positions.push_back(identifier{position, i});
		}
	}

	std::sort(positions.begin(), positions.end());

	letter_cache = std::vector<int>(n, 0);
	missing = std::set<int>();
	for (int i = 0; i < n; ++i) { missing.insert(i); }

	long unsigned int i = 0; long unsigned int j = 0;

	int min_length = std::numeric_limits<int>::max();

	while (j < positions.size()) {
		// std::cout << "i: " << i << " j: " << j << std::endl;
		// for (long unsigned int i = 0; i < letter_cache.size(); ++i) {
		// 	std::cout << letter_cache[i] << " ";
		// }

		// std::cout << std::endl;

		if (min_length == n) {
			return min_length;
		}

		if (valid()) {
			int length = 0;
			if (j > 0) {
				length += positions[j - 1].position - positions[i].position + 1;
			}

			if (length < min_length) {
				min_length = length;
			}

			// std::cout << "length: " << length << std::endl;

			// remove first letter
			int letter = positions[i++].letter;
			letter_cache[letter]--;

			if (letter_cache[letter] == 0) {
				missing.insert(letter);
			}
		} else {
			// add next letter
			int letter = positions[j++].letter;
			letter_cache[letter]++;
			
			// freshly appeared
			if (letter_cache[letter] == 1) {
				missing.erase(missing.find(letter));
			}
		}
		
	}

	return min_length;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}