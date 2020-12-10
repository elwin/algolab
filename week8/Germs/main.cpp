#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle								   Vertex_handle;

int duration(double x) {
	return std::ceil(std::sqrt(x - 0.5));
}

bool testcase() {
	int n; std::cin >> n;
	if (n == 0) return false;

	int left, bottom, right, top; std::cin >> left >> bottom >> right >> top;

	std::vector<K::Point_2> points(n);
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	Delaunay t;
	t.insert(points.begin(), points.end());

	std::vector<double> distances;
	for (auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		Vertex_handle next = CGAL::nearest_neighbor(t, v);
		
		double min_radius = std::numeric_limits<double>::max();
		if (next != NULL) {
			double distance = CGAL::sqrt(CGAL::squared_distance(next->point(), v->point()));
			min_radius = std::min(min_radius, distance / 2);
		}

		min_radius = std::min(min_radius, double(v->point().x()) - left);
		min_radius = std::min(min_radius, double(right - v->point().x()));
		min_radius = std::min(min_radius, double(v->point().y()) - bottom);
		min_radius = std::min(min_radius, double(top - v->point().y()));

		distances.push_back(min_radius);
	}

	std::sort(distances.begin(), distances.end());

	// for (int i = 0; i < distances.size(); ++i) {
	// 	std::cout << distances[i] << " ";
	// }
	// std::cout << std::endl;

	int median = distances.size() / 2;
	std::cout << duration(distances[0]) << " "
		<< duration(distances[median]) << " "
		<< duration(distances[distances.size() - 1]) << std::endl;

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (testcase());
}