// Let's define the equations of our lines as follows:
// - Sewage: x + by + u = 0   <=> x = -by - u
// - Water:  -bx + y + v = 0  <=> y =  bx - v
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const size_t b = 0;
const size_t u = 1;
const size_t v = 2;
const size_t d = 3;

double floor_to_long(const CGAL::Quotient<ET> &x){
	double a = std::ceil(CGAL::to_double(x)); 
	while (a < x) a += 1; 
	while (a-1 >= x) a -= 1;
	return a;
}
	
void set_distance_boundary(int x, int y, Program &lp, size_t &idx) {
	// y0 - y <= d
	// <=> -b * x0 + v - d <= -y0
	lp.set_a(b, idx, -x);
	lp.set_a(v, idx,  1);
	lp.set_a(d, idx, -1);
	lp.set_b(idx++, -y);
	
	// y - y0 <= d
	// <=> b * x0 - v - d <= y0
	lp.set_a(b, idx,  x);
	lp.set_a(v, idx, -1);
	lp.set_a(d, idx, -1);
	lp.set_b(idx++, y);
}


void testcase() {
	size_t n, m; long s; std::cin >> n >> m >> s;
	
	Program lp (CGAL::SMALLER, false, 0, false, 0);
	
	long sum_x_nobles = 0;
	long sum_y_nobles = 0;
	long sum_x_common = 0;
	long sum_y_common = 0;
	size_t idx = 0;
	
	for (size_t i = 0; i < n; i++) {
		int x, y; std::cin >> x >> y;
		set_distance_boundary(x, y, lp, idx);
		
		// Make sure nobles are on the left side of the sewage
		// x0 - x <= 0
		// <=> b * y0 + u <= -x0
		lp.set_a(b, idx, y);
		lp.set_a(u, idx, 1);
		lp.set_b(idx++, -x);
		
		sum_x_nobles += x;
		sum_y_nobles += y;
	}
	
	for (size_t i = 0; i < m; i++) {
		int x, y; std::cin >> x >> y;
		set_distance_boundary(x, y, lp, idx);
		
		// Make sure commons are on the right side of the sewage
		// x0 - x >= 0
		// <=> -b * y0 - u <= x0
		lp.set_a(b, idx, -y);
		lp.set_a(u, idx, -1);
		lp.set_b(idx++, x);
		
		sum_x_common += x;
		sum_y_common += y;
	}
	
	lp.set_c(d, 1);
	lp.set_l(d, true, 0);
	
	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()) {
		std::cout << "Yuck!" << std::endl;
		return;
	}
	
	// Set cost constraint for water sewage pipes
	// For nobles, we add x - x0
	// <=> -b * y0 - u - x0
	// For commons, we add x1 - x
	// <=> x1 + b * y1 + u
	// Adding all together, we get
	// b * (y1 - y0) + (m - n) * u + (x1 - x0)
	// This cost must not exceed s, thus we have
	// b * (y1 - y0) + (m - n) * u <= s + x0 - x1
	// Where {x, y}0 stand for the sum of nobles and
	// {x, y}1 for the sum of commons for x and y.
	if (s != -1) {
		lp.set_a(b, idx, sum_y_common - sum_y_nobles);
		lp.set_a(u, idx, m - n);
		lp.set_b(idx++, s + sum_x_nobles - sum_x_common);

		sol = CGAL::solve_linear_program(lp, ET());
	}
	
	if (sol.is_infeasible()) {
		std::cout << "Bankrupt!" << std::endl;
		return;
	}
	
	std::cout << floor_to_long(sol.objective_value()) << std::endl; 
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(0) << std::fixed;
	size_t t; std::cin >> t;
	while (t--) testcase();
}