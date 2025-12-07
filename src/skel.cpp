#include "util.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

void aoc_2025_y() {
#if 0
  std::ifstream input{get_filename(2025, -1)};
#else
  const std::string data{R"()"};
  std::istringstream input{data};
#endif

  int64_t part1{}, part2{};

  for (std::string line; std::getline(input, line);) {
    std::cout << line << '\n';
  }

  std::cout << "part 1: " << part1 << "\npart 2: " << part2 << '\n';
}
