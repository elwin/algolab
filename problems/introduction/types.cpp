#include <iostream>
#include <tuple>
#include <iomanip>

void testcase() {
  int a;         std::cin >> a;
  long b;        std::cin >> b;
  std::string c; std::cin >> c;
  double d;      std::cin >> d;

  std::cout << std::fixed << std::setprecision(2);
  std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;

  for (int i = 0; i < t; ++i) {
    testcase();
  }

  std::cout << std::flush;
}