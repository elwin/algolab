#include <iostream>
#include <set>
#include <vector>
#include <stack>

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;

  std::vector<std::set<int>> connections(n);
  std::vector<bool> seen(n, false);
  std::vector<int> start(n, -1);
  std::vector<int> end(n, -1);

  for (int i = 0; i < m; ++i) {
    int a, b; std::cin >> a >> b;
    connections[a].insert(b);
    connections[b].insert(a);
  }

  std::stack<int> next;
  next.push(v);

  int current = 0;

  for (; next.size() > 0;) {
    int c = next.top(); next.pop();
    
    // on the way back
    if (seen[c]) {
      if (end[c] == -1) {
        end[c] = current++;
      }
      continue;
    }

    start[c] = current++;
    seen[c] = true;
    next.push(c);

    for (auto it = connections[c].rbegin(); it != connections[c].rend(); ++it) {
      int n = *it;

      if (seen[n]) {
        continue;
      }

      next.push(n);
    }
  }

  for (int i : start) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  for (int i : end) {
    std::cout << i << ' ';
  }
  std::cout << '\n' << std::flush;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}