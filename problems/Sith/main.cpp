// After a given amount of time has passed, the largest possible score
// that can be achieved corresponds to the either the time itself
// or the largest connected subcomponent in the remaining graph,
// whichever is smaller.
// The largest connected subcomponent can be retrieved using
// BFS after we've constructed a graph (either quadratically
// or logarithmically using Delanauy, since we care for the
// closest neighbors anyway).
// Now to figure out how much time should pass we can use
// binary search: The optimum is when score = time, since
// at that point increasing time would decrease the availablee
// nodes and thus score, decreasing time would decrease the
// maximum possible score as well (since it is capped
// using min(score, time)).
// Note that score is a (non-strictly) monotonic increasing
// function. Now, whenever the score is lower than time,
// we can try to get closer to the maximum by
// decreasing the maximum. Conversely, when
// the score is higher than time, we can
// get closer to the optimum by increaing time.
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

	return max_score;
}

size_t testcase() {
	size_t n, r; std::cin >> n >> r;
	std::vector<K::Point_2> points(n);
	for (size_t i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	size_t low = 1; size_t high = n / 2;
	while (low < high) {
		size_t mid = low + (high - low) / 2;
		size_t score = possible(points, mid, r * r);

		// Found optimimum - can't do better than that
		// Increasing time would decrease the score,
		// decreasing time would decrease it naturally as well
		if (score == mid) return score;

		// Score is too low, can potentially improve by
		// lowering time
		if (score < mid) {
			high = mid - 1;
		}

		// Score is too high, looking for closer solution
		// by increasing time
		else {
			low = mid + 1;
		}
	}

	// Handle off by one error or so
	size_t score1 = std::min(low - 1, possible(points, low - 1, r * r));
	size_t score2 = std::min(low, possible(points, low, r * r));
	size_t score3 = std::min(low + 1, possible(points, low + 1, r * r));

	return std::max(score1, std::max(score2, score3));
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}