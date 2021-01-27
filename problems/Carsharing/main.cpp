// This problem is a bit of a weird one. At first I attempted it using DP,
// which might seem reasonable since the variables are fairly bounded
// (s <= 10, cars <= 100 * 10). However, at some point I gave up since
// I couldn't figure out how to properly memoize vectors (again,
// bounded vectors).
// Anyhow, flows seem the way to go: Keep a node for each station at
// every possible time point, with flows that correspond to cars being
// moved. Generally, every path has cost 0, except bookings have negative
// costs. This can be avoided however by adding a fixed offset to all
// edges.
#include <iostream>
#include <vector>

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

const size_t infinite = 999999;
const size_t max_cost = 10 * 100;
const size_t max_time = 100000;

struct booking {
	size_t depart, arrival;
	size_t start, end;
	size_t profit;
};

long testcase() {
	size_t n, s; std::cin >> n >> s;

	std::vector<size_t> stations(s);
	std::vector<booking> bookings(n);

	// For each station, keep a mapping from time to index
	std::vector<std::map<size_t, size_t>> station_times(s, std::map<size_t, size_t>());
	size_t cars = 0; // Number of total cars
	size_t idx = 0;  // Index used in graph

	size_t source = idx++;
	size_t target = idx++;

	// Add transition edges
	for (size_t i = 0; i < s; ++i) {
		std::cin >> stations[i];
		cars += stations[i];

		// Add starting and end-point for all stations,
		// such that the total cost is the same (i.e. max_time).
		station_times[i][0] = idx++;
		station_times[i][max_time] = idx++;
	}


	for (size_t i = 0; i < n; ++i) {
		booking b;
		std::cin >> b.depart >> b.arrival >> b.start >> b.end >> b.profit;
		b.depart--; b.arrival--; // Convert to 0-indexing
		bookings[i] = b;

		// If station / time pair has not been seen yet, add it to working set
		if (station_times[b.depart].find(b.start) == station_times[b.depart].end())
			station_times[b.depart][b.start] = idx++;

		if (station_times[b.arrival].find(b.end) == station_times[b.arrival].end())
			station_times[b.arrival][b.end] = idx++;
	}

	graph G(idx);
	edge_adder adder(G);

	for (size_t i = 0; i < s; ++i) {

		auto it = station_times[i].begin();
		size_t cur_idx = it->second;
		size_t cur_time = (it++)->first;

		// Add edges from source to first time point (i.e. time = 0)
		adder.add_edge(source, cur_idx, stations[i], 0);

		for (; it != station_times[i].end(); it++) {
			size_t next_idx = it->second;
			size_t next_time = it->first;

			// Add edges inbetween times, cost corresponds to time difference
			adder.add_edge(cur_idx, next_idx, infinite, max_cost * (next_time - cur_time));

			cur_idx = next_idx;
			cur_time = next_time;
		}

		// Add edges from last time point (i.e. time = max_time) to target
		adder.add_edge(cur_idx, target, infinite, 0);
	}

	for (size_t i = 0; i < n; ++i) {
		booking b = bookings[i];

		size_t start = station_times[b.depart][b.start];
		size_t end = station_times[b.arrival][b.end];

		// Add edges for trips - cost corresponds to profit generated,
		// (or rather the inverse of it)
		adder.add_edge(start, end, 1, max_cost * (b.end - b.start) - b.profit);
	}

	boost::successive_shortest_path_nonnegative_weights(G, source, target);
	long cost = boost::find_flow_cost(G);

	return max_time * max_cost * cars - cost;
}

int main() {
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}