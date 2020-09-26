#include <iostream>
#include <vector>

int testcase2() {
	int n; std::cin >> n;

	// second[i][j] is true iff the sum up to (and inclusive) i, j is even
	std::vector<std::vector<bool>> second(n);
	for (int i = 0; i < n; ++i) {
		second[i] = std::vector<bool>(n);
		for (int j = 0; j < n; ++j) {
			int next; std::cin >> next;

			if (i > 0 && j > 0) {
				second[i][j] = ((next % 2 == 0) + second[i - 1][j - 1] + second[i - 1][j] + second[i][j - 1]) % 2 == 0;
			} else if (i > 0) {
				second[i][j] = (next % 2 == 0) == second[i - 1][j];
			} else if (j > 0) {
				second[i][j] = (next % 2 == 0) == second[i][j - 1];
			} else {
				second[0][0] = next % 2 == 0;
			}

			std::cout << second[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << "============" << std::endl;

	std::vector<std::vector<int>> third(n);
	for (int i = 0; i < n; ++i) {
		third[i] = std::vector<int>(n);
		for (int j = 0; j < n; ++j) {
			third[i][j] = second[i][j];
			if (i > 0) { third[i][j] += third[i - 1][j]; }
			if (j > 0) { third[i][j] += third[i][j - 1]; }
			if (i > 0 && j > 0) { third[i][j] -= third[i - 1][j - 1]; }

			std::cout << third[i][j] << " ";
		}

		std::cout << std::endl;
	}

	int singles = 0; int evens = 0; int odds = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {

			if (second[i][j]) {
				singles++;

				evens += third[i][j] - 1;
			} else {
				odds += (i + 1) * (j + 1) - third[i][j] - 1;
			}
		}
	}

	std::cout << "singles: " << singles << ", evens: " << evens << ", odds: " << odds << std::endl;

	return singles + evens + odds;
}

int testcase1() {
	int n; std::cin >> n;

	// second[i][j] is true iff the sum up to (and inclusive) i, j is even
	std::vector<std::vector<bool>> second(n);
	for (int i = 0; i < n; ++i) {
		second[i] = std::vector<bool>(n);
		for (int j = 0; j < n; ++j) {
			int next; std::cin >> next;

			if (i > 0 && j > 0) {
				second[i][j] = ((next % 2 == 0) + second[i - 1][j - 1] + second[i - 1][j] + second[i][j - 1]) % 2 == 0;
			} else if (i > 0) {
				second[i][j] = (next % 2 == 0) == second[i - 1][j];
			} else if (j > 0) {
				second[i][j] = (next % 2 == 0) == second[i][j - 1];
			} else {
				second[0][0] = next % 2 == 0;
			}

			// std::cout << second[i][j] << " ";
		}
		// std::cout << std::endl;
	}

	int evens = 0;

	for (int i0 = 0; i0 < n; ++i0) {
		for (int j0 = 0; j0 < n; ++j0) {
			for (int i1 = i0; i1 < n; ++i1) {
				for (int j1 = j0; j1 < n; ++j1) {
					if ((i0 != i1 || j0 != j1) && second[i0][j0] == second[i1][j1]) {
						evens++;
					}

					if ((i0 == i1 && j0 == j1) && second[i1][j1]) {
						evens++;
					}
				}
			}
		}
	}

	return evens;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		int result = testcase1();
		std::cout << result << "\n";
	}

	std::cout << std::flush;
}