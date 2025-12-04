#include "util.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

void aoc_2025_1() {
  std::ifstream ss{get_filename(2025, 1)};
  int64_t position{100000050};
  size_t part_1{0};
  size_t part_2{0};

  for (std::string line; std::getline(ss, line);) {
    int n{};
    for (size_t i = 1; '0' <= line[i] && line[i] <= '9'; i++) {
      n *= 10;
      n += line[i] ^ '0';
    }
    const bool left = line[0] == 'L';
    const int64_t next = position - (left ? n : -n);
    const bool at_zero = next % 100 == 0;
    part_1 += at_zero;

    int64_t rotations = 0;
    if (at_zero)
      rotations = n / 100 + 1;
    else
      rotations = std::abs(position / 100 - next / 100) - (left && (position % 100 == 0));
    part_2 += rotations;

    position = next;
  }

  std::cout << "part 1: " << part_1 << std::endl;
  std::cout << "part 2: " << part_2 << std::endl;
}
