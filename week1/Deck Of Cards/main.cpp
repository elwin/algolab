#include <iostream>
#include <vector>

int abs(int x) {
  if (x >= 0) {
    return x;
  }

  return -x;
}

void testcase1() {
  int n, k; std::cin >> n >> k;
  std::vector<int> cards(n);
  for (int i = 0; i < n; ++i)
  {
    std::cin >> cards[i];

    if (i > 0) {
      cards[i] += cards[i - 1];
    }
  }

  int best = -1;
  int i0 = -1; int j0 = -1;

  for (int i = 0; i < n; ++i)
  {
    for (int j = i; j < n; ++j)
    {
      int sum = cards[j];
      if (i > 0) {
        sum -= cards[i - 1];
      }

      if (best == -1 || abs(sum - k) < abs(best - k)) {
        best = sum; i0 = i; j0 = j;
      }
    }
  }

  std::cout << i0 << ' ' << j0 << '\n';
}


void testcase2() {
  int n, k; std::cin >> n >> k;
  std::vector<int> cards(n);
  for (int i = 0; i < n; ++i)
  {
    std::cin >> cards[i];

    if (i > 0) {
      cards[i] += cards[i - 1];
    }
  }

  int best = -1;
  int i = 0; int j = 0;
  int i0 = -1; int j0 = -1;

  while (i <= n && j <= n) {
    int sum = cards[j];
    if (i > 0) {
      sum -= cards[i - 1];
    }

    if (best == -1 || abs(sum - k) < abs(best - k)) {
      best = sum; i0 = i; j0 = j;
    }

    if (sum < k || i == j) {
      j++;
    } else if (sum > k) {
      i++;
    } else {
      break;
    }
  }

  std::cout << i0 << ' ' << j0 << '\n';
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase2();
  }

  std::cout << std::flush;
}