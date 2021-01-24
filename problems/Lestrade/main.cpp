// This problem can be solved by
//  1. Figure out the closest gang member for each agent
//  2. Construct a LP problem to figuer out the minimum
//     amount of money that needs to be spent
// Some crucial points:
//  - The closest member can be found efficiently using Delaunay
//  - Each gang member shall only be guarded by at most one agent,
//    namely the one with the lowest wage of all possible agents
//  - Since we're only interested, whether an assignment with
//    cost lower than Holmes is possible, we can encode this
//    directly in our LP formulation to make it slightly
//    more efficient

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

bool testcase() {
	size_t z, u, v, w; std::cin >> z >> u >> v >> w;
	size_t a, g; std::cin >> a >> g;

	std::vector<member> members(g);
	std::vector<K::Point_2> points(g);
	std::map<K::Point_2, size_t> mapping;
	for (size_t i = 0; i < g; ++i) {
		int x, y; size_t u2, v2, w2; std::cin >> x >> y >> u2 >> v2 >> w2;
		members[i] = {x, y, u2, v2, w2};
		K::Point_2 pt(x, y);
		points[i] = pt;
		mapping[pt] = i;
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	std::vector<agent> agents(a);
	std::vector<int> member_agent(g, -1);
	for (size_t i = 0; i < a; ++i) {
		int x, y; size_t z; std::cin >> x >> y >> z;
		agents[i] = {x, y, z};

		K::Point_2 p = t.nearest_vertex(K::Point_2(x, y))->point();
		size_t j = mapping[p];

		if (member_agent[j] < 0 || agents[member_agent[j]].wage > agents[i].wage) {
			member_agent[j] = i;
		}
	}

	// create an LP with Ax <= b, lower bound 0 and no upper bounds
	Program lp (CGAL::SMALLER, true, 0, true, 24);

	for (size_t i = 0; i < g; ++i) {
		if (member_agent[i] < 0) continue;

		member m = members[i];
		agent a = agents[member_agent[i]];

		lp.set_a(member_agent[i], 0, -m.u);
		lp.set_a(member_agent[i], 1, -m.v);
		lp.set_a(member_agent[i], 2, -m.w);

		lp.set_c(member_agent[i], a.wage);
	}

	lp.set_b(0, -u);
	lp.set_b(1, -v);
	lp.set_b(2, -w);

	for (size_t i = 0; i < a; ++i) {
		lp.set_a(i, 3, agents[i].wage);
	}

	lp.set_b(3, z);

	Solution s = CGAL::solve_linear_program(lp, ET());

	return !s.is_infeasible();
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << (testcase() ? "L" : "H") << std::endl;
}