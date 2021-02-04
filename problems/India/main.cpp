// More or less typical max-flow, min-cost problem with flow set to
// the number of elephants between each city and cost to... well
// cost of one elephant.
// When solving the flow-problem, we'll attempt to get maximum flow,
// irrespective of cost. Thus, we can constrain the flow (i.e. number
// of luggage we want to transport) to some number and check whether
// the costs exceed our budget. We can set this constraint by
// inserting an edge between source and the actual start
// with the maximum luggage.
// The correct luggage number we transport we can figuer out
// using exponential and binary search.

// Includes
// ========
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

struct guide {
    size_t from, to, cost, cap;
};

size_t costs(std::vector<guide> &guides, size_t c, size_t from, size_t to, size_t units) {
     size_t source = c;

    graph G(c + 1);
    edge_adder adder(G);

    for (size_t i = 0; i < guides.size(); i++) {
        guide g = guides[i];

        adder.add_edge(g.from, g.to, g.cap, g.cost);
    }

    adder.add_edge(source, from, units, 0);

    boost::successive_shortest_path_nonnegative_weights(G, source, to);

    size_t s_flow = 0;
    out_edge_it e, eend;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
        s_flow += c_map[*e] - rc_map[*e];

    if (s_flow < units) return std::numeric_limits<size_t>::max();

    return boost::find_flow_cost(G);
}

int testcase() {
    size_t c, n, budget, from, to;
    std::cin >> c >> n >> budget >> from >> to;
    std::vector<guide> guides(n);

    size_t max_units = 0;

    for (size_t i = 0; i < n; ++i) {
        guide g;
        std::cin >> g.from >> g.to >> g.cost >> g.cap;
        guides[i] = g;

        if (g.from == from) max_units += g.cap;
    }

    // Exponential search for upper bound
    size_t low = 0; size_t high = 1;
    while (true) {
        size_t cur_costs = costs(guides, c, from, to, high);
        if (cur_costs < budget) {
            low = high;
            high *= 2;
        }

        else if (cur_costs > budget) break;
        else return high;
    }

    // Binary search for correct value
    while (low < high) {
        size_t middle = low + (high - low) / 2;

        size_t cur_costs = costs(guides, c, from, to, middle);

        if (cur_costs < budget) low = middle + 1;
        else if (cur_costs > budget) high = middle - 1;
        else return middle;
    }

    // If there's no exact match, our threshold will
    // be set one too high
    if (costs(guides, c, from, to, low) > budget) return low - 1;
    return high;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t t; std::cin >> t;
    while (t--) std::cout << testcase() << std::endl;
}