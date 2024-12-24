#include "util.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>

namespace {
class secret {
  uint64_t n;
  constexpr void mix(uint64_t y) noexcept { n ^= y; }
  constexpr void prune() noexcept { n &= 16777215; }

public:
  constexpr secret(uint32_t n) noexcept : n{n} {}
  constexpr unsigned operator()() noexcept {
    mix(n << 6);
    prune();
    mix(n >> 5);
    prune();
    mix(n << 11);
    prune();
    return n % 10;
  }

  [[nodiscard]] constexpr uint64_t operator*() const noexcept { return n; }
};

std::vector<unsigned> read_ints(const std::filesystem::path &s) {
  std::vector<unsigned> r;
  uint32_t i;
  for (std::ifstream file{s}; file >> i; r.push_back(i))
    ;
  return r;
}

void run_sim(const std::vector<uint32_t> &inits) {
#if 1
  using map = boost::unordered::unordered_flat_map<uint32_t, uint8_t>;
#else
  using map = std::unordered_map<uint32_t, uint8_t>;
#endif
  std::unordered_set<uint32_t> sequences;
  std::vector<map> prices;
  prices.resize(inits.size());

  timer t;

  int64_t part1 = 0;
  for (size_t i = 0; i < inits.size(); i++) {
    map &price = prices[i];
    price.reserve(2000);

    secret s{inits[i]};
    int8_t prev = *s % 10;
    uint32_t seq{0};

    for (size_t j = 0; j < 2000; j++) {
      const uint8_t k = s();
      const int8_t diff = k - prev;
      seq <<= 8;
      seq |= static_cast<uint8_t>(diff);
      if (j >= 4) {
        sequences.emplace(seq);
        price.try_emplace(seq, k);
      }
      prev = k;
    }
    part1 += *s;
  }

  std::cout << "part 1: " << part1 << std::endl;

  int64_t best_bananas{-10000000000};
  for (const uint32_t seq : sequences) {
    int64_t bananas = 0;
    for (const auto &price : prices)
      if (auto it = price.find(seq); it != price.end())
        bananas += it->second;
    if (bananas > best_bananas)
      best_bananas = bananas;
  }
  std::cout << "part 2: " << best_bananas << std::endl;
}

} // namespace

void aoc_2024_22() {
  const std::vector<uint32_t> inits = read_ints(get_filename(2024, 22));
  run_sim(inits);
}
