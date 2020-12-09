#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>

struct Point {
	int x, y;    
};

struct Status {
	bool visited;
	bool first;
	bool visited_component;
	int component;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Status, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

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
		v->info() = Status{false, false, false, -1};
	}

	bool possible = true;
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		auto a = e->first->vertex((e->second + 1) % 3);
		auto b = e->first->vertex((e->second + 2) % 3);

		if (!a->info().visited && b->info().visited) {
			a->info().visited = true; a->info().first = true;
			b->info().visited = true; b->info().first = false;
		} else if (a->info().first == b->info().first) {
			if (t.segment(e).squared_length() > r * r) {
				continue;
			}

			possible = false;
			break;
		} else {
			if (!a->info().visited) {
				auto temp = a;
				a = b;
				b = temp;
			}

			b->info().first = !a->info().first;
			b->info().visited = true;
		}
	}

	if (!possible) {
		for (int i = 0; i < m; ++i) {
			std::cout << 'x';
		}
		std::cout << std::endl;

		return;
	}

	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		if (!v->info().visited) {
			throw "unmarked vertex!";
		}

		if (v->info().first) {
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

	// Figure out components
	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		std::stack<Vertex_handle> stack;
		stack.push(v);

		int current_component = 0;
		while (!stack.empty()) {
			Vertex_handle next = stack.top(); stack.pop();
			if (next->info().visited_component) {
				continue;
			}

			next->info().component = current_component;
			next->info().visited_component = true;
		}

		current_component++;
	}


	for (int i = 0; i < m; ++i) {
		Point a = clues_a[i];
		Point b = clues_b[i];

		Vertex_handle a_handle = t.nearest_vertex(K::Point_2(a.x, a.y));
		Vertex_handle b_handle = t.nearest_vertex(K::Point_2(b.x, b.y));

		if (a_handle->info().component == b_handle->info().component) {
			std::cout << 'y';
		} else {
			std::cout << 'n';
		}
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