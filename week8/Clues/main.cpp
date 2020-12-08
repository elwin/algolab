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
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Edge_iterator Edge_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<graph>::vertex_iterator vertex_it;

struct Point {
	int x, y;    
};

int squared_distance(Point a, Point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void testcase(bool debug) {
	int n, m, r; std::cin >> n >> m >> r;
	if (debug) std::cout << n << " " << m << " " << r << std::endl;

	std::vector<K::Point_2> stations(n);
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		if (debug) std::cout << x << " " << y << std::endl;

		stations[i] = K::Point_2(x, y);
	}

	std::vector<Point> clues_a(m);
	std::vector<Point> clues_b(m);
	for (int i = 0; i < m; ++i) {
		int ax, ay; std::cin >> ax >> ay;
		int bx, by; std::cin >> bx >> by;
		clues_a[i] = Point{ax, ay};
		clues_b[i] = Point{bx, by};
	}

	Triangulation t, t0, t1;
	t.insert(stations.begin(), stations.end());
	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		v->info() = -1;
	}

	bool possible = true;
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		auto a = e->first->vertex((e->second + 1) % 3);
		auto b = e->first->vertex((e->second + 2) % 3);

		if (a->info() == -1 && b->info() == -1) {
			a->info() = 0;
			b->info() = 1;
		} else if (a->info() == b->info()) {
			if (t.segment(e).squared_length() > r * r) {
				continue;
			}

			possible = false;
			break;
		} else {
			if (a->info() == -1) {
				auto temp = a;
				a = b;
				b = temp;
			}

			b->info() = a->info() == 0 ? 1 : 0;
		}
	}

	if (!possible) {
		for (int i = 0; i < m; ++i) {
			std::cout << 'n';
		}
		std::cout << std::endl;

		return;
	}

	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		std::cout << v->info() << std::endl;

		if (v->info() == -1) {
			throw "unmarked vertex!";
		}

		if (v->info() == 0) {
			t0.insert(v->point());
		} else {
			t1.insert(v->point());
		}
	}

	for (Edge_iterator e = t0.finite_edges_begin(); e != t0.finite_edges_end(); e++) {
		if (t0.segment(e).squared_length() <= r * r) {
			possible = false;
			break;
		}
	}

	for (Edge_iterator e = t1.finite_edges_begin(); e != t1.finite_edges_end(); e++) {
		if (t1.segment(e).squared_length() <= r * r) {
			possible = false;
			break;
		}
	}


	if (!possible) {
		for (int i = 0; i < m; ++i) {
			std::cout << 'n';
		}
		std::cout << std::endl;

		return;
	}

	for (int i = 0; i < m; ++i) {
		Point a = clues_a[i];
		Point b = clues_b[i];
	}







	// for (int i = 0; i < m; ++i) {
	// 	CluePair pair = clues[i];
	// 	Point first = pair.first;
	// 	Point second = pair.second;

	// 	// auto next = IPoint(K::Point_2(first.x, first.y), n+i);
	// 	auto handleA = t.insert(K::Point_2(first.x, first.y));
	// 	handleA->info() = n + 1;

	// 	auto handleB = t.insert(K::Point_2(second.x, second.y));
	// 	handleB->info() = n + 2;

	// 	graph G(0);

	// 	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
	// 		auto seg = t.segment(e);

	// 		// Skip lines that can't connect
	// 		if (seg.squared_length() > r * r) {
	// 			continue;
	// 		}

	// 		int a = e->first->vertex((e->second + 1) % 3)->info();
	// 		int b = e->first->vertex((e->second + 2) % 3)->info();

	// 		boost::add_edge(a, b, G);
	// 	}

	// 	bool possible = true;
	// 	for (auto vd : boost::make_iterator_range(vertices(G))) {
	// 		if (degree(vd, G) > 2) {
	// 			possible = false;
	// 			break;
	// 		}
	// 	}

	// 	std::cout << (possible ? 'y' : 'n');

	// 	t.remove(handleA);
	// 	t.remove(handleB);
	// }


	// for (int i = 0; i < m; ++i) {
	//     int ax, ay, bx, by;
	//     std::cin >> ax >> ay >> bx >> by;
	//     if (debug) std::cout << ax << " " << ay << " " << bx << " " << by << std::endl;

	//     Point a = Point{ax, ay};
	//     Point b = Point{bx, by};

	//     for (auto vd : boost::make_iterator_range(vertices(G))) {
	//         Point x = Point{myVert[idx].x, myVert[idx].y};
	//         int distance = squared_distance(x, a);
	//         if (distance > r * r) continue;



	//         std::cout << myVert[idx].x << std::endl;
	//     }

	//     bool possible = true;
	//     for (auto vd : boost::make_iterator_range(vertices(G))) {
	//         if (degree(vd, G) > 2) {
	//             possible = false;
	//             break;
	//         }
	//     }

	//     std::cout << (possible ? 'y' : 'n');
	// }

	// std::cout << std::endl;
}

int main() {
	int n; std::cin >> n;
	for (int i = 0; i < n; ++i) {
		testcase(false);
	}
	
	return 0;
}