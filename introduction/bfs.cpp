#include <iostream>
#include <set>
#include <vector>
#include <queue>

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;

  std::vector<std::set<int>> connections(n);
  std::vector<int> distance(n, -1);
  std::queue<int> next;

  for (int i = 0; i < m; ++i) {
    int a, b; std::cin >> a >> b;
    connections[a].insert(b);
    connections[b].insert(a);
  }

  int cur_distance = 0;
  next.push(v);

  while (next.size() > 0) {

    std::queue<int> temp;

    while (next.size() > 0) {
      int c = next.front(); next.pop();
      if (distance[c] != -1) {
        continue;
      }

      distance[c] = cur_distance;
      for (int neighbour : connections[c]) {
        temp.push(neighbour);
      }
    }

    cur_distance++;
    next = temp;
  }

  for (int d : distance) {
    std::cout << d << ' ';
  }

  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}