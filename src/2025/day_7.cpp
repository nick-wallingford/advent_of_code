#include "util.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>

void aoc_2025_7() {
#if 1
  std::ifstream input{get_filename(2025, 7)};
#else
  const std::string data{R"(.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............)"};
  std::istringstream input{data};
#endif

  std::string line;
  std::getline(input, line);
  std::vector<int64_t> worlds(line.size(), 0);
  for (size_t i = 0; i < line.size(); i++)
    if (line[i] == 'S')
      worlds[i] = 1;

  int64_t part1 = 0;
  while (std::getline(input, line))
    for (size_t i = 0; i < line.size(); i++)
      if (worlds[i])
        if (line[i] == '^') {
          ++part1;
          worlds[i - 1] += worlds[i];
          worlds[i + 1] += worlds[i];
          worlds[i] = 0;
        }

  std::cout << "part 1: " << part1 << "\npart 2: " << std::reduce(worlds.begin(), worlds.end()) << '\n';
}
