#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

bool testcase() {
	int n; std::cin >> n;
	if (n == 0) return false;

	std::vector<K::Point_2> restaurants(n);
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		restaurants[i] = K::Point_2(x, y);
	}

	int m; std::cin >> m;
	std::vector<K::Point_2> suggested(m);
	for (int i = 0; i < m; ++i) {
		int x, y; std::cin >> x >> y;
		suggested[i] = K::Point_2(x, y);
	}

	Delaunay t;
  	t.insert(restaurants.begin(), restaurants.end());

  	for (int i = 0; i < m; ++i) {
  		K::Point_2 x = suggested[i];

  		auto dist = CGAL::squared_distance(t.nearest_vertex(x)->point(), x);

  		std::cout << std::fixed << std::setprecision(0) << dist << std::endl;
  	}


	return true;
}

int main() {
	while (testcase()) {}

	return 0;
}