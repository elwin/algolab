#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main() {
	std::ios_base::sync_with_stdio(false);

	while (true) {
		int n; std::cin >> n;
		if (n == 0) {
			break;
		}

		long x, y, a, b; std::cin >> x >> y >> a >> b;
		K::Point_2 phil_start(x, y), phil_end(a, b);
		K::Ray_2 phil(phil_start, phil_end);

		bool hit = false;
		for (int i = 0; i < n; ++i) {
			long r, s, t, u; std::cin >> r >> s >> t >> u;
			K::Point_2 left(r, s), right(t, u);
			K::Segment_2 obstacle(left, right);

			if (!hit && CGAL::do_intersect(phil, obstacle)) {
				hit = true;
			}
		}
		std::cout << (hit ? "yes" : "no") << std::endl;
	}
}