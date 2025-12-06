#include "util.hpp"
#include <fstream>
#include <iostream>
#include <regex>

void aoc_2025_5() {
  std::ifstream input{get_filename(2025, 5)};
  size_t part1{}, part2{};

  std::vector<std::pair<uint64_t, uint64_t>> fresh;
  std::vector<uint64_t> produce;

  std::regex regex{"([0-9]+)-([0-9]+)"};
  for (std::string line; std::getline(input, line);) {
    std::smatch results;
    if (!std::regex_match(line, results, regex))
      break;

    fresh.emplace_back(std::stoul(results[1].str()), std::stoul(results[2].str()));
  }

  for (std::string line; std::getline(input, line);)
    produce.emplace_back(std::stoul(line));

  std::sort(fresh.begin(), fresh.end());
  std::sort(produce.begin(), produce.end());

  auto f = fresh.cbegin();
  auto p = produce.cbegin();

  while (f != fresh.cend() && p != produce.cend()) {
    if (*p < f->first)
      ++p;
    else if (*p > f->second)
      ++f;
    else {
      ++part1;
      ++p;
    }
  }

  size_t left{}, right{};
  for (const auto [a, b] : fresh) {
    if (a > right) {
      part2 += 1 + right - left;
      left = a;
      right = b;
    } else {
      right = std::max(right, b);
    }
  }
  part2 += right - left;

  std::cout << "part1: " << part1 << "\npart2: " << part2 << '\n';
}
