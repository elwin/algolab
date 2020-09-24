#include <iostream>
#include <vector>

int testcase() {
	int n; std::cin >> n;

	// even[i][j] is true iff all entries up and inclusive i, j sum up to an even number
	std::vector<std::vector<bool>> even(n);

	for (int i = 0; i < n; ++i)
	{
		even[i] = std::vector<bool>(n);

		for (int j = 0; j < n; ++j)
		{
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

			std::cout << even[i][j] << " ";
		}

		std::cout << std::endl;
	}

	// even_sum[i][j] contains the number of 1s up and inclusive i, j from even
	std::vector<std::vector<int>> even_sum(n);

	std::cout << "=============" << std::endl;

	for (int i = 0; i < n; ++i)
	{
		even_sum[i] = std::vector<int>(n);
		for (int j = 0; j < n; ++j)
		{
			if (even[i][j]) { even_sum[i][j]++; }

			if (i > 0) { even_sum[i][j] += even_sum[i - 1][j]; }
			if (j > 0) { even_sum[i][j] += even_sum[i][j - 1]; }
			if (i > 0 && j > 0) { even_sum[i][j] -= even_sum[i - 1][j - 1]; }

			std::cout << even_sum[i][j] << " ";
		}
		std::cout << std::endl;
	}


	int singles = 0; int even_pairs = 0; int odd_pairs = 0;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (even[i][j]) {
				singles++;

				even_pairs += even_sum[i][j] - 1;
			} else {
				odd_pairs += (i + 1) * (j + 1) - even_sum[i][j] - 1;
			}
		}
	}

	std::cout << " single: " << singles << "\n pairs even: " << even_pairs << "\n pairs odd: " << odd_pairs << std::endl;

	return singles + even_pairs + odd_pairs;
	

	// std::cout << " single: " << evens << "\n pairs even: " << even_pairs[n - 1][n - 1] << "\n pairs odd: " << odd_pairs[n - 1][n - 1] << std::endl;

	// return evens + even_pairs[n - 1][n - 1] + odd_pairs[n - 1][n - 1];

	// std::cout << "========" << std::endl;

	// for (int i = 0; i < n; ++i) {
	// 	evencount[i] = std::vector<int>(n);

	// 	for (int j = 0; j < n; ++j) {
	// 		evencount[i][j] = even[i][j];
	// 		if (i > 0) {
	// 			evencount[i][j] += evencount[i - 1][j];
	// 		}

	// 		if (j > 0) {
	// 			evencount[i][j] += evencount[i][j - 1];
	// 		}

	// 		if (i > 0 && j > 0) {
	// 			evencount[i][j] -= evencount[i - 1][j - 1];
	// 		}

	// 		std::cout << evencount[i][j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }

	// int single_evens = 0;
	// int pair_even = 0;
	// int pair_odd = 0;

	// // even[i][j] contains how many evens are contained in the upper left corner
	// std::vector<std::vector<int>> evencount(n);

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < n; ++j) {
	// 		if (even[i][j]) {
	// 			single_evens++;

	// 			if (i > 0) {
	// 				pair_even += evencount[i - 1][j];
	// 			}

	// 			if (j > 0) {
	// 				pair_even += evencount[i][j - 1];
	// 			}

	// 			if (i > 0 && j > 0) {
	// 				pair_even += evencount[i - 1][j - 1] - 1;
	// 			}
	// 		} else {
	// 			pair_odd += (i + 1) * (j + 1) - evencount[i][j] - 1;
	// 		}
	// 	}
	// }

	// std::cout << " single: " << single_evens << "\n pairs even: " << pair_even << "\n pairs odd: " << pair_odd << std::endl;

	// return single_evens + pair_even + pair_odd;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		int result = testcase();
		std::cout << result << "\n";
	}

	std::cout << std::flush;
}