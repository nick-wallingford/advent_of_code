#include <fstream>
#include <iostream>
#include <vector>

static uint64_t concat_integer(uint64_t a, uint32_t b) {
  for (uint32_t x = b; x; x /= 10)
    a *= 10;
  return a + b;
}

static bool is_valid(const uint32_t *const v, const uint64_t value,
                     const uint64_t target, const size_t n) {
  if (n)
    return is_valid(v + 1, value * *v, target, n - 1) ||
           is_valid(v + 1, value + *v, target, n - 1) ||
           is_valid(v + 1, concat_integer(value, *v), target, n - 1);
  else
    return value == target;
}

void aoc_2024_7() {
  std::ifstream file{"day_07.txt"};
  std::vector<uint32_t> v;
  uint64_t sum = 0;

  for (std::string line; std::getline(file, line); v.clear()) {
    const char *p = line.c_str();
    uint64_t target = 0;
    for (; *p != ':'; ++p) {
      target *= 10;
      target += *p - '0';
    }

    p += 2;
    for (uint32_t x = 0;; ++p) {
      if (const uint8_t c = *p - '0'; c < 10) {
        x *= 10;
        x += c;
      } else {
        v.push_back(x);
        if (*p)
          x = 0;
        else
          break;
      }
    }

    if (is_valid(v.data() + 1, v.front(), target, v.size() - 1))
      sum += target;
  }

  std::cout << sum << '\n';
}
