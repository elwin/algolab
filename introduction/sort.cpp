#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> x(n);

  for (int i = 0; i < n; ++i) {
    std::cin >> x[i];
  }

  int direction; std::cin >> direction;
  if (direction == 0) {
    std::sort(x.begin(), x.end());
  } else {
    std::sort(x.begin(), x.end(), std::greater);
  }

  for (int i : x) {
    std::cout << i << ' ';
  }

  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }
}