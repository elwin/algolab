#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int testcase() {
	// create an LP with Ax <= b, lower bound 0 and no upper bounds
	Program lp (CGAL::SMALLER, false, 0, false, 0);

	// set the coefficients of A and b
	const int R = 0;
	const int X = 1;
	const int Y = 2;
	
	lp.set_a(R, 0, 1); lp.set_a(X, 0,  1); lp.set_a(Y, 0,  0); lp.set_b(0, 0);  //  r - x0 <= 0
	lp.set_a(R, 1, 1); lp.set_a(X, 1,  0); lp.set_a(Y, 1,  1); lp.set_b(1, 0);  //  r - y0 <= 0
	lp.set_a(R, 2, 5); lp.set_a(X, 2, -3); lp.set_a(Y, 2, -4); lp.set_b(2, 12); //  5r - 3x0 - 4y0 <= 7

	lp.set_l(R, true, 0);

	// objective function
	lp.set_c(R, -1); // -R

	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_linear_program(lp, ET());

	if (!s.is_optimal()) return -1;

	Solution::Variable_value_iterator opt = s.variable_values_begin();
	CGAL::Quotient<ET> res = *opt;
	// std::cout << res.numerator() << std::endl;

	// std::cout << s << std::endl;

	return res.numerator().to_double();
 	// return res.numerator();
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t = 1;
	// size_t t; std::cin >> t;
	while (t--) {
		int out = testcase();
		if (out < 0) std::cout << "no" << std::endl;
		else std::cout << out << std::endl;
	}
}