#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<bool, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

size_t possible(std::vector<K::Point_2> &points, size_t time, long sqrt_dist) {
	Triangulation t;
	t.insert(points.begin() + time, points.end());

	size_t max_score = 0;
	for (Triangulation::Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
		v->info() = false;
	}

	for (Triangulation::Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {

		if (v->info() == true) continue;

		size_t cur_score = 0;
		
		std::queue<Triangulation::Vertex_handle> q;
		q.push(v);

		while (!q.empty()) {
			auto cur = q.front(); q.pop();
			if (cur->info() == true) continue;
			cur->info() = true;
			cur_score++;

			Triangulation::Vertex_circulator next = t.incident_vertices(cur);
			do {
				if (t.is_infinite(next) || next->info() == true) continue;

				if (CGAL::squared_distance(cur->point(), next->point()) > sqrt_dist) continue;

				q.push(next);
			} while (++next != t.incident_vertices(cur));
		}

		max_score = std::max(max_score, cur_score);
	}

	return std::min(max_score, time);
}

size_t testcase() {
	size_t n, r; std::cin >> n >> r;
	std::vector<K::Point_2> points(n);
	for (size_t i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	size_t max = 0;
	for (size_t time = n / 2; time > 0; time--) {
		std::cout << n << ": " << possible(points, time, r * r) << std::endl;
		max = std::max(max, possible(points, time, r * r));
	}

	return max;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}