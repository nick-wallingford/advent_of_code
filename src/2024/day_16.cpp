#include "maze.hpp"
#include "path.hpp"
#include "vec.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>

using path_ptr = path::path_ptr;

namespace {
class a_star {
  const maze m;
  std::priority_queue<path_ptr, std::vector<path_ptr>, path_priority> paths;
  std::unordered_map<path_ptr, int64_t, std::hash<path_ptr>, path_equals_hash>
      visited;
  std::vector<path_ptr> best_scores;

public:
  a_star(const char *f) : m{f} {
    paths.emplace(path::create(m));
    visited.emplace(paths.top(), 0);
  }

  int64_t go() {
    while (!paths.empty()) {
      path_ptr p{paths.top()};
      paths.pop();

      if (p->done()) {
        if (best_scores.empty()) {
          best_scores.push_back(p);
          continue;
        }
        const auto score = best_scores.front()->get_score();
        if (score > p->get_score())
          best_scores.clear();
        else if (score < p->get_score())
          break;
        best_scores.push_back(p);
        continue;
      }

      if (!best_scores.empty() &&
          p->get_heuristic() >= best_scores.front()->get_score())
        break;

      for (auto neighbor : {++*p, p->rot<false>(), p->rot<true>()})
        if (neighbor) {
          auto it = visited.find(neighbor);
          if (it == visited.end()) {
            visited[neighbor] = neighbor->get_score();
          } else {
            int64_t &s = it->second;
            if (s != 0) {
              if (neighbor->get_score() > s)
                continue;
              it->first->add_parent(p);
              if (neighbor->get_score() == s)
                continue;
            }
            s = neighbor->get_score();
          }
          paths.push(neighbor);
        }
    }
    if (best_scores.empty())
      return std::numeric_limits<int64_t>::max();
    else
      return best_scores.front()->get_score();
  }

  size_t part2() const {
    std::unordered_set<vec> nodes;
    for (std::unordered_set<path_ptr> paths{best_scores.begin(),
                                            best_scores.end()};
         !paths.empty();) {
      auto it = paths.begin();
      assert(it != paths.end());
      path_ptr p = *it;
      paths.erase(it);
      nodes.insert(p->get_position());
      for (const auto &p2 : p->get_parents())
        paths.insert(p2);
    }

    assert(nodes.contains(m.get_start()));
    assert(nodes.contains(m.get_end()));

    return nodes.size();
  }
};
} // namespace

void aoc_2024_16() {
  a_star p{"day_16.txt"};
  std::cout << "part 1: " << p.go() << std::endl;
  std::cout << "part 2: " << p.part2() << std::endl;
}
