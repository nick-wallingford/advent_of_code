#ifndef VEC_H
#define VEC_H

#include <cstdint>
#include <functional>
#include <ostream>

struct vec {
  int x{}, y{};

  constexpr vec() noexcept = default;
  constexpr vec(int x, int y) noexcept : x{x}, y{y} {}
  constexpr vec(const vec &) noexcept = default;
  constexpr vec &operator=(const vec &) noexcept = default;
  constexpr vec(vec &&) noexcept = default;
  constexpr vec &operator=(vec &&) noexcept = default;

  constexpr vec &operator+=(const vec &o) noexcept {
    x += o.x;
    y += o.y;
    return *this;
  }
  constexpr vec &operator-=(const vec &o) noexcept {
    x -= o.x;
    y -= o.y;
    return *this;
  }
  [[nodiscard]] constexpr vec operator+(const vec &o) const noexcept { return {x + o.x, y + o.y}; }
  [[nodiscard]] constexpr vec operator-(const vec &o) const noexcept { return {x - o.x, y - o.y}; }
  [[nodiscard]] constexpr bool operator==(const vec &o) const noexcept { return x == o.x && y == o.y; }
  [[nodiscard]] constexpr bool operator!=(const vec &o) const noexcept { return x != o.x || y != o.y; }
  [[nodiscard]] constexpr int manhattan_distance(const vec &o) const noexcept {
    vec v = *this - o;
    if (v.x < 0)
      v.x = -v.x;
    if (v.y < 0)
      v.y = -v.y;
    return v.x + v.y;
  }
  [[nodiscard]] constexpr int64_t operator%(const vec &o) const noexcept {
    return static_cast<int64_t>(x) * static_cast<int64_t>(o.x) + static_cast<int64_t>(y) * static_cast<int64_t>(o.y);
  }
};

template <> struct std::hash<vec> {
  using is_avalanching = std::true_type;

  [[nodiscard]] size_t operator()(const vec &v) const noexcept {
    __uint128_t h = (static_cast<uint64_t>(v.x) << 32) | static_cast<uint64_t>(v.y);
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }
};

std::ostream &operator<<(std::ostream &o, const vec &v);

#endif
