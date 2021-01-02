// ALGOLAB BGL Tutorial 3
// Code demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

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

void testcase() {
    size_t N, M, S; std::cin >> N >> M >> S;
    std::vector<size_t> state_limits(S);
    for (size_t i = 0; i < S; ++i) {
        std::cin >> state_limits[i];
    }

    std::vector<size_t> site_assignments(M);
    for (size_t i = 0; i < M; ++i) {
        std::cin >> site_assignments[i];
        site_assignments[i]--;
    }

    std::vector<std::vector<size_t>> bids(N);
    for (size_t i = 0; i < N; ++i) {
        bids[i] = std::vector<size_t>(M);
        for (size_t j = 0; j < M; ++j) {
            std::cin >> bids[i][j];
        }
    }

    size_t idx = 0;
    size_t v_source = idx++;
    size_t v_target = idx++;

    std::vector<size_t> states(S);
    for (size_t i = 0; i < S; ++i) states[i] = idx++;

    std::vector<size_t> sites(M);
    for (size_t i = 0; i < M; ++i) sites[i] = idx++;


    std::vector<size_t> bidders(N);
    for (size_t i = 0; i < N; ++i) bidders[i] = idx++;

    graph G(0);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    // Edges from Source to States
    for (size_t i = 0; i < S; ++i) {
        adder.add_edge(v_source, states[i], state_limits[i], 0);
    }

    // Edges from States to Sites
    for (size_t i = 0; i < M; ++i) {
        adder.add_edge(states[site_assignments[i]], sites[i], 1, 0);
    }

    // Edges from Sites to Bidders
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            // Since we want to maximize profit, we invert the cost,
            // and to avoid negative costs, we add an offset of 100 (since this is the maximum bid)
            adder.add_edge(sites[j], bidders[i], 1, 100 - bids[i][j]);
        }
    }

    // Edges from Bidders to Sink
    for (size_t i = 0; i < N; ++i) {
        adder.add_edge(bidders[i], v_target, 1, 0);
    }

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];     
    }

    // Our costs represent negative profits, offset by 100
    std::cout << flow << " " << flow * 100 - cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    size_t T; std::cin >> T;
    while (T--) testcase();
}
