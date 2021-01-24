#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
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

bool testcase() {
  size_t w, h; std::cin >> w >> h;
  size_t idx = 0;
  size_t source = idx++;
  size_t sink = idx++;
  size_t tiles = 0;

  std::vector<std::vector<bool>> garden(h);
  std::vector<std::vector<int>> garden_idx(h);

  for (size_t i = 0; i < h; ++i) {
    garden[i] = std::vector<bool>(w);
    garden_idx[i] = std::vector<int>(w);

    for (size_t j = 0; j < w; ++j) {
      char n; std::cin >> n;
      if (n == '.') {
        tiles++;
        garden[i][j] = true;
      }
      garden_idx[i][j] = idx++;
    }
  }


  graph G(idx);
  edge_adder adder(G);

  for (size_t i = 0; i < h; ++i) {
    for (size_t j = 0; j < w; ++j) {
      if (!garden[i][j]) continue;

      size_t cur_idx = garden_idx[i][j];

      if (i % 2 == j % 2) {
        adder.add_edge(source, cur_idx, 1);
        
        if (i > 0 && garden[i - 1][j]) adder.add_edge(cur_idx, garden_idx[i - 1][j], 1);
        if (j > 0 && garden[i][j - 1]) adder.add_edge(cur_idx, garden_idx[i][j - 1], 1);
        if (i < h - 1 && garden[i + 1][j]) adder.add_edge(cur_idx, garden_idx[i + 1][j], 1);
        if (j < w - 1 && garden[i][j + 1]) adder.add_edge(cur_idx, garden_idx[i][j + 1], 1);

      } else {
        adder.add_edge(cur_idx, sink, 1);
      }
    }
  }

  size_t flow = boost::push_relabel_max_flow(G, source, sink);


  // One unit of flow accounts for one domino, thus covering two tiles
  return tiles == flow * 2; 
}

int main() {
  std::ios_base::sync_with_stdio(false);
  size_t t; std::cin >> t;
  while (t--) std::cout << (testcase() ? "yes" : "no") << std::endl;
}