#include "util.hpp"
#include <fstream>
#include <iostream>

static int64_t mark(std::vector<std::string> &map) {
  int64_t count{};
  for (size_t i = 1; i < map.size() - 1; i++) {
    for (size_t j = 1; j < map.front().size() - 1; j++) {
      if (map[i][j] == '.')
        continue;

      int neighbors = 0;
      for (size_t k = i - 1; k <= i + 1; k++)
        for (size_t l = j - 1; l <= j + 1; l++)
          neighbors += map[k][l] != '.';
      if (neighbors < 5) {
        ++count;
        map[i][j] = 'x';
      }
    }
  }
  return count;
}

static void sweep(std::vector<std::string> &map) {
  for (size_t i = 1; i < map.size() - 1; i++)
    for (size_t j = 1; j < map.front().size() - 1; j++)
      if (map[i][j] == 'x')
        map[i][j] = '.';
}

void aoc_2025_4() {
  std::ifstream input{get_filename(2025, 4)};
  int64_t part1{}, part2{};

  std::vector<std::string> map;
  for (std::string line; std::getline(input, line);) {
    if (map.empty())
      map.emplace_back(line.size() + 2, '.');

    map.emplace_back(".");
    map.back() += line;
    map.back() += '.';
  }
  map.emplace_back(map.front());

  part1 = part2 = mark(map);

  int64_t removals;
  do {
    sweep(map);
    part2 += removals = mark(map);
  } while (removals);

  std::cout << "part1: " << part1 << "\npart2: " << part2 << '\n';
}
