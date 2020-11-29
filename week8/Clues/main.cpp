#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;

void testcase(bool debug) {
	int n, m, r; std::cin >> n >> m >> r;
	if (debug) std::cout << n << " " << m << " " << r << std::endl;

  	typedef std::pair<K::Point_2, int> IPoint;
	std::vector<IPoint> pts;
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
	    if (debug) std::cout << x << " " << y << std::endl;

	    pts.emplace_back(K::Point_2(x, y), i);
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

    	int a = e->first->vertex((e->second + 1) % 3)->info();
    	int b = e->first->vertex((e->second + 2) % 3)->info();

    	boost::add_edge(a, b, G);
	}

	for (int i = 0; i < m; ++i) {
		int ax, ay, bx, by;
		std::cin >> ax >> ay >> bx >> by;
	    if (debug) std::cout << ax << " " << ay << " " << bx << " " << by << std::endl;

		bool possible = true;
		for (auto vd : boost::make_iterator_range(vertices(G))) {
			if (degree(vd, G) > 2) {
				possible = false;
				break;
			}
		}

		std::cout << (possible ? 'y' : 'n');
	}

	std::cout << std::endl;
}

int main() {
	int n; std::cin >> n;
	for (int i = 0; i < n; ++i) {
		testcase(false);
	}
	
	return 0;
}