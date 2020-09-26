#include <iostream>
#include <vector>

int testcase() {
  int n; std::cin >> n;

  int toppled = 0;

  for (int i = 0; i < n; ++i) {
    int next; std::cin >> next;

    // can't topple anymore :(
    if (i > toppled) {
      continue;
    }

    if (next + i >= n) {
      toppled = n - 1;
      continue;
    }

    if (next + i - 1 >= toppled) {
      toppled = next + i - 1;
    }
  }

  return toppled + 1;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    int toppled = testcase();
    std::cout << toppled << '\n';
  }

  std::cout << std::flush;
}