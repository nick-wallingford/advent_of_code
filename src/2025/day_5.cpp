#include "util.hpp"
#include <fstream>
#include <iostream>
#include <regex>

void aoc_2025_5() {
  std::ifstream input{get_filename(2025, 5)};

  // read fresh ingredient ranges
  std::vector<std::pair<uint64_t, uint64_t>> fresh;
  std::regex regex{"([0-9]+)-([0-9]+)"};
  for (std::string line; std::getline(input, line);) {
    std::smatch results;
    if (!std::regex_match(line, results, regex))
      break;

    fresh.emplace_back(std::stoul(results[1].str()), std::stoul(results[2].str()));
  }
  std::sort(fresh.begin(), fresh.end());

  // read produce ids
  std::vector<uint64_t> produce;
  for (std::string line; std::getline(input, line);)
    produce.emplace_back(std::stoul(line));
  std::sort(produce.begin(), produce.end());

  // count part 1, do most of part 2.
  size_t part1{}, part2{}, left{}, right{};
  auto f = fresh.cbegin();
  auto p = produce.cbegin();
  while (f != fresh.cend() && p != produce.cend()) {
    if (*p < f->first)
      ++p;
    else if (*p > f->second) {
      if (const auto [a, b] = *f; a > right + 1) {
        part2 += 1 + right - left;
        left = a;
        right = b;
      } else
        right = std::max(right, b);
      ++f;
    } else {
      ++part1;
      ++p;
    }
  }

  // finish part 2. this should be nearly complete.
  for (; f != fresh.cend(); ++f)
    if (const auto [a, b] = *f; a > right + 1) {
      part2 += 1 + right - left;
      left = a;
      right = b;
    } else
      right = std::max(right, b);
  part2 += right - left;
  std::cout << "part1: " << part1 << "\npart2: " << part2 << '\n';
}
