#include <iostream>
#include <vector>

bool evenness(std::vector<bool> even, int i, int j) {
  if (i == 0) {
    return even[j];
  }

  return even[i - 1] == even[j];
}

int testcase1() {
  int n; std::cin >> n;

  // even[i] is true iff all entries up and inclusive i sum up to an even number
  std::vector<bool> even(n);

  for (int i = 0; i < n; ++i) {
    int next; std::cin >> next;
    if (i == 0) {
      even[0] = next % 2 == 0;
      continue;
    }

    // next is even only if the previous one and the current one
    // have the same parity
    even[i] = even[i - 1] == (next % 2 == 0);
  }

  int evens = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      if (evenness(even, i, j)) {
        evens++;
      }
    }
  }

  return evens;
}

int testcase2() {
  int n; std::cin >> n;

  // even[i] is true iff all entries up and inclusive i sum up to an even number
  std::vector<bool> even(n);

  for (int i = 0; i < n; ++i) {
    int next; std::cin >> next;
    if (i == 0) {
      even[0] = next % 2 == 0;
      continue;
    }

    // next is even only if the previous one and the current one
    // have the same parity
    even[i] = even[i - 1] == (next % 2 == 0);
  }

  int evens = 0, odds = 0;
  for (int i = 0; i < n; ++i) {
    if (even[i]) {
      evens++;
    } else {
      odds++;
    }
  }

  return evens + (evens * (evens - 1) / 2) + (odds * (odds - 1) / 2);
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    int result = testcase2();
    std::cout << result << '\n';
  }

  std::cout << std::flush;
}