#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
	int n; std::cin >> n;
	std::vector<int> positions(n);

	for (int i = 0; i < n; ++i) {
		std::cin >> positions[i];
	}

	std::sort(positions.begin(), positions.end());

	// Find optimal parameters
	int max_positions = 0; int min_distance = 200;
	int i = 0; int j = 0;
	while (j < n) {
		int cur_positions = j + 1 - i;
		int distance = std::abs(positions[j] - positions[i]);

		if (distance <= 200 && cur_positions > max_positions) {
			max_positions = cur_positions;
			min_distance = 200;
		}

		if (cur_positions == max_positions && distance < min_distance) {
			min_distance = distance;
		}

		if (distance < 200) { j++; }
		else { i++; };
	}

	// Bump distance in case it is odd
	if (min_distance % 2 != 0) {
		min_distance++;
	}

	std::cout << max_positions << " " << min_distance / 2 << std::endl;

	// Find positions matching the parameters
	i = 0; j = 0;
	while (j < n) {
		int cur_positions = j + 1 - i;
		int distance = std::abs(positions[j] - positions[i]);

		if (cur_positions == max_positions && (distance + 1) / 2 == min_distance / 2) {
			std::cout << positions[i] + distance / 2 << " ";		
			if (distance % 2 != 0) {
				std::cout << positions[i] + distance / 2 + 1 << " ";
			}
		}

		if (distance < 200) { j++; }
		else { i++; };
	}

	std::cout << std::endl;

	return;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}