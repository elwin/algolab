#include <string>
#include <iostream>

std::string reverse(std::string x) {
  char temp;
  int n = x.length();

  for (int i = 0; i < n / 2; ++i) {
    temp = x[i];
    x[i] = x[n - i - 1];
    x[n - i - 1] = temp;
  }

  return x;
}


void testcase() {
  std::string a, b;

  std::cin >> a >> b;

  std::cout << a.length() << ' ' << b.length() << '\n';
  std::cout << a << b << '\n';

  a = reverse(a);
  b = reverse(b);

  char temp = a[0];
  a[0] = b[0];
  b[0] = temp;

  std::cout << a << ' ' << b << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}