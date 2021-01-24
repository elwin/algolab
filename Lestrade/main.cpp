#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<size_t, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

struct member {
	int x, y;
	size_t u, v, w;
};

struct agent {
	int x, y;
	size_t wage;
};

size_t squared_distance(int x0, int y0, int x1, int y1) {
	return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
}

size_t closest_member(agent a, std::vector<member> &members) {
	size_t distance = std::numeric_limits<size_t>::max();
	size_t index = 0;
	for (size_t i = 0; i < members.size(); i++) {
		member m = members[i];
		int new_dist = squared_distance(a.x, a.y, m.x, m.y);
		if (new_dist < distance) {
			distance = new_dist;
			index = i;
		}
	}

	return index;
}

bool testcase() {
	size_t z, u, v, w; std::cin >> z >> u >> v >> w;
	size_t a, g; std::cin >> a >> g;

	std::vector<member> members(g);
	std::vector<K::Point_2> points(g);
	for (size_t i = 0; i < g; ++i) {
		int x, y; size_t u2, v2, w2; std::cin >> x >> y >> u2 >> v2 >> w2;
		members[i] = {x, y, u2, v2, w2};
		points[i] = K::Point_2(x, y);
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	std::vector<agent> agents(a);
	std::vector<int> member_agent(g, -1);
	for (size_t i = 0; i < a; ++i) {
		int x, y; size_t z; std::cin >> x >> y >> z;
		agents[i] = {x, y, z};
		size_t j = closest_member(agents[i], members);
		if (member_agent[j] < 0 || agents[member_agent[j]].wage > agents[i].wage) {
			member_agent[j] = i;
		}
	}

	// for (size_t i = 0; i < g; ++i) {
	// 	std::cout << i << " -> " << member_agent[i] << std::endl;
	// }

	// create an LP with Ax <= b, lower bound 0 and no upper bounds
	Program lp (CGAL::LARGER, true, 0, true, 24);

	// for (int i = 0; i < a; ++i) {
	// 	lp.set_u(i, true, 24);
	// 	lp.set_l(i, true, 0);
	// }

	for (size_t i = 0; i < g; ++i) {
		if (member_agent[i] < 0) continue;

		member m = members[i];
		agent a = agents[member_agent[i]];

		lp.set_a(i, 0, m.u);
		lp.set_a(i, 1, m.v);
		lp.set_a(i, 2, m.w);

		lp.set_c(i, a.wage);
	}

	lp.set_b(0, u);
	lp.set_b(1, v);
	lp.set_b(2, w);

	Solution s = CGAL::solve_linear_program(lp, ET());
	// return !s.is_infeasible();

	if (s.is_infeasible()) return false;

	return CGAL::to_double(s.objective_value()) <= z;

}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << (testcase() ? "L" : "H") << std::endl;
}