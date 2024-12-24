#include "util.hpp"
#include "vec.hpp"
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
class pad {
  static constexpr int lookup_x(char c) {
    switch (c) {
    case '7':
    case '4':
    case '1':
    case '<':
      return 0;
    case '8':
    case '5':
    case '2':
    case '0':
    case '^':
    case 'v':
      return 1;
    case '9':
    case '6':
    case '3':
    case 'A':
    case '>':
      return 2;
    default:
      std::cout << '{' << static_cast<int>(c) << '}' << std::endl;
      assert(false);
      __builtin_unreachable();
    }
  }

  static constexpr int lookup_y(char c) {
    switch (c) {
    case '7':
    case '8':
    case '9':
      return 3;
    case '4':
    case '5':
    case '6':
      return 2;
    case '1':
    case '2':
    case '3':
      return 1;
    case '0':
    case 'A':
    case '^':
      return 0;
    case '<':
    case 'v':
    case '>':
      return -1;
    default:
      std::cout << '{' << static_cast<int>(c) << '}' << std::endl;
      assert(false);
      __builtin_unreachable();
    }
  }

  static constexpr vec lookup(char c) { return {lookup_x(c), lookup_y(c)}; }

protected:
  vec position{2, 0};

  std::string get_pushes(char c) {
    const vec target = lookup(c);
    std::string s;
    const int x = target.x - position.x;
    const int y = target.y - position.y;

    const char y_chr = y < 0 ? 'v' : '^';
    const char x_chr = x < 0 ? '<' : '>';

    if ((x < 0 && !(position.y == 0 && target.x == 0)) || (position.x == 0 && target.y == 0)) {
      for (int i = std::abs(x); i--; s.push_back(x_chr))
        ;
      position.x = target.x;
      assert(position != vec(0, 0));
      for (int i = std::abs(y); i--; s.push_back(y_chr))
        ;
      position.y = target.y;
    } else {
      for (int i = std::abs(y); i--; s.push_back(y_chr))
        ;
      position.y = target.y;
      assert(position != vec(0, 0));
      for (int i = std::abs(x); i--; s.push_back(x_chr))
        ;
      position.x = target.x;
    }

    s.push_back('A');
    return s;
  }

public:
  virtual ~pad() noexcept = default;
  virtual int64_t push(char c) = 0;
};

struct hpad final : public pad {
  int64_t push(char) override { return 1; }
  [[nodiscard]] constexpr bool operator==(hpad) const noexcept { return true; }
  [[nodiscard]] constexpr size_t hash() const noexcept { return 0; }
};

template <size_t depth> struct dpad final : public pad {
  using child_t = std::conditional_t<static_cast<bool>(depth), dpad<depth - 1>, hpad>;
  child_t child;

  using state_t = std::pair<dpad, char>;
  using state_result = std::pair<dpad, int64_t>;
  struct state_hash {
    [[nodiscard]] size_t operator()(const state_t &p) const noexcept {
      __uint128_t h = p.first.hash();
      h *= p.second;
      h += h >> 64;
      return h;
    }
  };

  using umap = std::unordered_map<state_t, state_result, state_hash>;
  static inline umap memo;

public:
  int64_t push(char c) override {
    const state_t state{*this, c};

    if (auto it = memo.find(state); it != memo.end()) {
      *this = it->second.first;
      return it->second.second;
    }

    int64_t length = 0;
    for (char x : get_pushes(c))
      length += child.push(x);
    memo.emplace(std::piecewise_construct_t{}, std::forward_as_tuple(state), std::forward_as_tuple(*this, length));
    return length;
  }

  [[nodiscard]] constexpr bool operator==(const dpad &o) const noexcept {
    return position == o.position && child == o.child;
  }

  [[nodiscard]] size_t hash() const noexcept {
    __uint128_t h = std::hash<vec>{}(position);
    if constexpr (depth) {
      h *= child.hash();
      h += h >> 64;
    }
    return h;
  }
};

template <size_t depth> class kpad final : public pad {
  dpad<depth> child;

public:
  int64_t push(char c) override {
    int64_t length = 0;
    for (char x : get_pushes(c))
      length += child.push(x);

    return length;
  }
};

template <size_t robots> void run_sim(const std::vector<std::string> &lines) {
  timer t;
  int64_t total = 0;
  dpad<robots> pad;
  for (std::string_view line : lines) {
    int64_t n = 0;
    int64_t sz = 0;
    for (const char c : line) {
      if (c != 'A') {
        n *= 10;
        n += c - '0';
      }

      sz += pad.push(c);
    }
    std::cout << line << '\t' << n * sz << std::endl;
    total += n * sz;
  }
  std::cout << total << '\n' << std::endl;
}

} // namespace

void aoc_2024_21() {
  std::vector<std::string> lines;
  std::string line;
  for (std::ifstream f{get_data(2024, 21)}; std::getline(f, line); lines.push_back(std::move(line)))
    ;
  run_sim<2>(lines);
  run_sim<25>(lines);
}
