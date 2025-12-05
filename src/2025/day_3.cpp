#include "util.hpp"
#include <fstream>
#include <iostream>

static int64_t pow10(char x) {
  int64_t ret{1};
  while (x--)
    ret *= 10;
  return ret;
}

static int64_t best_joltage(std::string_view s, const int64_t current, const int64_t begin_index, char N) {
  if (!N--)
    return current;
  int8_t best_current{-1};
  const int64_t power = pow10(N);
  int64_t best = 0;

  for (size_t i = begin_index; i < s.size() - N; i++) {
    uint8_t c = s[i];
    if (c <= best_current)
      continue;
    best_current = c;
    c ^= '0';
    if (const int64_t x = best_joltage(s, current + c * power, i + 1, N); x > best)
      best = x;
  }

  return best;
}

void aoc_2025_3() {
  std::ifstream input{get_filename(2025, 3)};
  int64_t part1{}, part2{};
  for (std::string line; std::getline(input, line);) {
    part1 += best_joltage(line, 0, 0, 2);
    part2 += best_joltage(line, 0, 0, 12);
  }

  std::cout << "part1: " << part1 << "\npart2: " << part2 << '\n';
}
