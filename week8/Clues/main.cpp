#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

void ass(bool assertion, std::string reason) {
	if (!assertion) {
		std::cout << "Something went wrong: " << reason << std::endl;
		exit(-1);
	}
}

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
typedef Triangulation::Edge_circulator Edge_circulator;
typedef Triangulation::Vertex_circulator Vertex_circulator;

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
		v->info() = Status{false, false, false, 0};
	}

	// Figure out components
	int current_component = 1;
	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		if (v->info().visited_component) continue;

		std::stack<Vertex_handle> stack;
		stack.push(v);

		while (!stack.empty()) {
			Vertex_handle next = stack.top(); stack.pop();
			if (next->info().visited_component) continue;

			next->info().component = current_component;
			next->info().visited_component = true;

			Vertex_circulator c = t.incident_vertices(next);
			do {
				if (t.is_infinite(c) || CGAL::squared_distance(c->point(), next->point()) > r * r) { continue; }
				stack.push(c);
			} while (++c != t.incident_vertices(next));
		}

		current_component++;
	}

	bool possible = true;
	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		std::stack<Vertex_handle> stack1;
		std::stack<Vertex_handle> stack2;

		if (v->info().visited) continue;
		stack1.push(v);

		while (!stack1.empty() || !stack2.empty()) {
			while (!stack1.empty()) {
				Vertex_handle next = stack1.top(); stack1.pop();
				if (next->info().visited) {
					if (next->info().first != true)
						possible = false;

					continue;
				}

				next->info().visited = true;
				next->info().first = true;

				Vertex_circulator c = t.incident_vertices(next);
				do {
					if (t.is_infinite(c) || CGAL::squared_distance(c->point(), next->point()) > r * r) { continue; }
					stack2.push(c);
				} while (++c != t.incident_vertices(next));
			}

			while (!stack2.empty()) {
				Vertex_handle next = stack2.top(); stack2.pop();
				if (next->info().visited) {
					if (next->info().first != false)
						possible = false;

					continue;
				}
				next->info().visited = true;
				next->info().first = false;

				Vertex_circulator c = t.incident_vertices(next);
				do {
					if (t.is_infinite(c) || CGAL::squared_distance(c->point(), next->point()) > r * r) { continue; }
					stack1.push(c);
				} while (++c != t.incident_vertices(next));
			}
		}
	}

	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		ass(v->info().visited == true, "not all vertices visited");
		ass(v->info().visited_component == true, "not all vertices visited");
		ass(v->info().component > 0, "not all components larger than 0");
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

	if (debug) {
		int count1 = 0; int count2 = 0;
		for (Edge_iterator e = t0.finite_edges_begin(); e != t0.finite_edges_end(); e++) {
			count1++;
		}
		for (Edge_iterator e = t1.finite_edges_begin(); e != t1.finite_edges_end(); e++) {
			count2++;
		}
		std::cout << "assignment count: " << count1 << " / " << count2 << std::endl;
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

		Vertex_handle a_handle = t.nearest_vertex(K::Point_2(a.x, a.y));
		Vertex_handle b_handle = t.nearest_vertex(K::Point_2(b.x, b.y));

		if (CGAL::squared_distance(K::Point_2(a.x, a.y), K::Point_2(b.x, b.y)) <= r * r || (
			CGAL::squared_distance(a_handle->point(), K::Point_2(a.x, a.y)) <= r * r &&
			CGAL::squared_distance(b_handle->point(), K::Point_2(b.x, b.y)) <= r * r &&
			a_handle->info().component == b_handle->info().component)) {
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