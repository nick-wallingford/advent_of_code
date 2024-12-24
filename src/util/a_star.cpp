#include "a_star.hpp"

#include <iostream>
#include <queue>

[[nodiscard]] int64_t a_star::get_g_score(const vec &v) const {
  if (auto it = g_score.find(v); it != g_score.end())
    return it->second;
  return std::numeric_limits<int64_t>::max();
}

[[nodiscard]] int64_t a_star::get_f_score(const vec &v) const {
  if (auto it = g_score.find(v); it != g_score.end())
    return it->second + heuristic(v);
  return std::numeric_limits<int64_t>::max();
}

a_star::a_star(const maze &m) : m{m} {
  std::priority_queue<vec, std::vector<vec>, comperator> open_set{
      comperator{*this}};
  open_set.emplace(m.get_start());
  g_score[m.get_start()] = 0;

  while (!open_set.empty()) {
    const vec current = open_set.top();
    open_set.pop();
    const int64_t current_score = get_g_score(current);

    for (const vec d : {vec{1, 0}, vec{0, -1}, vec{-1, 0}, vec{0, 1}})
      if (const vec neighbor = current + d; neighbor == m.get_end()) {
        came_from.emplace(neighbor, current);
        success = true;
        return;
      } else if (const int64_t score = current_score + 1;
                 m[neighbor] == '.' && score < get_g_score(neighbor)) {
        came_from.emplace(neighbor, current);
        g_score.emplace(neighbor, score);
        open_set.emplace(neighbor);
      }
  }
}

void a_star::debug() const {
  for (const auto [to, from] : came_from)
    std::cout << to << ' ' << from << '\n';
}

std::vector<vec> a_star::get_path() const {
  std::vector<vec> r;
  vec current = m.get_end();
  r.push_back(current);

  for (auto it = came_from.find(current); it != came_from.end();
       it = came_from.find(current)) {
    current = it->second;
    r.push_back(current);
  }
  std::reverse(r.begin(), r.end());
  return r;
}

int64_t a_star::get_path_length() const {
  int64_t r = 0;
  for (auto it = came_from.find(m.get_end()); it != came_from.end();
       it = came_from.find(it->second))
    ++r;
  return r;
}
