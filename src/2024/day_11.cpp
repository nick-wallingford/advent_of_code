#include "util.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using visited_value = std::pair<int8_t, int64_t>;
using visited_set = std::unordered_map<visited_value, int64_t>;

template <> struct std::hash<visited_value> {
  size_t operator()(const visited_value &x) const noexcept {
    __int128 h = (static_cast<uint64_t>(x.first) << 56) + static_cast<uint64_t>(x.second);
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }
};

namespace {
std::vector<int64_t> read(const std::filesystem::path &f) {
  int64_t x = 0;
  std::ifstream file{f};
  std::string line;
  std::getline(file, line);
  std::vector<int64_t> v;
  for (const char c : line) {
    if (c == ' ') {
      v.push_back(x);
      x = 0;
    } else {
      x *= 10;
      x += c - '0';
    }
  }
  v.push_back(x);
  return v;
}

constexpr int64_t digit_count(int64_t x) {
  if (x < 10)
    return 1;
  else if (x < 100)
    return 2;
  else if (x < 1000)
    return 3;
  else if (x < 10000)
    return 4;
  else if (x < 100000)
    return 5;
  else if (x < 1000000)
    return 6;
  else if (x < 10000000)
    return 7;
  else if (x < 100000000)
    return 8;
  else if (x < 1000000000)
    return 9;
  else if (x < 10000000000)
    return 10;
  else if (x < 100000000000)
    return 11;
  else if (x < 1000000000000)
    return 12;
  else if (x < 10000000000000)
    return 13;
  else if (x < 100000000000000)
    return 14;
  else if (x < 1000000000000000)
    return 15;
  else if (x < 10000000000000000)
    return 16;
  else if (x < 100000000000000000)
    return 17;
  else
    return 18;
}

constexpr int64_t pow10(int x) {
  int64_t r = 1;
  for (; x--; r *= 10)
    ;
  return r;
}

int64_t count_stones(const int64_t x, const int8_t depth, visited_set &v) {
  int64_t r;
  if (depth == 0)
    return 1;
  else if (auto it = v.find({depth, x}); it != v.end())
    return it->second;
  else if (x == 0)
    r = count_stones(1, depth - 1, v);
  else if (const int count = digit_count(x); count & 1)
    r = count_stones(x * 2024, depth - 1, v);
  else {
    const int64_t s = pow10(count >> 1);
    r = count_stones(x / s, depth - 1, v) + count_stones(x % s, depth - 1, v);
  }
  v.emplace(std::piecewise_construct, std::forward_as_tuple(depth, x), std::forward_as_tuple(r));
  return r;
}
} // namespace

void aoc_2024_11() {
  int64_t c = 0;
  visited_set v;
  for (int64_t s : read(get_data(2024, 11)))
    c += count_stones(s, 75, v);

  std::cout << c << '\n';
}
