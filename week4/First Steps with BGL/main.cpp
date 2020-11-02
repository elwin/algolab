#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor Edge;


void testcase() {
	int n, m; std::cin >> n >> m;
	weighted_graph G(n);
	std::vector<std::vector<int>> weights(n);
	for (int i = 0; i < n; ++i) {
		weights[i] = std::vector<int>(n);
	}

	for (int i = 0; i < m; ++i) {
		int u, v, weight; std::cin >> u >> v >> weight;
		boost::add_edge(u, v, weight, G);
		weights[u][v] = weight;
		weights[v][u] = weight;
	}

	std::vector<Edge> spanning_tree;
	kruskal_minimum_spanning_tree(G, std::back_inserter(spanning_tree));

	int weight = 0;
 	for (std::vector< Edge >::iterator ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei) {
 		int source = boost::source(*ei, G);
 		int destination = boost::target(*ei, G);

 		weight += weights[source][destination];
    }

    std::vector<int> shortest_paths(n);
	auto dist = boost::distance_map(boost::make_iterator_property_map(shortest_paths.begin(), boost::get(boost::vertex_index, G)));
    dijkstra_shortest_paths(G, 0, dist);
    int longest_path = 0;
    for (int i = 0; i < n; ++i) {
    	longest_path = std::max(longest_path, shortest_paths[i]);
    }


    std::cout << weight << " " << longest_path << std::endl;
}

int main() {
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i) {
		testcase();
	}

	return 0;	
}