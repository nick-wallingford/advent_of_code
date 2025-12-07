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
  std::vector<std::string> tree;

  for (std::string line; std::getline(input, line);)
    tree.push_back(std::move(line));

  std::pair<std::vector<int64_t>, std::vector<int64_t>> worlds;
  worlds.first.resize(tree.front().size());
  worlds.second.resize(tree.front().size());
  for (size_t i = 0; i < tree.front().size(); i++)
    if (tree[0][i] == 'S') {
      worlds.second[i] = 1;
      tree[1][i] = '|';
    }
  for (size_t i = 1; i < tree.size() - 1; i++) {
    std::swap(worlds.first, worlds.second);
    std::memset(worlds.second.data(), 0, worlds.second.size() * sizeof(int64_t));

    for (size_t j = 0; j < tree.front().size(); j++)
      if (tree[i][j] == '|')
        if (tree[i + 1][j] == '^') {
          ++part1;
          tree[i + 1][j - 1] = '|';
          tree[i + 1][j + 1] = '|';

          worlds.second[j - 1] += worlds.first[j];
          worlds.second[j + 1] += worlds.first[j];
        } else {
          tree[i + 1][j] = '|';
          worlds.second[j] += worlds.first[j];
        }
  }

  part2 = std::reduce(worlds.second.begin(), worlds.second.end());
  std::cout << "part 1: " << part1 << "\npart 2: " << part2 << '\n';
}
