#include "util.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
template <typename T> class mdspan_3 {
  const size_t a, b, c;
  std::unique_ptr<T[]> d = std::make_unique<T[]>(a * b * c);
  int max_id = 0;

  void init_region(size_t i, size_t j, int id, int value) {
    if (i == a || j == b || i == std::numeric_limits<size_t>::max() || j == std::numeric_limits<size_t>::max() ||
        (*this)(i, j, 1) || value != (*this)(i, j, 0))
      return;
    (*this)(i, j, 1) = id;
    init_region(i - 1, j, id, value);
    init_region(i, j - 1, id, value);
    init_region(i + 1, j, id, value);
    init_region(i, j + 1, id, value);
  }

public:
  mdspan_3(size_t a, size_t b, size_t c) : a{a}, b{b}, c{c} {}

  [[nodiscard]] T &operator()(size_t i, size_t j, size_t k) noexcept {
    assert(i < a);
    assert(j < b);
    assert(k < c);
    return d[k + c * (j + b * i)];
  }

  [[nodiscard]] T operator()(size_t i, size_t j, size_t k) const noexcept {
    assert(i < a);
    assert(j < b);
    assert(k < c);
    return d[k + c * (j + b * i)];
  }

  [[nodiscard]] constexpr size_t height() const noexcept { return a; }
  [[nodiscard]] constexpr size_t width() const noexcept { return b; }
  [[nodiscard]] constexpr size_t depth() const noexcept { return c; }

  void init_values(const std::vector<std::string> &map) {
    if (map.size() != a)
      throw std::exception{};

    for (size_t i = 0; i < a; i++) {
      if (map[i].size() != b)
        throw std::exception{};

      for (size_t j = 0; j < b; j++) {
        (*this)(i, j, 0) = map[i][j];
        for (size_t k = c; --k;)
          (*this)(i, j, k) = 0;
      }
    }
  }

  void init_walls() {
    for (size_t i = 0; i < a; i++)
      for (size_t j = 0; j < b; j++) {
        const int r = (*this)(i, j, 1);
        const bool w1 = i == 0 || (*this)(i - 1, j, 1) != r;
        const bool w2 = j == 0 || (*this)(i, j - 1, 1) != r;
        const bool w3 = i == a - 1 || (*this)(i + 1, j, 1) != r;
        const bool w4 = j == b - 1 || (*this)(i, j + 1, 1) != r;

        int &p = (*this)(i, j, 2);
        p += w1;
        p += w2;
        p += w3;
        p += w4;

        int &c = (*this)(i, j, 3);
        c += w1 && w2;
        c += w2 && w3;
        c += w3 && w4;
        c += w4 && w1;

        c += !w1 && !w2 && r != (*this)(i - 1, j - 1, 1);
        c += !w2 && !w3 && r != (*this)(i + 1, j - 1, 1);
        c += !w3 && !w4 && r != (*this)(i + 1, j + 1, 1);
        c += !w4 && !w1 && r != (*this)(i - 1, j + 1, 1);
      }
  }

  void init_regions() {
    for (size_t i = 0; i < a; i++)
      for (size_t j = 0; j < b; j++)
        if (!(*this)(i, j, 1))
          init_region(i, j, ++max_id, (*this)(i, j, 0));
  }

  std::vector<std::tuple<int, int, int>> find_area_perimeter_corners() const {
    std::vector<std::tuple<int, int, int>> r{static_cast<size_t>(max_id) + 1};
    for (size_t i = 0; i < a; i++)
      for (size_t j = 0; j < b; j++) {
        auto &[area, perimeter, corners] = r[(*this)(i, j, 1)];
        ++area;
        perimeter += (*this)(i, j, 2);
        corners += (*this)(i, j, 3);
      }
    return r;
  }
};

mdspan_3<int> read(const std::filesystem::path &f) {
  std::vector<std::string> map;
  std::string s;
  for (std::ifstream file{f}; std::getline(file, s); map.push_back(s))
    ;
  mdspan_3<int> r{map.size(), map[0].size(), 4};
  r.init_values(map);
  r.init_regions();
  r.init_walls();

  return r;
}
} // namespace

void aoc_2024_12() {
  const auto map = read(get_filename(2024, 12));
  const auto area_perimeter = map.find_area_perimeter_corners();
  int64_t cost = 0;
  for (const auto &[area, perimeter, corners] : area_perimeter)
#if 1
    cost += area * perimeter;
#else
    cost += area * corners;
#endif
  std::cout << cost << std::endl;
}
