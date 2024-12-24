#include "util.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace {

class explorer {
  std::vector<std::string> map;
  int x, y;
  char c;
  int size_x, size_y;

public:
  explorer(const std::filesystem::path &f) {
    std::ifstream file{f};
    for (std::string line; std::getline(file, line);)
      map.push_back(std::move(line));
    size_y = map.size() - 1;
    size_x = map[0].size() - 1;

    for (size_t i = 0; i < map.size(); i++)
      for (size_t j = 0; j < map[0].size(); j++)
        if (c = map[i][j]; c == '^' || c == 'v' || c == '<' || c == '>') {
          y = i;
          x = j;
          return;
        }

    throw std::exception{};
  }
  explorer(const explorer &) = default;
  explorer(explorer &&) = default;

  size_t count() const {
    size_t count = 0;
    for (const auto &s : map)
      count += std::count(s.begin(), s.end(), 'X');
    return count;
  }

  bool operator++() {
    map[y][x] = 'X';
    for (;;) {
      if (c == '^') {
        if (y == 0)
          return false;
        if (map[y - 1][x] == '#') {
          c = '>';
          continue;
        } else {
          y--;
          return true;
        }
      } else if (c == '>') {
        if (x == size_x)
          return false;
        if (map[y][x + 1] == '#') {
          c = 'v';
          continue;
        } else {
          x++;
          return true;
        }
      } else if (c == 'v') {
        if (y == size_y)
          return false;
        if (map[y + 1][x] == '#') {
          c = '<';
          continue;
        } else {
          y++;
          return true;
        }
      } else if (c == '<') {
        if (x == 0)
          return false;
        if (map[y][x - 1] == '#') {
          c = '^';
          continue;
        } else {
          x--;
          return true;
        }
      }
    }
  }

  [[nodiscard]] constexpr int get_width() const noexcept { return size_x; }
  [[nodiscard]] constexpr int get_height() const noexcept { return size_y; }
  [[nodiscard]] constexpr int get_x() const noexcept { return x; }
  [[nodiscard]] constexpr int get_y() const noexcept { return y; }
  [[nodiscard]] constexpr int get_c() const noexcept {
    switch (c) {
    default:
    case '^':
      return 0;
    case '>':
      return 1;
    case 'v':
      return 2;
    case '<':
      return 3;
    }
  }

  bool block(int a, int b) {
    if (a == x && b == y)
      return false;
    map[b][a] = '#';
    return true;
  }

  [[nodiscard]] char operator()(size_t i, size_t j) const { return map.at(i).at(j); }

  bool operator==(const explorer &o) const { return x == o.x && y == o.y && c == o.c; }

  static bool loops(explorer e1, explorer e2) {
    while (++e1 && ++e1 && ++e2)
      if (e1 == e2)
        return true;

    return false;
  }

  int part2() const {
    int count = 0;
    const int i_end = map.size();
    const int j_end = map[0].size();
    for (int i = 0; i < i_end; i++) {
      for (int j = 0; j < j_end; j++) {
        if (i == y && j == x)
          continue;
        explorer e1 = *this;
        e1.block(j, i);
        explorer e2 = e1;
        if (loops(std::move(e1), std::move(e2)))
          ++count;
      }
    }
    return count;
  }
};

class explorer_opt {
  const int width, height;
  int x, y;
  int dir;

  std::vector<std::vector<char>> rows;
  std::vector<std::vector<char>> cols;

public:
  explorer_opt(const explorer &e)
      : width{e.get_width()}, height{e.get_height()}, x{e.get_x()}, y{e.get_y()}, dir{e.get_c()} {
    rows.resize(height);
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
        if ('#' == e(i, j))
          rows[i].push_back(j);

    cols.resize(width);
    for (int j = 0; j < width; j++)
      for (int i = 0; i < height; i++)
        if ('#' == e(i, j))
          cols[j].push_back(i);
  }
  explorer_opt(const explorer_opt &) = default;
};
} // namespace

void aoc_2024_6() {
  const explorer e{get_data(2024, 6)};
  std::cout << e.part2() << '\n';
}
