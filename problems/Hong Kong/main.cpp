// In this problem we combine Delaunay and some sort of Djikstra:
// First, we precompute for every face the largest radius that
// can be reached from it. We do this iteratively by initializing
// the outside faces with infinity and the inside faces using the
// biggest radius of a circle that can fit within. Using a
// priority queue we assign each face the largest radius
// from its neighboring faces.
// Once this has been done, all that is left to do is check
// for a given balloon that the nearest vertex is not too
// close and the face that it is located on is able
// to support its radius.
//
// ! Notes:
// - Use K:FT for every coordinate / length / ...,
//   but read input using int / long
// - Use Exact constructions due to t.dual()
#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;

const K::FT max_value = K::FT(int64_t(1)<<53) * K::FT(int64_t(1)<<53);

void testcase() {
	size_t n, m; K::FT r; std::cin >> n >> m >> r;
	std::vector<K::Point_2> points(n);
	for (size_t i = 0; i < n; ++i) {
		long x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	std::priority_queue<std::pair<K::FT, Triangulation::Face_handle>> q;
	for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
		f->info() = 0;

		if (t.is_infinite(f)) {
			// Faces outside of the triangulation strat with +inf
			q.push({max_value, f});
		
		} else {
			// Faces inside start with the length between the middle (voronoi)
			// and one of the vertices (all equally distant)
            auto dist = CGAL::squared_distance(t.dual(f), f->vertex(0)->point());
            q.push({dist, f});
		}
	}

	while (!q.empty()) {
		K::FT dist = q.top().first;
		auto f = q.top().second;
		q.pop();

		if (f->info() != 0) continue;

		f->info() = dist;

		for (size_t i = 0; i < 3; ++i) {
			Triangulation::Face_handle n = f->neighbor(i);

			if (n->info() != 0) continue;

			K::FT l = t.segment(f, i).squared_length();

			q.push({std::min(dist, l), n});
		}
	}

	for (size_t i = 0; i < m; ++i) {
	 	long x, y, s; std::cin >> x >> y >> s;

	 	K::Point_2 balloon(x, y);
	 	auto tree = t.nearest_vertex(balloon);

	 	auto min_dist = K::FT(r) + K::FT(s);

	 	if (CGAL::squared_distance(balloon, tree->point()) < min_dist * min_dist) {
	 		std::cout << "n";
	 		continue;
	 	}

	 	if (t.locate(balloon)->info() < 4 * min_dist * min_dist) {
	 		std::cout << "n";
	 		continue;
	 	}

	 	std::cout << "y";
	}

	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}