// In the 2D case the variables we are optimizing over are x/y and r, the center
// and the radius of our ball, with maximal r (which correponds to minimizinig -r).
// Our constaints can be given by making sure that r is larger than the smallest
// disitance between the center and any line, which corresponds to the following:
//
//  (ax + by + c) / sqrt(a^2 + b^2) >= r
// <=>
//  sqrt(a^2 + b^2) * r - ax - by <= c
//
// as given by this [1] formula.
// Furthermore, we add a lower bound of 0 to r to make sure it's positive.
//
// The d-dimension case can be solved accordingly.
//
// [1] https://math.stackexchange.com/a/275533

#include <iostream>
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

void testcase(size_t n, size_t d) {
	// create an LP with Ax <= b, lower bound 0 and no upper bounds
	Program lp (CGAL::SMALLER, false, 0, false, 0);

	const int R = 0;

	for (size_t i = 0; i < n; ++i) {
		double sum = 0;
		for (size_t j = 0; j < d; ++j) {
			int next; std::cin >> next;
			sum += next * next;
			lp.set_a(j + 1, i, -next);
		}

		int b; std::cin >> b;

		lp.set_a(0, i, std::sqrt(sum));
		lp.set_b(i, b);
	}

	lp.set_l(R, true, 0);

	// objective function
	lp.set_c(R, -1);

	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_linear_program(lp, ET());

	if (s.is_unbounded()) {
		std::cout << "inf" << std::endl; return;
	} else if (!s.is_optimal()) {
		std::cout << "none" << std::endl; return;
	}

	Solution::Variable_value_iterator opt = s.variable_values_begin();
	CGAL::Quotient<ET> res = *opt;
	std::cout << res.numerator() / res.denominator() << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t n, d; std::cin >> n;
	while (n > 0) {
		std::cin >> d;
		testcase(n, d);
		std::cin >> n;
	}
}