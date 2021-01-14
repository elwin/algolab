#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

bool testcase() {
	size_t n, m; std::cin >> n >> m;
	if (n == 0 && m == 0) return false;

	std::vector<std::pair<int, int>> nutritients(n);
	for (size_t i = 0; i < n; ++i) {
		std::cin >> nutritients[i].first >> nutritients[i].second;
	}

	std::vector<std::vector<int>> foods(m);
	std::vector<int> foods_price(m);
	for (size_t i = 0; i < m; ++i) {
		std::cin >> foods_price[i];

		foods[i] = std::vector<int>(n);
		for (size_t j = 0; j < n; ++j) {
			std::cin >> foods[i][j];
		}
	}

	Program lp(CGAL::SMALLER, true, 0, false, 0);
	
	for (size_t i = 0; i < m; ++i) {
		lp.set_c(i, foods_price[i]);
	}

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			lp.set_a(j, i, foods[j][i]);
			lp.set_a(j, i + n, -foods[j][i]);
		}
		lp.set_b(i, nutritients[i].second);
		lp.set_b(i + n, -nutritients[i].first);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());

	if (s.is_infeasible()) {
		std::cout << "No such diet." << std::endl;
	} else {
		std::cout << std::floor(CGAL::to_double(s.objective_value())) << std::endl;
	}

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (testcase());
}