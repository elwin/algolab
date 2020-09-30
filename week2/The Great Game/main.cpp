#include <iostream>
#include <vector>
#include <set>

bool holmes_wins(int move, int r, int b);

int n;
std::vector<std::vector<int>> connections;
std::vector<std::vector<std::vector<bool>>> dp;
std::vector<std::vector<std::vector<bool>>> dp_seen;

bool holmes_wins_cached(int move, int r, int b) {
	move = move % 4;
	if (!dp_seen[move][r][b]) {
		dp[move][r][b] = holmes_wins(move, r, b);
		dp_seen[move][r][b] = true;
	}

	return dp[move][r][b];
}

// +---+----------+-------+
// |   |  Player  | Color |
// +---+----------+-------+
// | 0 | Holmes   | Red   |
// | 1 | Moriarty | Black |
// | 2 | Holmes   | Black |
// | 3 | Moriarty | Red   |
// +---+----------+-------+
bool holmes_wins(int move, int r, int b) {
	if (r == n - 1) { return true; }
	if (b == n - 1) { return false; }

	move = move % 4;
	
	if (move == 0) {
		for (int next: connections[r]) {
			if (holmes_wins_cached(move + 1, next, b)) {
				return true;
			}
		}
		return false;
	}

	if (move == 1) {
		for (int next: connections[b]) {
			if (!holmes_wins_cached(move + 1, r, next)) {
				return false;
			}
		}
		return true;
	}

	if (move == 2) {
		for (int next: connections[b]) {
			if (holmes_wins_cached(move + 1, r, next)) {
				return true;
			}
		}
		return false;
	}

	if (move == 3) {
		for (int next: connections[r]) {
			if (!holmes_wins_cached(move + 1, next, b)) {
				return false;
			}
		}
		return true;
	}

	throw "unexpected case"
}

int testcase() {
	int m, r, b; std::cin >> n >> m >> r >> b;
	std::cout << "n: " << n << " m: " << m << " r: " << r << " b: " << b << std::endl;

	connections = std::vector<std::vector<int>>(n);
	for (int i = 0; i < m; ++i) {
		int u, v; std::cin >> u >> v;

		connections[u - 1].push_back(v - 1);
	}

	dp = std::vector<std::vector<std::vector<bool>>>(4);
	dp_seen = std::vector<std::vector<std::vector<bool>>>(4);
	for (int move = 0; move < 4; ++move) {
		dp[move] = std::vector<std::vector<bool>>(n);
		dp_seen[move] = std::vector<std::vector<bool>>(n);
		for (int i = 0; i < n; ++i) {
			dp[move][i] = std::vector<bool>(n);
			dp_seen[move][i] = std::vector<bool>(n);
		}
	}

	// std::cout << "prepared" << std::endl;

	bool result = holmes_wins_cached(0, r - 1, b - 1) == true ? 0 : 1;

	// for (int move = 0; move < 4; ++move) {
	// 	for (int i = 0; i < n; ++i) {
	// 		for (int j = 0; j < n; ++j) {
	// 			std::cout << dp_seen[move][i][j] << " ";
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << "==" << std::endl;	
	// }


	// std::cout << "calculated" << std::endl;

	return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}