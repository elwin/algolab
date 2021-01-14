// Mildly inspired by master solution

#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef std::result_of<K::Intersect_2(K::Ray_2, K::Segment_2)>::type IResult;

double floor_to_double(const K::FT& x) {
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a--;
	while (a + 1 <= x) a++;

	return a;
}

void shorten_vector(K::Segment_2 &segment, IResult isect) {
	if (K::Point_2 *p = boost::get<K::Point_2 >(&*isect)) {
		segment = K::Segment_2(segment.source(), *p);
		return;
	}

	// Obstacle is colinear with ray.
	// Take the point which is closer to the start
	if (K::Segment_2 *s = boost::get<K::Segment_2>(&*isect)) {
		if (CGAL::collinear_are_ordered_along_line(segment.source(), s->source(), s->target())) {
			segment = K::Segment_2(segment.source(), s->source());
		} else {
			segment = K::Segment_2(segment.source(), s->target());
		}

		return;
	}

	throw std::runtime_error("what the?!");
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);

	while (true) {
		int n; std::cin >> n;
		if (n == 0) {
			return 0;
		}

		long x, y, a, b; std::cin >> x >> y >> a >> b;
		K::Ray_2 phil(K::Point_2(x, y), K::Point_2(a, b));

		std::vector<K::Segment_2> segments(n);
		for (int i = 0; i < n; ++i) {
			long r, s, t, u; std::cin >> r >> s >> t >> u;
			K::Point_2 left(r, s), right(t, u);
			segments[i] = K::Segment_2(left, right);
		}

		std::random_shuffle(segments.begin(), segments.end());

		K::Segment_2 phil_segment(phil.source(), phil.point(1));
		bool clipped = false;

		for (auto segment : segments) {
			if (!clipped) {
				if (CGAL::do_intersect(segment, phil)) {
					shorten_vector(phil_segment, CGAL::intersection(segment, phil));					
					clipped = true;
				}

			} else {
				if (CGAL::do_intersect(segment, phil_segment)) {
					shorten_vector(phil_segment, CGAL::intersection(segment, phil));
				}
			}
		}

		if (clipped) {
			std::cout << floor_to_double(phil_segment.target().x()) << " "
					  << floor_to_double(phil_segment.target().y()) << std::endl;
		} else {
			std::cout << "no" << std::endl;
		}
	}
}