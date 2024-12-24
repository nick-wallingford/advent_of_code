#include "util.hpp"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

template <> struct std::hash<std::pair<int, int>> {
  size_t operator()(std::pair<int, int> x) const noexcept {
    size_t h = x.first;
    h <<= 32;
    h |= x.second;
    __int128 h2 = h;
    h2 *= 0xdeadbeefdeadbeefllu;
    h2 += h2 >> 64;
    return h2;
  }
};

void aoc_2024_8() {
  std::ifstream file{get_filename(2024, 8)};
  std::vector<std::string> map;
  for (std::string line; std::getline(file, line);)
    if (!line.empty())
      map.push_back(std::move(line));

  std::unordered_set<std::pair<int, int>> antinodes;
  const ptrdiff_t width = map[0].size();
  const ptrdiff_t height = map.size();

  for (ptrdiff_t i = 0; i < height; i++)
    for (ptrdiff_t j = 0; j < width; j++) {
      const char station = map[i][j];
      if (station == '.')
        continue;

      for (ptrdiff_t k = 0; k < height; k++)
        for (ptrdiff_t l = 0; l < width; l++)
          if (map[k][l] == station && (k != i || l != j)) {
            ptrdiff_t y_step = k - i;
            ptrdiff_t x_step = l - j;
            for (ptrdiff_t x = j, y = i; 0 <= x && x < width && 0 <= y && y < height; x += x_step, y += y_step)
              antinodes.emplace(y, x);
          }
    }

  std::cout << antinodes.size() << '\n';
}
