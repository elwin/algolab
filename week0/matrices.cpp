#include <iostream>
#include <vector>

bool evenness(std::vector<std::vector<bool>> even, int i0, int i1, int j0, int j1) {
	if (i0 == i1 && j0 == j1) {
		return even[i0][j0];
	}

	return even[i0][j0] == even[i1][j1];
}

int testcase() {
	int n; std::cin >> n;

	// even[i] is true iff all entries up and inclusive i sum up to an even number
	std::vector<std::vector<bool>> even(n);

	for (int i = 0; i < n; ++i) {
		even[i] = std::vector<bool>(n);

		for (int j = 0; j < n; ++j) {
			int next; std::cin >> next;
			
			if (i > 0 && j > 0) {
				even[i][j] = ((next % 2 == 0) + even[i - 1][j - 1] + even[i - 1][j] + even[i][j - 1]) % 2 == 0;
			} else if (i > 0) {
				even[i][j] = (next % 2 == 0) == even[i - 1][j];
			} else if (j > 0) {
				even[i][j] = (next % 2 == 0) == even[i][j - 1];
			} else {
				even[0][0] = next % 2 == 0;
			}
		}
	}

	int evens = 0;

	for (int i0 = 0; i0 < n; ++i0)
	{
		for (int i1 = i0; i1 < n; ++i1)
		{
			for (int j0 = 0; j0 < n; ++j0)
			{
				for (int j1 = j0; j1 < n; ++j1)
				{
					if (evenness(even, i0, i1, j0, j1)) {
						evens++;
					}
				}
			}
		}
	}


	// for (int i = 0; i < n; ++i)
	// {
	// 	for (int j = 0; j < n; ++j)
	// 	{
	// 		std::cout << even[i][j] << ' ';
	// 	}
	// 	std::cout << std::endl;
	// }

	return evens;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		int result = testcase();
		std::cout << result << '\n';
	}

	std::cout << std::flush;
}