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

const size_t infinite = 100000;
const size_t max_cost = 100 * 1000;

struct booking {
	size_t depart, arrival;
	size_t start, end;
	size_t profit;
};

size_t idx(size_t station, size_t time, size_t s) {
	return station + time * s;
}

long testcase() {
	size_t n, s; std::cin >> n >> s;

	std::vector<size_t> stations(s);
	std::vector<booking> bookings(n);
	std::set<size_t> timeslots;
	size_t cars = 0;

	// Add transition edges
	for (size_t i = 0; i < s; ++i) {
		std::cin >> stations[i];
		cars += stations[i];
	}

	for (size_t i = 0; i < n; ++i) {
		booking b;
		std::cin >> b.depart >> b.arrival >> b.start >> b.end >> b.profit;
		b.depart--; b.arrival--;
		bookings[i] = b;

		timeslots.insert(b.start);
		timeslots.insert(b.end);
		// adder.add_edge(idx(depart, start), idx(arrival, end), 1, (end - start) * max_cost - profit);
	}

	std::map<size_t, size_t> timeslot_mapping;
	{
		size_t i = 0;
		for (size_t slot : timeslots) {
			timeslot_mapping[slot] = i++;
		}
	}

	size_t source = timeslots.size() * s;
	size_t target = timeslots.size() * s + 1;


	graph G(target + 1);
	edge_adder adder(G);
	for (size_t i = 0; i < s; ++i) {
		adder.add_edge(source, idx(i, 0, s), stations[i], 0);

		for (size_t j = 1; j < timeslots.size(); ++j)
			adder.add_edge(idx(i, j - 1, s), idx(i, j, s), infinite, max_cost);

		adder.add_edge(idx(i, timeslots.size() - 1, s), target, infinite, 0);
	}

	// for (auto slot : timeslots) {
	// 	std::cout << slot << " (" << timeslot_mapping[slot] << ") ";
	// }
	// std::cout << std::endl;

	for (size_t i = 0; i < n; ++i) {
		booking b = bookings[i];
		// std::cout << i << std::endl;
		adder.add_edge(
			idx(b.depart, timeslot_mapping[b.start], s),
			idx(b.arrival, timeslot_mapping[b.end], s),
			1,
			max_cost * (timeslot_mapping[b.end] - timeslot_mapping[b.start]) - b.profit
		);

		// std::cout << max_cost * (timeslot_mapping[b.end] - timeslot_mapping[b.start]) - b.profit << std::endl;
	}

	boost::successive_shortest_path_nonnegative_weights(G, source, target);
	long cost = boost::find_flow_cost(G);

	return (timeslots.size() - 1) * max_cost * cars - cost;
}

int main() {
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}