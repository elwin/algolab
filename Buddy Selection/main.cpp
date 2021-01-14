// Copied from [1] and adapted until it did what we want
//
// [1] https://www.boost.org/doc/libs/1_59_0/libs/graph/example/matching_example.cpp

#include <map>
#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

bool testcase() {
    size_t n, c, f; std::cin >> n >> c >> f;

    graph g(n);

    std::map<std::string, std::vector<int>> preferences;

    // Collect characteriistics and store in maps
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < c; ++j) {
            std::string characteristic;
            std::cin >> characteristic;

            if (preferences.find(characteristic) == preferences.end()) {
                preferences[characteristic] = std::vector<int>(0);
            }

            preferences[characteristic].push_back(i);
        }
    }

    // Initialize similiatiry matrix
    std::vector<std::vector<size_t>> preference_overlap(n);
    for (size_t i = 0; i < n; ++i) {
        preference_overlap[i] = std::vector<size_t>(n);
    }

    // preference_overlap[i][j] corresponds to how many characteristics
    // persons i and j share
    for (auto it = preferences.begin(); it != preferences.end(); it++) {
        std::vector<int> people = it->second;

        for (size_t i = 0; i < people.size(); ++i) {
            for (size_t j = i + 1; j < people.size(); ++j) {
                preference_overlap[people[i]][people[j]]++;
            }
        }
    }

    // add edge between two persons when the overlap in preferences
    // is at least f
    for (size_t i = 0; i < preference_overlap.size(); ++i) {
        for (size_t j = i + 1; j < preference_overlap.size(); ++j) {
            // std::cout << f << ": " << preference_overlap[i][j] << std::endl;
            if (preference_overlap[i][j] <= f) {
                // std::cout << "skipping " << i << " " << j << std::endl;
                continue;
            };

            boost::add_edge(i, j, g);
        }
    }


    std::vector<boost::graph_traits<graph>::vertex_descriptor> mate(n);

    boost::checked_edmonds_maximum_cardinality_matching(g, &mate[0]);

    // // Uncomment to get matching
    // boost::graph_traits<graph>::vertex_iterator vi, vi_end;
    // for(boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
    //     if (mate[*vi] != boost::graph_traits<graph>::null_vertex() && *vi < mate[*vi])
    //         std::cout << "{" << *vi << ", " << mate[*vi] << "}" << std::endl;
    // }

    return boost::matching_size(g, &mate[0]) * 2 == n;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t t; std::cin >> t;
    while (t--) std::cout << (testcase() ? "not optimal" : "optimal") << std::endl;
}