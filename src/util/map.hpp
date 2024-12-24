#pragma once

#include "vec.hpp"

#include <cassert>
#include <ifstream

class map {
  vec sz{0, 0};
  vec start_point{-1, -1};
  vec end_point{-1, -1};
  std::string data;

public:
  map(const char *filename) {
    std::ifstream file{filename};
    for (std::string line; std::getline(file, line);) {
      if (!sz.x) {
        sz.x = line.size();
        data = std::move(line);
      } else {
        if (sz.x != static_cast<int>(line.size()))
          throw std::exception{};
        data.append(line);
      }
      ++sz.y;
    }

    if (const auto r = data.find('S'); r != std::string::npos) {
      start_point.x = r % sz.x;
      start_point.y = r / sz.x;
      assert('S' == (*this)[start_point]);
      data[r] = '.';
      assert('.' == (*this)[start_point]);
    } else
      throw std::exception{};
    if (const auto r = data.find('E'); r != std::string::npos) {
      end_point.x = r % sz.x;
      end_point.y = r / sz.x;
      assert('E' == (*this)[end_point]);
      data[r] = '.';
      assert('.' == (*this)[end_point]);
    } else
      throw std::exception{};
  }

  [[nodiscard]] constexpr char operator[](const vec &v) const noexcept {
    return data[v.y * sz.x + v.x];
  }

  [[nodiscard]] constexpr const vec &get_start() const noexcept {
    return start_point;
  }
  [[nodiscard]] constexpr const vec &get_end() const noexcept {
    return end_point;
  }
  [[nodiscard]] constexpr const vec &size() const noexcept { return this->sz; }
};
