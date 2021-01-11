#include <iostream>
#include <math.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<std::tuple<int, int, int>> Data;

IT power(int a, size_t x) {
	IT b = 1;
	while (x > 0) {
		b *= a;
		x--;
	}
	return b;
}

void add_constraints(Program &lp, size_t d, size_t i, std::tuple<int, int, int> v, int multiplier) {
	int idx = 0;
	int x = std::get<0>(v);
	int y = std::get<1>(v);
	int z = std::get<2>(v);

	lp.set_a(idx++, i, multiplier);

	for (size_t a = 0; a <= d; ++a) {
		for (size_t b = 0; b <= d; ++b) {
			for (size_t c = 0; c <= d; ++c) {
				if (a + b + c > d || a + b + c == 0) continue;

				IT value = power(x, a) * power(y, b) * power(z, c);

				lp.set_a(idx++, i, value * multiplier);
			}
		}
	}
}


bool is_possible(size_t d, Data healthy, Data tumor) {
	size_t h = healthy.size();
	size_t t = tumor.size();

	Program lp(CGAL::SMALLER, false, 0, false, 0);

	for (size_t i = 0; i < h; ++i)
		add_constraints(lp, d, i, healthy[i], 1);

	for (size_t i = 0; i < t; ++i)
		add_constraints(lp, d, i + h, tumor[i], -1);

	lp.set_l(0, true, 1);
	Solution s = CGAL::solve_linear_program(lp, ET());

	if (!s.is_infeasible()) return true;

	lp.set_l(0, false, 1);
	lp.set_u(0, true, -1);
	s = CGAL::solve_linear_program(lp, ET());

	if (!s.is_infeasible()) return true;

	return false;
}

int testcase() {
	size_t h, t; std::cin >> h >> t;

	Data healthy(h);
	Data tumor(t);

	for (size_t i = 0; i < h; ++i) {
		int x, y, z; std::cin >> x >> y >> z;
		healthy[i] = std::make_tuple(x, y, z);
	}

	for (size_t i = 0; i < t; ++i) {
		int x, y, z; std::cin >> x >> y >> z;
		tumor[i] = std::make_tuple(x, y, z);
	}

	if (t == 0 || h == 0) {
		return 0;
	}

	for (size_t d = 1; d <= 30; ++d) {
		if (is_possible(d, healthy, tumor)) {
			return d;
		}
	}

	return -1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t n; std::cin >> n;
	for (size_t i = 0; i < n; ++i) {
		int out = testcase();
		if (out >= 0) {
			std::cout << out << std::endl;
		} else {
			std::cout << "Impossible!" << std::endl;
		}
	}
}