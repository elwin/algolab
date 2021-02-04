// This problem is close to a typicall bi-partite matching
// problem, except that we're not interested in maximum
// but rather a coefficient maximization (i.e. cost 
// minimization) problem.
// To allow for boats or sailors not to be matched,
// we simply add psuedo edges with no coefficients
// (i.e. max costs) from each sailor to source.
// The same would work by adding those edges
// from each boat to each sink. For some
// reason, it doesn't work when doing
// both (sum will always equal 0).

#include <iostream>
// BGL includes
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

const size_t max_cost = 50;

int testcase() {
    size_t b, s, p; std::cin >> b >> s >> p;

    size_t source = b + s;
    size_t sink = b + s + 1;

    graph G(b + s + 2);
    edge_adder adder(G);

    for (size_t i = 0; i < b; ++i) {
        // Edge from source to boat
        adder.add_edge(source, i, 1, 0);
    }

    for (size_t i = 0; i < s; ++i) {
        // Edge from sailor to sink
        adder.add_edge(i + b, sink, 1, 0);
        
        // // Pseudo matching edge
        adder.add_edge(source, i + b, 1, max_cost);
    }

    for (size_t i = 0; i < p; ++i) {
        size_t from, to, c; std::cin >> from >> to >> c;
        adder.add_edge(from, to + b, 1, max_cost - c);
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    return s * max_cost - boost::find_flow_cost(G);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t t; std::cin >> t;
    while (t--) std::cout << testcase() << std::endl;
}
