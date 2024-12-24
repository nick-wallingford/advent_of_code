#include "util.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

namespace {
class map {
  int width{0}, height{0};
  std::string data;
  int x{-1}, y{-1};

  char o(int i, int j) const { return data[i * width + j]; }
  char &o(int i, int j) { return data[i * width + j]; }

  void moveup() {
    char c = o(y - 1, x);
    if (c == '#')
      return;
    else if (c == '.') {
      y--;
      return;
    }
    assert(c == 'O');

    int i = y - 1;
    for (; 'O' == (c = o(i, x)); i--)
      ;
    if (c == '#')
      return;

    assert(c == '.');
    o(i, x) = 'O';
    o(--y, x) = '.';
  }
  void movedown() {
    char c = o(y + 1, x);
    if (c == '#')
      return;
    else if (c == '.') {
      y++;
      return;
    }
    assert(c == 'O');

    int i = y + 1;
    for (; 'O' == (c = o(i, x)); i++)
      ;
    if (c == '#')
      return;

    assert(c == '.');
    o(i, x) = 'O';
    o(++y, x) = '.';
  }
  void moveleft() {
    char c = o(y, x - 1);
    if (c == '#')
      return;
    else if (c == '.') {
      x--;
      return;
    }
    assert(c == 'O');

    int i = x - 1;
    for (; 'O' == (c = o(y, i)); i--)
      ;
    if (c == '#')
      return;

    assert(c == '.');
    o(y, i) = 'O';
    o(y, --x) = '.';
  }
  void moveright() {
    char c = o(y, x + 1);
    if (c == '#')
      return;
    else if (c == '.') {
      x++;
      return;
    }
    assert(c == 'O');

    int i = x + 1;
    for (; 'O' == (c = o(y, i)); i++)
      ;
    if (c == '#')
      return;

    assert(c == '.');
    o(y, i) = 'O';
    o(y, ++x) = '.';
  }

public:
  map(std::ifstream &file) {
    for (std::string line; std::getline(file, line);) {
      if (line.empty()) {
        const auto r = data.find('@');
        if (r == data.npos)
          throw std::exception{};
        x = r % width;
        y = r / width;
        assert(o(y, x) == '@');
        o(y, x) = '.';
        return;
      } else if (width) {
        if (width != static_cast<int>(line.size()))
          throw std::exception{};
        data.append(line);
        ++height;
      } else {
        width = line.size();
        data = std::move(line);
        ++height;
      }
    }
  }

  void print() {
    for (int i = 0; i < height; i++) {
      std::cout << std::string_view{data.data() + i * width, data.data() + i * width + width} << std::endl;
    }
  }

  void move(char c) {
    switch (c) {
    case '^':
      moveup();
      break;
    case '>':
      moveright();
      break;
    case 'v':
      movedown();
      break;
    case '<':
      moveleft();
      break;
    default:
      assert(false);
      break;
    }
  }

  [[nodiscard]] int64_t score() const noexcept {
    int64_t score = 0;
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
        if (const char c = o(i, j); c == 'O')
          score += 100 * i + j;
    return score;
  }
};

class map2 {
  int width{0}, height{0};
  std::string data;
  int x{-1}, y{-1};

  char o(int i, int j) const { return data[i * width + j]; }
  char &o(int i, int j) { return data[i * width + j]; }

  template <int dir> bool can_move_obstacles(int i, int j) const {
    static_assert(dir == 1 || dir == -1);
    if (const char c = o(i, j); c == '.')
      return true;
    else if (c == '#')
      return false;
    else if (!can_move_obstacles<dir>(i + dir, j))
      return false;
    else if (c == '[')
      return can_move_obstacles<dir>(i + dir, j + 1);
    else
      return can_move_obstacles<dir>(i + dir, j - 1);
  }

  template <int dir> void move_obstacles(int i, int j) {
    assert(can_move_obstacles<dir>(i, j));
    const char c = o(i, j);
    if (c == '.')
      return;
    move_obstacles<dir>(i + dir, j);
    if (c == '[') {
      move_obstacles<dir>(i + dir, j + 1);
      o(i + dir, j) = '[';
      o(i + dir, j + 1) = ']';
      o(i, j) = '.';
      o(i, j + 1) = '.';
    } else {
      move_obstacles<dir>(i + dir, j - 1);
      o(i + dir, j - 1) = '[';
      o(i + dir, j) = ']';
      o(i, j - 1) = '.';
      o(i, j) = '.';
    }
  }

  template <int dir> void move_ud() {
    if (!can_move_obstacles<dir>(y + dir, x))
      return;
    move_obstacles<dir>(y + dir, x);
    y += dir;
  }

  void moveup() { move_ud<-1>(); }
  void movedown() { move_ud<1>(); }
  void moveleft() {
    char c = o(y, x - 1);
    if (c == '#')
      return;
    else if (c == '.') {
      x--;
      return;
    }
    assert(c == ']');
    int i = x - 3;
    for (; ']' == (c = o(y, i)); i -= 2)
      ;
    if (c == '#')
      return;
    assert(c == '.');
    std::memmove(data.data() + y * width + i, data.data() + y * width + i + 1, x - i);
    --x;
  }
  void moveright() {
    char c = o(y, x + 1);
    if (c == '#')
      return;
    else if (c == '.') {
      x++;
      return;
    }
    assert(c == '[');
    int i = x + 3;
    for (; '[' == (c = o(y, i)); i += 2)
      ;
    if (c == '#')
      return;
    assert(c == '.');
    std::memmove(data.data() + y * width + x + 1, data.data() + y * width + x, i - x);
    ++x;
  }

public:
  map2(std::ifstream &file) {
    for (std::string line; std::getline(file, line);) {
      if (line.empty()) {
        if (x == -1 || y == -1)
          throw std::exception{};
        return;
      }

      if (!width)
        width = 2 * line.size();
      else if (width != 2 * static_cast<int>(line.size()))
        throw std::exception{};

      for (const char c : line)
        switch (c) {
        case '@':
          x = data.size() % width;
          y = height;
          [[fallthrough]];
        case '.':
          data.push_back('.');
          data.push_back('.');
          break;
        case 'O':
          data.push_back('[');
          data.push_back(']');
          break;
        case '#':
          data.push_back('#');
          data.push_back('#');
        }
      ++height;
    }
  }

  int64_t score() const noexcept {
    int64_t score = 0;
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
        if (const char c = o(i, j); c == '[')
          score += 100 * i + j;
    return score;
  }

  void move(const char c) {
    switch (c) {
    case '^':
      moveup();
      break;
    case '>':
      moveright();
      break;
    case 'v':
      movedown();
      break;
    case '<':
      moveleft();
      break;
    default:
      assert(false);
      break;
    }
  }
  void print() {
    for (int i = 0; i < height; i++) {
      std::cout << std::string_view{data.data() + i * width, data.data() + i * width + width} << std::endl;
    }
  }
};
} // namespace

void aoc_2024_15() {
  std::ifstream file{get_data(2024, 15)};
  map2 m{file};
  for (std::string line; std::getline(file, line);)
    for (const char c : line)
      m.move(c);
  std::cout << m.score() << '\n';
}
