#include "util.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using visited_value = std::tuple<uint16_t, uint16_t>;
using visited_set = std::unordered_set<visited_value>;

template <> struct std::hash<visited_value> {
  size_t operator()(const visited_value &x) const noexcept {
    __int128 h = static_cast<uint64_t>(std::get<0>(x)) + (static_cast<uint64_t>(std::get<1>(x)) << 16);
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }
};

namespace {
int64_t dfs(const std::vector<std::string> &map, uint64_t y, uint64_t x, char c) {
  if (map[y][x] != c)
    return 0;
  if (c == '9')
    return 1;

  int64_t r = 0;

  ++c;
  if (y > 0)
    r += dfs(map, y - 1, x, c);
  if (y + 1 < map.size())
    r += dfs(map, y + 1, x, c);
  if (x > 0)
    r += dfs(map, y, x - 1, c);
  if (x < map[0].size())
    r += dfs(map, y, x + 1, c);
  return r;
}

void dfs(const std::vector<std::string> &map, visited_set &v, uint64_t y, uint64_t x, char c) {
  if (map[y][x] != c)
    return;
  if (c == '9') {
    v.emplace(y, x);
    return;
  }

  ++c;
  if (y > 0)
    dfs(map, v, y - 1, x, c);
  if (y + 1 < map.size())
    dfs(map, v, y + 1, x, c);
  if (x > 0)
    dfs(map, v, y, x - 1, c);
  if (x < map[0].size())
    dfs(map, v, y, x + 1, c);
}

[[maybe_unused]] void day_10_1() {
  std::ifstream file{get_data(2024, 10)};
  std::vector<std::string> map;

  for (std::string line; std::getline(file, line);)
    map.push_back(std::move(line));

  int64_t score = 0;
  for (size_t i = 0; i < map.size(); i++)
    for (size_t j = 0; j < map[0].size(); j++)
      if (map[i][j] == '0') {
        visited_set s;
        dfs(map, s, i, j, '0');
        score += s.size();
      }
  std::cout << score << '\n';
}

[[maybe_unused]] void day_10_2() {
  std::ifstream file{get_data(2024, 10)};
  std::vector<std::string> map;

  for (std::string line; std::getline(file, line);)
    map.push_back(std::move(line));

  int64_t score = 0;
  for (size_t i = 0; i < map.size(); i++)
    for (size_t j = 0; j < map[0].size(); j++)
      if (map[i][j] == '0') {
        int64_t x = dfs(map, i, j, '0');
        score += x;
      }
  std::cout << score << '\n';
}

} // namespace

void aoc_2024_10() { day_10_2(); }
