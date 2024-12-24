#include "maze.hpp"

#include <cassert>
#include <fstream>
#include <queue>

map::map(const std::filesystem::path &filename) {
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
}

std::ostream &map::print(std::ostream &o) const {
  for (int i = 0; i < sz.y; i++)
    o << std::string_view{data.data() + i * sz.x, static_cast<size_t>(sz.x)} << '\n';
  return o;
}

maze::maze(const std::filesystem::path &filename) : map{filename} {
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

std::unordered_map<vec, int64_t> maze::flood(const vec initial) const {
  std::unordered_map<vec, int64_t> ret;
  ret[initial] = 0;

  std::queue<vec> queue;
  queue.push(initial);

  while (!queue.empty()) {
    const vec v = queue.front();
    queue.pop();
    const int64_t cost = ret[v] + 1;

    static constexpr std::array<vec, 4> directions{vec{1, 0}, vec{0, -1}, vec{-1, 0}, vec{0, 1}};
    for (vec d : directions) {
      if ((*this)[d += v] != '#' && ret.try_emplace(d, cost).second)
        queue.push(d);
    }
  }

  return ret;
}
