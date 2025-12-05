#include "util.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>

namespace {
constexpr uint8_t log10i(uint64_t x) {
  uint8_t n{1};
  if (x >= 1e32) {
    n += 32;
    x /= 1e32;
  }
  if (x >= 1e16) {
    n += 16;
    x /= 1e16;
  }
  if (x >= 1e8) {
    n += 8;
    x /= 1e8;
  }
  if (x >= 1e4) {
    n += 4;
    x /= 1e4;
  }
  if (x >= 1e2) {
    n += 2;
    x /= 1e2;
  }
  if (x >= 1e1) {
    n += 1;
    x /= 1e1;
  }
  return n;
}

static_assert(log10i(10) == 2);
static_assert(log10i(99) == 2);
static_assert(log10i(100) == 3);

static constexpr std::array<uint64_t, 20> powers_of_10 = []() {
  std::array<uint64_t, 20> ret;
  uint64_t x = 1;
  for (auto &y : ret) {
    y = x;
    x *= 10;
  }
  return ret;
}();

constexpr bool is_invalid(const uint64_t x, size_t idx) {
  const uint64_t f = powers_of_10[idx];
  const uint64_t m = x % f;
  if (log10i(m) < idx)
    return false;
  for (uint64_t y = x / f; y; y /= f)
    if (y % f != m)
      return false;

  return true;
}

} // namespace

void aoc_2025_2() {
#if 0
  const std::string data{R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
1698522-1698528,446443-446449,38593856-38593862,565653-565659,
824824821-824824827,2121212118-2121212124)"};
#else
  const std::string data = read_entire_file(get_filename(2025, 2));
#endif

  std::regex reg{"([0-9]+)-([0-9]+)"};
  std::sregex_iterator it{data.begin(), data.end(), reg}, it_end;
  size_t part1{}, part2{};

  for (; it != it_end; ++it) {
    uint64_t begin = std::stoul((*it)[1]);
    uint64_t end = std::stoul((*it)[2]);
    for (; begin <= end; ++begin) {
      const size_t digits = log10i(begin);
      const uint64_t group = powers_of_10[digits >> 1];

      // part1
      const uint64_t left = begin / group;
      const uint64_t right = begin % group;
      if (left == right)
        part1 += begin;

      // part2
      for (size_t i = 1; i <= digits >> 1; ++i) {
        if (is_invalid(begin, i)) {
          part2 += begin;
          break;
        }
      }
    }
  }

  std::cout << "part1: " << part1 << "\npart2: " << part2 << '\n';
}
