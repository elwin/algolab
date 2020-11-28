#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <boost/graph/adjacency_list.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;

void testcase() {
	int n, m, r; std::cin >> n >> m >> r;

	std::vector<K::Point_2> pts;
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}

	Triangulation t;
	t.insert(pts.begin(), pts.end());

	graph G(0);

  	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	auto seg = t.segment(e);

    	// Skip lines that can't connect
    	if (seg.squared_length() > r * r) {
    		continue;
    	}

    	std::cout << seg << std::endl;
	}

	for (int i = 0; i < m; ++i) {
		int ax, ay, bx, by;
		std::cin >> ax >> ay >> bx >> by;
	}
}

int main() {
	int n; std::cin >> n;
	for (int i = 0; i < n; ++i) {
		testcase();
	}
	
	return 0;
}