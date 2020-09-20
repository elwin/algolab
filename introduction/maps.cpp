#include <iostream>
#include <vector>
#include <set>
#include <map>

void testcase() {
  std::map<std::string, std::multiset<int>> m;

  int q; std::cin >> q;
  for (int i = 0; i < q; ++i) {
    int a; std::string b; std::cin >> a >> b;
    if (a == 0) {
      m.erase(b);
    } else {
      auto it = m.find(b);
      if (it == m.end()) {
        std::multiset<int> x;
        m[b] = x;
      }

      m[b].insert(a);
    }
  }

  std::string key; std::cin >> key;
  auto it = m.find(key);
  if (it == m.end()) {
    std::cout << "Empty\n";
    return;
  }

  for (int i : it->second) {
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