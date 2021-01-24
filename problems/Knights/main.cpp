#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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

struct vertex
{
	int in, out;
};

void testcase() {
	int m, n, k, c; std::cin >> m >> n >> k >> c;

	graph G(2 + 2 * m * n + n + m);
	edge_adder adder(G);

	int idx = 0;
	int source = idx++; int sink = idx++;
	
	std::vector<std::vector<vertex>> intersections(n);
	for (int i = 0; i < n; ++i) {
		intersections[i] = std::vector<vertex>(m);

		for (int j = 0; j < m; ++j) {
			intersections[i][j] = {idx++, idx++};
		}
	}

	// Connect all intersections
	for (std::size_t i = 0; i < intersections.size(); ++i) {
		for (std::size_t j = 0; j < intersections[i].size(); ++j) {

			// Add capacity for how many knights can pass through this intersection
			adder.add_edge(intersections[i][j].in, intersections[i][j].out, c);

			// Escape hallways row-wise
			if (i == 0 || i == intersections.size() - 1) {
				adder.add_edge(intersections[i][j].out, sink, 1);
			}

			// Escape hallways column-wise
			if (j == 0 || j == intersections[i].size() - 1) {
				adder.add_edge(intersections[i][j].out, sink, 1);	
			}

			// Connect to top intersection
			if (i > 0) {
				adder.add_edge(intersections[i][j].out, intersections[i - 1][j].in, 1);
			}

			// Connect to bottom intersection
			if (i < intersections.size() - 1) {
				adder.add_edge(intersections[i][j].out, intersections[i + 1][j].in, 1);
			}

			// Connect to left intersection
			if (j > 0) {
				adder.add_edge(intersections[i][j].out, intersections[i][j - 1].in, 1);
			}

			// Connect to bottom intersection
			if (j < intersections[i].size() - 1) {
				adder.add_edge(intersections[i][j].out, intersections[i][j + 1].in, 1);
			}
		}
	}

	// Add starting positions of knights
	for (int i = 0; i < k; ++i) {
		int x, y; std::cin >> x >> y;

		// Probably mixed up column and rows somewhere, so let's go with [y][x]
		adder.add_edge(source, intersections[y][x].in, 1);
	}

	long flow = boost::push_relabel_max_flow(G, source, sink);

	std::cout << flow << std::endl;
}

int main() {
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i) {
		testcase();	
	}

	return 0;
}