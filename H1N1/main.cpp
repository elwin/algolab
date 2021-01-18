#include <iostream>
#include <vector>
#include <queue>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef std::pair<K::FT, Triangulation::Face_handle> HandlePair;

bool testcase() {
	size_t n; std::cin >> n;
	if (n == 0) return false;

	std::vector<K::Point_2> points(n);
	for (size_t i = 0; i < n; ++i) {
		int x, y; std::cin >> x >> y;
		points[i] = K::Point_2(x, y);
	}

	Triangulation t;
	t.insert(points.begin(), points.end());

	std::priority_queue<HandlePair, std::vector<HandlePair>, std::less<HandlePair>> queue;
	for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
		f->info() = 0;

		if (t.is_infinite(f)) {
			// Faces that are outside have infinite size, as arbitrary
			// distances can be satisfied
			queue.push({std::numeric_limits<K::FT>::max(), f});
		}
	}

	while (!queue.empty()) {
		HandlePair next = queue.top(); queue.pop();
		K::FT distance = next.first;
		Triangulation::Face_handle f = next.second;

		// Ignore points that have already been processed - since we're
		// using a priority queue, we'll never try to assign a bigger
		// size to an already processed face
		if (f->info() != 0) continue;

		f->info() = distance;

		for (size_t i = 0; i < 3; i++) {
			Triangulation::Face_handle neighbor = f->neighbor(i);
			
			K::FT next_distance = std::min(t.segment(f, i).squared_length() / 4, distance);
			queue.push({ next_distance, neighbor });
		}
	}

	size_t m; std::cin >> m;
	for (size_t i = 0; i < m; ++i) {
		int x, y; long d; std::cin >> x >> y >> d;
	
		K::Point_2 person(x, y);
		Triangulation::Vertex_handle v = t.nearest_vertex(person);

		// Check that we're not too close to the closest person
		if (CGAL::squared_distance(person, v->point()) < d) {
			std::cout << "n"; continue;
		}

		// Check the maximum allowed size for the face that we're currently in
		Triangulation::Face_handle f = t.locate(person);
		if (f->info() < d) {
			std::cout << "n"; continue;
		}

		std::cout << "y";
	}

	std::cout << std::endl;

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (testcase());
}