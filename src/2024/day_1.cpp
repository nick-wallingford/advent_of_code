#include "util.hpp"
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <regex>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace {
std::pair<std::vector<int>, std::vector<int>> get_lists() {
  std::ifstream f{get_data(2024, 1)};

  const std::regex pattern(R":((\d+)\s+(\d+)):", std::regex::optimize);
  std::vector<int> a, b;

  for (std::string line; std::getline(f, line);)
    if (std::cmatch m; std::regex_match(line.c_str(), m, pattern)) {
      int left, right;
      std::from_chars(m[1].first, m[1].second, left);
      std::from_chars(m[2].first, m[2].second, right);
      a.push_back(left);
      b.push_back(right);
    }
  return {std::move(a), std::move(b)};
}

void part1() {
  auto [a, b] = get_lists();

  std::sort(a.begin(), a.end());
  std::sort(b.begin(), b.end());

  unsigned sum = 0;
  for (const auto [x, y] : std::views::zip(a, b))
    sum += std::abs(x - y);

  std::cout << "day 1: " << sum << std::endl;
}

void part2() {
  auto [a, b] = get_lists();
  std::unordered_map<int, int> frequency;

  for (const auto x : b)
    ++frequency[x];

  size_t score = 0;
  for (const auto x : a)
    score += x * frequency[x];

  std::cout << "day 1: part 2: " << score << std::endl;
}
} // namespace

void aoc_2024_1() { part2(); }
