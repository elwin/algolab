#include <iostream>
#include <set>

void testcase() {
  std::set<int> s;

  int n; std::cin >> n;
  for (int i = 0; i < n; ++i) {
    int o, b; std::cin >> o >> b;
    if (o == 0) {
      s.insert(b);
    } else {
      auto it = s.find(b);
      if (it != s.end()) {
        s.erase(s.find(b));
      }
    }
  }

  if (s.size() == 0) {
    std::cout << "Empty\n";
    return;
  }

  for (int i : s) {
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