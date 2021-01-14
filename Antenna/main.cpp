#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K>                              Traits;
typedef CGAL::Min_circle_2<Traits>                                  Min_circle;


double ceil_to_double(const K::FT& x) {
	double a = std::ceil(CGAL::to_double(x));
	while (a > x) a--;
	while (a < x) a++;

	return a;
}

double testcase(size_t n) {
	std::vector<K::Point_2> points(n);
	for (size_t i = 0; i < n; ++i) {
		long x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	Min_circle circle(points.begin(), points.end(), true);

	auto radius = CGAL::sqrt(circle.circle().squared_radius());

	return ceil_to_double(radius);
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(0);

	while (true) {
		size_t n; std::cin >> n;
		if (n == 0) return 0;

		std::cout << testcase(n) << std::endl;
	}
}