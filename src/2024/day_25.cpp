#include "util.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

namespace {
class vec5 {
  std::array<int, 5> a{};

public:
  [[nodiscard]] constexpr int &operator[](size_t n) { return a[n]; }
  [[nodiscard]] constexpr int operator[](size_t n) const { return a[n]; }

  constexpr vec5 operator+(const vec5 &o) noexcept {
    vec5 r = *this;
    r += o;
    return r;
  }

  constexpr vec5 &operator+=(const vec5 &o) noexcept {
    for (size_t i = 0; i < 5; i++)
      a[i] += o[i];
    return *this;
  }

  constexpr void clear() noexcept {
    for (auto &i : a)
      i = 0;
  }
};

std::ostream &operator<<(std::ostream &o, const vec5 &v) {
  std::cout << '{';
  for (size_t i = 0; i < 5; i++)
    if (i)
      std::cout << ',' << v[i];
    else
      std::cout << v[i];
  std::cout << '}';
  return o;
}
} // namespace

void aoc_2024_25() {
  std::string line;
  std::vector<vec5> keys;
  std::vector<vec5> locks;
  bool iskey = false;
  vec5 current;
  int state = 0;

  for (std::ifstream file{get_filename(2024, 25)}; std::getline(file, line);)
    switch (state++) {
    case 0:
      assert(line.size() == 5);
      iskey = line[0] == '.';
      current.clear();
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      for (size_t i = 0; i < 5; i++)
        current[i] += line[i] == '#';
      break;
    case 6:
      assert(line.size() == 5);
      if (iskey)
        keys.push_back(current);
      else
        locks.push_back(current);
      current.clear();
      break;
    case 7:
      assert(line.empty());
      state = 0;
      break;
    }

  int64_t overlap = 0;
  for (const auto &key : keys)
    for (auto lock : locks) {
      lock += key;
      for (size_t i = 0; i < 5; i++) {
        if (lock[i] > 5) {
          ++overlap;
          break;
        }
      }
    }

  std::cout << "part 1: " << locks.size() * keys.size() - overlap << std::endl;
}
