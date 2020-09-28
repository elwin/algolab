#include <iostream>
#include <vector>
#include <set>

struct tuple {
	int i0, i1, j0, j1;
};

bool operator < (const tuple& a, const tuple& b) {
	return a.i0 < b.i0 ||
		(a.i0 == b.i0 && (a.i1 < b.i1 ||
			(a.i1 < b.i1 && (a.j0 == b.j0 ||
				(a.j0 == b.j0 && a.j1 < b.j1)))));
}


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

	std::vector<std::vector<bool>> first(n);
	for (int i = 0; i < n; ++i) {
		first[i] = std::vector<bool>(n);
		for (int j = 0; j < n; ++j) {
			int next; std::cin >> next;
			first[i][j] = next == 1;
		}
	}

	// second[i][j] is true iff the sum up to (and inclusive) i, j is even
	std::vector<std::vector<bool>> second(n);
	for (int i = 0; i < n; ++i) {
		second[i] = std::vector<bool>(n);
		for (int j = 0; j < n; ++j) {
			int next = first[i][j];

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

	std::vector<tuple> tuples;

	for (int i0 = 0; i0 < n; ++i0) {
		for (int j0 = 0; j0 < n; ++j0) {
			for (int i1 = i0; i1 < n; ++i1) {
				for (int j1 = j0; j1 < n; ++j1) {

					// it's not a single cell, and we have even + even = even or odd + odd = even
					if ((i0 != i1 || j0 != j1) && second[i0][j0] == second[i1][j1]) {
						evens++;
						tuples.push_back(tuple{i0, i1, j0, j1});
					}

					// it's a single cell, only take it if the original value is even (i.e. 0)
					if ((i0 == i1 && j0 == j1) && !first[i1][j1]) {
						evens++;
						tuples.push_back(tuple{i0, i1, j0, j1});
					}
				}
			}
		}
	}

	std::sort(tuples.begin(), tuples.end());

	tuple a = tuples[0];
	for (int i = 1; i < tuples.size(); ++i) {
		tuple b = tuples[i];

		if (a.i0 == b.i0 && a.i1 == b.i1 && a.j0 == b.j0 && a.j1 == b.j1) {
			return tuples[-1].i0;
		}
	}

	for (tuple a : tuples) {
		std::cout << "(" << a.i0 << ", " << a.j0 << ") (" << a.i1 << ", " << a.j1 << ")" << std::endl;
	}

	return evens;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		int result = testcase1();
		std::cout << result << "\n";

		if (i == 2) {
			break;
		}
	}

	std::cout << std::flush;
}