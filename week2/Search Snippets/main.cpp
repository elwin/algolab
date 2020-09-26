#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct tuple {
	int position, word;
};

bool operator < (const tuple& x, const tuple& y) {
    return x.position < y.position;
}

std::vector<tuple> positions;

int smallest_snippet(std::vector<int> word_counts, int i, int j) {
	if (j - i <= word_counts.size()) {
		return j - i;
	}

	int left_snippet = std::numeric_limits<int>::max();
	int right_snippet = std::numeric_limits<int>::max();

	if (word_counts[positions[i].word] > 1) {
		std::vector<int> left = word_counts; left[positions[i].word]--;
		left_snippet = smallest_snippet(left, i + 1, j);
	}

	if (word_counts[positions[j - 1].word] > 1) {
		std::vector<int> right = word_counts; right[positions[j - 1].word]--;
		right_snippet = smallest_snippet(right, i, j - 1);
	}

	return std::min(std::min(left_snippet, right_snippet), j - i);
}

int testcase() {
	int n; std::cin >> n;
	std::vector<int> word_lengths(n);
	positions = std::vector<tuple>(0);
	
	for (int i = 0; i < n; ++i) {
		std::cin >> word_lengths[i];
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < word_lengths[i]; ++j) {
			int position; std::cin >> position;
			positions.push_back(tuple{position, i});
		}
	}

	std::sort(positions.begin(), positions.end());

	return smallest_snippet(word_lengths, 0, positions.size());
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}