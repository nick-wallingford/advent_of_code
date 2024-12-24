#pragma once

#include <cstddef>
#include <functional>

template <> struct std::hash<std::pair<size_t, size_t>> {
  [[nodiscard]] size_t
  operator()(const std::pair<size_t, size_t> &x) const noexcept {
    __int128 h = x.first;
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    h &= 0xffffffffffffffffllu;
    h += x.second;
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }
};
