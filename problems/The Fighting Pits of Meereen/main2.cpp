#include <iostream>

union Ints {
  struct {
    int part1 : 10;
    int part2 : 6;
    int part3 : 16;
 } word;
 uint32_t another_way_to_access_word;
};

int main() {

	Ints x = {1, 2, 3};
	std::cout << x.another_way_to_access_word << std::endl;

	uint32_t y = x.another_way_to_access_word;

	

}