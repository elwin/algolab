#include <iostream>
#include <vector>

bool holmes_wins(int move_number, int r, int b);

// move_number, r, b
// 0: Moriarty wins
// 1: Holmes wins
// -1: Not set yet
std::vector<std::vector<std::vector<bool>>> dp;
std::vector<std::vector<std::vector<bool>>> dp_seen;
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

	if (!dp_seen[move_number][r][b]) {
		dp[move_number][r][b] = holmes_wins(move_number, r, b);
		dp_seen[move_number][r][b] = true;
	}

	return dp[move_number][r][b];
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

	std::cout << "Initialized conns" << std::endl;

	dp = std::vector<std::vector<std::vector<bool>>>(4);
	dp_seen = std::vector<std::vector<std::vector<bool>>>(4);
	for (int i = 0; i < 4; ++i) {
		dp[i] = std::vector<std::vector<bool>>(n);
		dp_seen[i] = std::vector<std::vector<bool>>(n);
		for (int j = 0; j < n; ++j) {
			dp[i][j] = std::vector<bool>(n);
			dp_seen[i][j] = std::vector<bool>(n);
		}
	}

	std::cout << "Initialized dp" << std::endl;


	int r, b; std::cin >> r >> b;
	r--; b--;

	int maxDiff = 0;

	for (int i = 0; i < m; ++i) {
		int u, v; std::cin >> u >> v;
		u--; v--;
		connections[u].push_back(v);
	}

	std::cout << "Filled" << std::endl;

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