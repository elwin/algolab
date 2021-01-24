#include <iostream>

void testcase() {
	int n; std::cin >> n;
	int sum = 0; int x;

	for (int i = 0; i < n; ++i) {
		std::cin >> x;
		sum += x;
	}

	std::cout << sum << '\n';
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;

	for (int i = 0; i < t; ++i) {
		testcase();
	}

	std::cout << std::flush;
}