// Linear optimization problem.
// Some notes:
//  1. A contour does not need to be crossed when the warehouse and
//     stadium are both either inside the contour or outside
//  2. To enforce equality for beer consumption we can use both a
//     lower and upper bound (<= && >=)

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

struct warehouse {
	long x, y;
	size_t supply, alcohol;
};

struct stadium {
	long x, y;
	size_t demand, alcohol;
};

struct contour {
	long x, y;
	long r;
};

int idx(size_t w, size_t s, size_t n, size_t m) {
	return w + s * n;
}

long distance(long x0, long y0, long x1, long y1) {
	return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
}

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


void testcase() {
	size_t n, m, c; std::cin >> n >> m >> c;

	std::vector<warehouse> warehouses(n);
	for (size_t i = 0; i < n; ++i) {
		long x, y; size_t supply, alcohol;
		std::cin >> x >> y >> supply >> alcohol;
		warehouses[i] = {x, y, supply, alcohol};
	}

	std::vector<stadium> stadiums(m);
	for (size_t i = 0; i < m; ++i) {
		long x, y; size_t demand, alcohol;
		std::cin >> x >> y >> demand >> alcohol;
		stadiums[i] = {x, y, demand, alcohol};
	}

	std::vector<std::vector<int>> revenues(n);
	for (size_t i = 0; i < n; ++i) {
		revenues[i] = std::vector<int>(m);
		for (size_t j = 0; j < m; ++j) {
			std::cin >> revenues[i][j];
			revenues[i][j] *= 100;
		}
	}

	std::vector<contour> contours(0);
	for (size_t i = 0; i < c; ++i) {
		long x, y; long r; std::cin >> x >> y >> r;

		bool possible = false;
		for (size_t j = 0; j < n && !possible; ++j) {
			if (distance(x, y, warehouses[j].x, warehouses[j].y) <= r * r) possible = true;
		}

		for (size_t j = 0; j < m && !possible; ++j) {
			if (distance(x, y, stadiums[j].x, stadiums[j].y) <= r * r) possible = true;
		}

		if (possible) contours.push_back({x, y, r});
	}

	// Adjust revenue due to contours
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			warehouse w = warehouses[i];
			stadium s = stadiums[j];

			for (size_t k = 0; k < contours.size(); ++k) {
				contour c = contours[k];

				// Add penalty if the warehouse are not in the same contour
				if ((distance(c.x, c.y, w.x, w.y) < c.r * c.r) != (distance(c.x, c.y, s.x, s.y) < c.r * c.r)) {
					revenues[i][j] -= 1;
				}
			}
		}
	}

	Program lp (CGAL::SMALLER, true, 0, false, 0);

	size_t ineq = 0;

	// Ensure supply
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			lp.set_a(idx(i, j, n, m), ineq, 1);
		}

		lp.set_b(ineq, warehouses[i].supply);

		ineq++;
	}

	// Ensure demand below
	for (size_t j = 0; j < m; ++j) {
		for (size_t i = 0; i < n; ++i) {
			lp.set_a(idx(i, j, n, m), ineq, 1);
		}

		lp.set_b(ineq, stadiums[j].demand);

		ineq++;
	}

	// Ensure demand above
	for (size_t j = 0; j < m; ++j) {
		for (size_t i = 0; i < n; ++i) {
			lp.set_a(idx(i, j, n, m), ineq, -1);
		}

		lp.set_b(ineq, -stadiums[j].demand);

		ineq++;
	}

	// Ensure alcohol
	for (size_t j = 0; j < m; ++j) {
		for (size_t i = 0; i < n; ++i) {
			lp.set_a(idx(i, j, n, m), ineq, warehouses[i].alcohol);
		}

		lp.set_b(ineq, stadiums[j].alcohol * 100);

		ineq++;
	}

	// Set optimization function using revenue multiplier (negative for maximization)
	for (size_t j = 0; j < m; ++j) {
		for (size_t i = 0; i < n; ++i) {
			lp.set_c(idx(i, j, n, m), -revenues[i][j]);
		}
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible()) std::cout << "RIOT!" << std::endl;

	else std::cout << floor_to_double(-1 * s.objective_value() / 100) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(0);
	size_t t; std::cin >> t;
	while (t--) testcase();
	return 0;
}