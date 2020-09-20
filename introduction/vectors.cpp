#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> x;

  for (int i = 0; i < n; ++i) {
    int next; std::cin >> next;
    x.push_back(next);
  }

  int d; std::cin >> d;
  x.erase(x.begin() + d);
  
  int a, b; std::cin >> a >> b;
  x.erase(x.begin() + a, x.begin() + b + 1);

  if (x.size() == 0) {
    std::cout << "Empty\n";

    return;
  }

  for (int i : x) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}