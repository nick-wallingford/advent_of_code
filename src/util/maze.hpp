#ifndef MAZE_H
#define MAZE_H

#include "vec.hpp"
#include <filesystem>
#include <unordered_map>

class map {
protected:
  vec sz{0, 0};
  std::string data;

public:
  map(vec sz) : sz{sz} { data.resize(sz.x * sz.y); }
  map(const std::filesystem::path&);

  [[nodiscard]] constexpr char operator[](const vec &v) const noexcept { return data[v.y * sz.x + v.x]; }
  [[nodiscard]] constexpr char &operator[](const vec &v) noexcept { return data[v.y * sz.x + v.x]; }
  [[nodiscard]] constexpr const vec &size() const noexcept { return this->sz; }
  std::ostream &print(std::ostream &) const;
};

class maze : public map {
  vec start_point{-1, -1};
  vec end_point{-1, -1};

public:
  maze(vec sz) : map{sz} {}
  maze(const std::filesystem::path&);

  constexpr void set_start(const vec v) noexcept { start_point = v; }
  constexpr void set_end(const vec v) noexcept { end_point = v; }
  [[nodiscard]] constexpr const vec &get_start() const noexcept { return start_point; }
  [[nodiscard]] constexpr const vec &get_end() const noexcept { return end_point; }
  [[nodiscard]] std::unordered_map<vec, int64_t> flood(vec) const;
};

inline std::ostream &operator<<(std::ostream &o, const map &m) { return m.print(o); }

#endif
