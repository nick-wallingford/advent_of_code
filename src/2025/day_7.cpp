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

  int64_t part1{}, part2{};
  std::pair<std::vector<int64_t>, std::vector<int64_t>> worlds;

  std::string line;
  std::getline(input, line);

  worlds.first.resize(line.size());
  worlds.second.resize(line.size());
  for (size_t i = 0; i < line.size(); i++)
    if (line[i] == 'S') {
      worlds.first[i] = 1;
      line[i] = '|';
    }

  for (std::string last_line = line; std::getline(input, line); last_line = line,
                   std::swap(worlds.first, worlds.second),
                   std::memset(worlds.second.data(), 0, worlds.second.size() * sizeof(int64_t)))
    for (size_t j = 0; j < line.size(); j++)
      if (last_line[j] == '|')
        if (line[j] == '^') {
          ++part1;
          line[j - 1] = '|';
          line[j + 1] = '|';

          worlds.second[j - 1] += worlds.first[j];
          worlds.second[j + 1] += worlds.first[j];
        } else {
          line[j] = '|';
          worlds.second[j] += worlds.first[j];
        }

  part2 = std::reduce(worlds.first.begin(), worlds.first.end());
  std::cout << "part 1: " << part1 << "\npart 2: " << part2 << '\n';
}
