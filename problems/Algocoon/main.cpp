#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor	edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
	graph &G;

public:
	explicit edge_adder(graph &G) : G(G) {}

	void add_edge(int from, int to, long capacity) {
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const edge_desc e = boost::add_edge(from, to, G).first;
		const edge_desc rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

long testcase() {
	size_t n, m; std::cin >> n >> m;

	graph G(m);
	edge_adder adder(G);

	for (size_t i = 0; i < m; ++i) {
		size_t a, b, c; std::cin >> a >> b >> c;
		adder.add_edge(a, b, c);
	}

	if (m == 0) return 0;

	long min_cut = std::numeric_limits<long>::max();
	for (size_t j = 0; j < n; ++j) {
		// There must be a boundary where we take one of the figures and
		// our friend takes the next one
		min_cut = std::min(min_cut, boost::push_relabel_max_flow(G, j, (j + 1) % n));
	}

	return min_cut;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}