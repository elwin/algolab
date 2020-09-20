#include <iostream>
#include <set>

void testcase() {
  int n; std::cin >> n;
  std::set<int> x;

  for (int i = 0; i < n; ++i) {
    int temp; std::cin >> temp;
    x.insert(temp);
  }

  int direction; std::cin >> direction;
  if (direction == 0) {
    for (auto it = x.begin(); it != x.end(); ++it) {
      std::cout << *it << ' ';
    }
  } else {
    for (auto it = x.rbegin(); it != x.rend(); ++it) {
      std::cout << *it << ' ';
    }
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