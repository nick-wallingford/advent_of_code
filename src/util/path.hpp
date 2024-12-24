#pragma once

#include "maze.hpp"
#include "vec.hpp"

#include <cassert>
#include <memory>

class path : public std::enable_shared_from_this<path> {
public:
  using path_ptr = std::shared_ptr<path>;

private:
  struct Private {
    explicit Private() = default;
  };

  const maze &m;
  vec p{m.get_start()};
  int dir{};
  int64_t score{};
  int64_t heuristic{};
  mutable std::vector<path_ptr> parents;

  static constexpr std::array<vec, 4> dlookup{vec{1, 0}, vec{0, -1}, vec{-1, 0},
                                              vec{0, 1}};

  void update_heuristic() noexcept {
    int64_t h = p.manhattan_distance(m.get_end());
    if (h) {
      const vec v = m.get_end() - p;
      const int64_t dot = dlookup[dir] % v;
      if (dot != h)
        h += dot < 0 ? 2000 : 1000;
    }
    heuristic = h + score;
  }

public:
  constexpr path(Private, path &o) noexcept
      : m{o.m}, p{o.p}, dir{o.dir}, score{o.score}, heuristic{o.heuristic} {
    parents.push_back(o.get_ptr());
  }
  constexpr path(Private, const maze &m) noexcept : m{m} { update_heuristic(); }

  static path_ptr create(const maze &m) {
    return std::make_shared<path>(Private{}, m);
  }

#if 0
  ~path() {
    std::vector<path_ptr> freelist = std::move(parents);
    while (!freelist.empty()) {
      path_ptr par = freelist.back();
      freelist.pop_back();
      for (auto pare : par->parents)
        freelist.push_back(pare);
    }
  }
#endif

  path_ptr get_ptr() { return shared_from_this(); }

  [[nodiscard]] path_ptr operator++() {
    if (m[p + dlookup[dir]] == '#')
      return {};
    path_ptr r{std::make_shared<path>(Private{}, *this)};
    ++r->score;
    r->p += dlookup[dir];
    r->update_heuristic();
    return r;
  }

  template <bool d> [[nodiscard]] path_ptr rot() {
    path_ptr r{std::make_shared<path>(Private{}, *this)};
    if constexpr (d) {
      if (++r->dir == 4)
        r->dir = 0;
    } else {
      if (--r->dir == -1)
        r->dir = 3;
    }
    assert(r->dir >= 0);
    assert(r->dir < 4);
    if (m[p + dlookup[r->dir]] == '#')
      return {};
    else {
      r->score += 1000;
      r->update_heuristic();
    }
    return r;
  }

  [[nodiscard]] constexpr const std::vector<path_ptr> &
  get_parents() const noexcept {
    return parents;
  }

  [[nodiscard]] constexpr bool done() const noexcept {
    return p == m.get_end();
  }

  [[nodiscard]] size_t hash() const noexcept {
    __uint128_t h = std::hash<vec>{}(p);
    h += dir;
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }

  void add_parent(const path_ptr &parent) const {
    parents.push_back(parent);
  }

  [[nodiscard]] constexpr int get_dir() const noexcept { return dir; }
  [[nodiscard]] constexpr const vec &get_position() const noexcept { return p; }
  [[nodiscard]] constexpr int get_score() const noexcept { return score; }
  [[nodiscard]] constexpr int64_t get_heuristic() const noexcept {
    return heuristic;
  }
};

struct path_priority {
  [[nodiscard]] constexpr bool operator()(const path::path_ptr &a,
                                          const path::path_ptr &b) const noexcept {
    return a->get_heuristic() > b->get_heuristic();
  }
};

struct path_equals_hash {
  [[nodiscard]] constexpr bool operator()(const path::path_ptr &a,
                                          const path::path_ptr &b) const noexcept {
    return a->get_position() == b->get_position() &&
           a->get_dir() == b->get_dir();
  }
};

template <> struct std::hash<path::path_ptr> {
  [[nodiscard]] size_t operator()(const path::path_ptr &p) const noexcept {
    return p->hash();
  }
};
