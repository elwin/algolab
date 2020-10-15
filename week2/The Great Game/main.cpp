#include <iostream>
#include <vector>
#include <set>
#include <limits>

int shortest_path(int i, bool adversary, int move_nr);

int n;
std::vector<std::vector<int>> connections;
std::vector<std::vector<std::vector<int>>> dp;

int shortest_path_cached(int i, bool adversary, int move_nr) {
	if (dp[adversary][move_nr][i] == 0) {
		dp[adversary][move_nr][i] = shortest_path(i, adversary, move_nr);
	}

	return dp[adversary][move_nr][i];
}

// +---+----------+-------+
// |   | Player   | Color |
// +---+----------+-------+
// | 0 | Holmes   | Red   |
// | 1 | Moriarty | Black |
// | 2 | Holmes   | Black |
// | 3 | Moriarty | Red   |
// +---+----------+-------+
//
// +----------+-----------+
// | Move Nr. | Color     |
// +----------+-----------+
// | 0, 1     | current   |
// | 2, 3     | adversary |
// +----------+-----------+
int shortest_path(int i, bool adversary, int move_nr) {
	
	// Minimum length is 1 so we can distinguish in our dp table
	if (i == n - 1) { return 1; }
	
	// Latter two moves are for the opposite color, so just add 1
	if (move_nr > 1) {
		return shortest_path_cached(i, !adversary, (move_nr + 1) % 4) + 1;
	}

	int cur_path = adversary ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
	for (int next: connections[i]) {
		int path_length = shortest_path_cached(next, !adversary, (move_nr + 1)) + 1;
		if ((adversary && path_length > cur_path) || (!adversary && path_length < cur_path)) {
			cur_path = path_length;
		}
	}

	return cur_path;
}

bool holmes_wins(int r, int b) {
	int holmes_length = shortest_path_cached(r, false, 1);
	int moriarty_length = shortest_path_cached(b, false, 0);

	return holmes_length <= moriarty_length;
}

int testcase() {
	int m, r, b; std::cin >> n >> m >> r >> b;

	connections = std::vector<std::vector<int>>(n);
	for (int i = 0; i < m; ++i) {
		int u, v; std::cin >> u >> v;

		connections[u - 1].push_back(v - 1);
	}

	dp = std::vector<std::vector<std::vector<int>>>(2);
	for (int i = 0; i < 2; ++i) {
		dp[i] = std::vector<std::vector<int>>(4);
		for (int j = 0; j < 4; ++j) {
			dp[i][j] = std::vector<int>(n + 1);
		}
	}

	return holmes_wins(r - 1, b - 1) == true ? 0 : 1;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}