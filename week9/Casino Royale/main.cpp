#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

size_t testcase() {
	size_t n, m, l; std::cin >> n >> m >> l;
	size_t max_costs = (2 << 7) * n;
	size_t idx = 0;
	size_t source = idx++;
	size_t target = idx++;
	std::vector<size_t> stops(n);
	for (size_t i = 0; i < n; ++i)
		stops[i] = idx++;

	graph G(idx);
    edge_adder adder(G);

    adder.add_edge(source, stops[0], l, 0);
    adder.add_edge(stops[n - 1], target, l, 0);
	for (size_t i = 1; i < n; ++i)
    	adder.add_edge(stops[i - 1], stops[i], l, max_costs);

    for (size_t i = 0; i < m; ++i) {
    	size_t x, y, q; std::cin >> x >> y >> q;
    	adder.add_edge(stops[x], stops[y], 1, (y - x) * max_costs - q);
    }


    boost::successive_shortest_path_nonnegative_weights(G, source, target);

    return (n - 1) * l * max_costs - boost::find_flow_cost(G);
}


int main() {
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}