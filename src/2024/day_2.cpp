#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

static std::pair<bool, bool> is_safe(const std::string &s, int n) {
  int prev = -1;
  int inc = 0;

  bool safe = true;

  std::istringstream ss(s);
  bool cont = false;
  int i = 0;

  for (const auto x : std::views::istream<int>(ss)) {
    if (i++ == n) {
      cont = true;
      continue;
    }

    if (prev != -1) {
      if (inc == 0)
        inc = x > prev ? 1 : -1;

      const int diff = (x - prev) * inc;
      safe &= (0 < diff) & (diff <= 3);
    }

    prev = x;
  }

  return {safe, n == -1 || cont};
}

static bool is_safe(const std::string &s) {
  for (int i = -1;; i++) {
    const auto [safe, cont] = is_safe(s, i);
    if (safe)
      return true;
    if (!cont)
      break;
  }

  return false;
}

void aoc_2024_2() {
  std::ifstream file{"day_02.txt"};
  int safe_count = 0;

  for (std::string line; std::getline(file, line);) {
    const bool safe = is_safe(line);
    if (safe)
      std::cout << "  safe:" << line << '\n';
    else
      std::cout << "unsafe:" << line << '\n';
    safe_count += safe;
  }

  std::cout << safe_count << '\n';
}
