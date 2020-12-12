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

struct Status {
	bool visited;
	bool color;
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

bool color_vertices(Triangulation &t, Vertex_handle v, int rr, bool expected, int component) {
	if (v->info().visited) {
		return v->info().color == expected;
	}

	v->info().visited = true;
	v->info().color = expected;
	v->info().component = component;

	Vertex_circulator c = t.incident_vertices(v);
	do {
		if (t.is_infinite(c) || CGAL::squared_distance(v->point(), c->point()) > rr) continue;

		if (!color_vertices(t, c, rr, !expected, component)) {
			return false;
		}

	} while (++c != t.incident_vertices(v));

	return true;
}


void testcase() {
	int n, m, r; std::cin >> n >> m >> r;
	int rr = r * r;

	std::vector<K::Point_2> stations(n);
	for (int i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		stations[i] = K::Point_2(x, y);
	}

	std::vector<K::Point_2> a_clue(m);
	std::vector<K::Point_2> b_clue(m);
	for (int i = 0; i < m; ++i) {
		int ax, ay, bx, by;
		std::cin >> ax >> ay >> bx >> by;
		a_clue[i] = K::Point_2(ax, ay);
		b_clue[i] = K::Point_2(bx, by);
	}

	Triangulation t, t0, t1;
	t.insert(stations.begin(), stations.end());

	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		v->info() = {false, false, -1};
	}


	bool possible = true;
	int component = 1;
	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		if (!v->info().visited && !color_vertices(t, v, rr, false, component++)) {
			possible = false;
		}
	}

	for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		ass(v->info().visited == true, "not all vertices visited");
		ass(v->info().component > 0, "not all components larger than 0");
	}

	if (possible) {
		for (Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
			if (v->info().color) {
				t0.insert(v->point());
			} else {
				t1.insert(v->point());
			}
		}

		for (Edge_iterator e = t0.finite_edges_begin(); e != t0.finite_edges_end(); e++) {
			if (t1.segment(e).squared_length() <= rr) {
				possible = false;
				break;
			}
		}

		for (Edge_iterator e = t1.finite_edges_begin(); e != t1.finite_edges_end(); e++) {
			if (t1.segment(e).squared_length() <= rr) {
				possible = false;
				break;
			}
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
		K::Point_2 a = a_clue[i];
		K::Point_2 b = b_clue[i];
		Vertex_handle a_nearest = t.nearest_vertex(a);
		Vertex_handle b_nearest = t.nearest_vertex(b);

		bool condition = CGAL::squared_distance(a, b) <= rr;
		condition = condition || (a_nearest->info().component == b_nearest->info().component && 
			CGAL::squared_distance(a_nearest->point(), a) <= rr &&
			CGAL::squared_distance(b_nearest->point(), b) <= rr);

		std::cout << (condition ? 'y' : 'n');
	}
	std::cout << std::endl;

}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i) {
		testcase();
	}
}