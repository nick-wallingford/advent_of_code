#pragma once

#include "maze.hpp"
#include "vec.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include <queue>

class a_star {
  class comperator {
    const a_star &as;

  public:
    comperator(const a_star &a) : as{a} {}
    [[nodiscard]] bool operator()(const vec &a, const vec &b) const {
      return as.get_f_score(b) < as.get_f_score(a);
    }
  };

  const maze &m;

  boost::unordered_flat_map<vec, vec, std::hash<vec>> came_from;
  boost::unordered_flat_map<vec, int64_t, std::hash<vec>> g_score;

  [[nodiscard]] constexpr int64_t heuristic(const vec &v) const {
    return v.manhattan_distance(m.get_end());
  }

  [[nodiscard]] int64_t get_g_score(const vec &v) const;
  [[nodiscard]] int64_t get_f_score(const vec &v) const;
  bool success = false;

public:
  a_star(const maze &m);

  void debug() const;

  [[nodiscard]] constexpr operator bool() const noexcept { return success; }

  std::vector<vec> get_path() const;
  int64_t get_path_length() const;
};
