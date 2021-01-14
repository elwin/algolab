#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void add_edge(int i0, int j0, int i1, int j1, graph &g, std::vector<std::vector<bool>> &board) {
	if (i1 < 0 || i1 >= int(board.size()) || j1 < 0 || j1 >= int(board.size()) || !board[i1][j1]) return;
	size_t n = board.size();

	boost::add_edge(i0 * n + j0, i1 * n + j1, g);
}

size_t testcase() {
	size_t n; std::cin >> n;
	std::vector<std::vector<bool>> board(n);
	size_t valids = 0;
	for (size_t i = 0; i < n; ++i) {
		board[i] = std::vector<bool>(n);

		for (size_t j = 0; j < n; ++j) {
			size_t next; std::cin >> next;
			board[i][j] = next == 1;
			if (board[i][j]) valids++;
		}
	}

	graph g(n * n);
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if ((i + j) % 2 == 0) continue; // make sure we add edges only once
			if (!board[i][j]) continue;

			add_edge(i, j, i - 1, j - 2, g, board);
			add_edge(i, j, i - 1, j + 2, g, board);
			add_edge(i, j, i + 1, j - 2, g, board);
			add_edge(i, j, i + 1, j + 2, g, board);
			add_edge(i, j, i - 2, j - 1, g, board);
			add_edge(i, j, i - 2, j + 1, g, board);
			add_edge(i, j, i + 2, j - 1, g, board);
			add_edge(i, j, i + 2, j + 1, g, board);
		}
	}


	std::vector<vertex_desc> mate_map(n * n);
	boost::edmonds_maximum_cardinality_matching(g, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));
	size_t matchings = boost::matching_size(g, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));

	return valids - matchings;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	size_t t; std::cin >> t;
	while (t--) std::cout << testcase() << std::endl;
}