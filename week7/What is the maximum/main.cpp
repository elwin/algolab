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
	int p, a, b; std::cin >> p >> a >> b;
	if (p == 0) return false;

	const int X = 0;
	const int Y = 1;
	const int Z = 2;

	if (p == 1) {
		Program lp(CGAL::SMALLER, false, 0, false, 0);
		lp.set_c(X, a);		
		lp.set_c(Y, -b);

		lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
		lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
		lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
		lp.set_l(X, true, 0); lp.set_l(Y, true, 0);

		Solution s = CGAL::solve_linear_program(lp, ET());

		if (s.is_infeasible()) {
			std::cout << "no" << std::endl;
		} else if (s.is_unbounded()) {
			std::cout << "unbounded" << std::endl;
		} else {
			std::cout << std::floor(-CGAL::to_double(s.objective_value())) << std::endl;
		}
	} else {
		Program lp(CGAL::LARGER, false, 0, false, 0);
		lp.set_c(X, a);
		lp.set_c(Y, b);
		lp.set_c(Z, 1);

		lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
		lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -(a * b));
		lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
		lp.set_u(X, true, 0); lp.set_u(Y, true, 0); lp.set_u(Z, true, 0);

		Solution s = CGAL::solve_linear_program(lp, ET());

		if (s.is_infeasible()) {
			std::cout << "no" << std::endl;
		} else if (s.is_unbounded()) {
			std::cout << "unbounded" << std::endl;
		} else {
			std::cout << std::ceil(CGAL::to_double(s.objective_value())) << std::endl;
		}	
	}

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (testcase());
}