#include <iostream>
#include <vector>

bool holmes_wins(int move_number, int r, int b);

// move_number, r, b
// 0: Moriarty wins
// 1: Holmes wins
// -1: Not set yet
std::vector<std::vector<std::vector<int>>> dp;
std::vector<std::vector<int>> connections;
int n;

// +---+----------+-------+
// |   |  Player  | Color |
// +---+----------+-------+
// | 0 | Holmes   | Red   |
// | 1 | Moriarty | Black |
// | 2 | Holmes   | Black |
// | 3 | Moriarty | Red   |
// +---+----------+-------+

bool holmes_wins_cache(int move_number, int r, int b) {
	move_number = move_number % 4;

	if (dp[move_number % 4][r][b] == -1) {
		dp[move_number % 4][r][b] = holmes_wins(move_number, r, b);
	}

	return dp[move_number % 4][r][b] == 1;
}

bool holmes_wins(int move_number, int r, int b) {
	if (r == n - 1) {
		return true;
	} else if (b == n - 1) {
		return false;
	}

	if (move_number % 4 == 0) {
		for (int link: connections[r]) {
			if (holmes_wins_cache(move_number + 1, link, b)) {
				return true;
			}
		}

		return false;
	}

	if (move_number % 4 == 1) {
		for (int link: connections[b]) {
			if (!holmes_wins_cache(move_number + 1, r, link)) {
				return false;
			}
		}

		return true;	
	}

	if (move_number % 4 == 2) {
		for (int link: connections[b]) {
			if (holmes_wins_cache(move_number + 1, r, link)) {
				return true;
			}
		}

		return false;
	}

	if (move_number % 4 == 3) {
		for (int link: connections[r]) {
			if (!holmes_wins_cache(move_number + 1, link, b)) {
				return false;
			}
		}

		return true;	
	}

	return -1;
}

int testcase() {
	int m; std::cin >> n >> m;
	connections = std::vector<std::vector<int>>(n);
	for (int i = 0; i < n; ++i) {
		connections[i] = std::vector<int>();
	}

	dp = std::vector<std::vector<std::vector<int>>>(4);
	for (int i = 0; i < 4; ++i) {
		dp[i] = std::vector<std::vector<int>>(n);
		for (int j = 0; j < n; ++j) {
			dp[i][j] = std::vector<int>(n, -1);
		}
	}


	int r, b; std::cin >> r >> b;
	r--; b--;

	for (int i = 0; i < m; ++i) {
		int u, v; std::cin >> u >> v;
		u--; v--;
		connections[u].push_back(v);
	}
	// 0 if Holmes wins, 1 otherwise
	return !holmes_wins(0, r, b);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    std::cout << testcase() << '\n';
  }

  std::cout << std::flush;
}