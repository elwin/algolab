// Notes: Copied from [1] and adapted until it does what we want
//
// [1] https://www.boost.org/doc/libs/1_60_0/libs/graph/example/biconnected_components.cpp 

#include <iostream>
#include <vector>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/biconnected_components.hpp>


struct edge_component_t {
	enum {
		num = 555
	};
	typedef boost::edge_property_tag kind;
} edge_component;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<edge_component_t, std::size_t>> graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_t;

void testcase() {
	size_t n, m; std::cin >> n >> m;
	graph_t g(n);

	for (size_t i = 0; i < m; ++i) {
		size_t from, to; std::cin >> from >> to;
		boost::add_edge(from, to, g);		
	}

	boost::property_map<graph_t, edge_component_t>::type component = boost::get(edge_component, g);

	size_t num_comps = boost::biconnected_components(g, component);

	std::vector<std::vector<std::pair<size_t, size_t>>> bridges(num_comps);
	for (size_t i = 0; i < num_comps; ++i)
		bridges[i] = std::vector<std::pair<size_t, size_t>>(0);

	boost::graph_traits<graph_t>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
		size_t c = component[*ei];
		size_t source = boost::source(*ei, g);
		size_t target = boost::target(*ei, g);
		bridges[c].push_back({std::min(source, target), std::max(source, target)});
	}

	std::vector<std::pair<size_t, size_t>> critical(0);
	for (size_t i = 0; i < bridges.size(); ++i) {
		if (bridges[i].size() != 1) continue;
		critical.push_back(bridges[i][0]);
	}

	std::sort(critical.begin(), critical.end());
	std::cout << critical.size() << "\n";
	for (size_t i = 0; i < critical.size(); ++i) {
		std::cout << critical[i].first << " " << critical[i].second << "\n";
	}
	std::cout << std::flush;

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) testcase();
}