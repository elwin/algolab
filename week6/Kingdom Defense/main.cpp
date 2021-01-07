#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}
	void add_edge(int from, int to, long capacity) {
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const auto e = boost::add_edge(from, to, G).first;
		const auto rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

struct path {
	int from, to, min, max;
};

struct location {
	int current, desired;
	size_t idx;
};

bool testcase() {
	size_t l, p; std::cin >> l >> p;
	std::vector<location> locations(l);
	size_t idx = 0;
	size_t source = idx++;
	size_t target = idx++;

	for (size_t i = 0; i < l; ++i) {
		int current, desired; std::cin >> current >> desired;
		locations[i] = {current, desired, idx++};
	}

	std::vector<path> paths(p);
	for (size_t i = 0; i < p; ++i) {
		int start, end, min, max; std::cin >> start >> end >> min >> max;
		paths[i] = {start, end, min, max};
	}

	graph G(idx);
	edge_adder adder(G);

	size_t goal = 0;

	for (size_t i = 0; i < l; ++i) {
		adder.add_edge(source, locations[i].idx, locations[i].current);
		adder.add_edge(locations[i].idx, target, locations[i].desired);

		goal += locations[i].desired;
	}

	for (size_t i = 0; i < p; ++i) {
		// Enfore minimum flow by manually wiring it up with source / sink
		adder.add_edge(source, locations[paths[i].to].idx, paths[i].min);
		adder.add_edge(locations[paths[i].from].idx, target, paths[i].min);
		goal += paths[i].min;

		adder.add_edge(locations[paths[i].from].idx, locations[paths[i].to].idx, paths[i].max);
	}


	size_t flow = boost::push_relabel_max_flow(G, source, target);

	return flow >= goal;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << (testcase() ? "yes" : "no") << std::endl;
}