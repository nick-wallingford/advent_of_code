#include "util.hpp"
#include "maze.hpp"
#include <iostream>

#if 1
constexpr int cheat_distance = 2; // part 1
#else
constexpr int cheat_distance = 20; // part 2
#endif

void aoc_2024_20() {
  const maze m{get_data(2024, 20)};
  const auto flood_start = m.flood(m.get_start()); // map of vec -> distance from start
  const auto flood_end = m.flood(m.get_end());     // map of vec -> distance from end
  const int64_t max_cheat = flood_start.find(m.get_end())->second - 100;
  int64_t count = 0;

  for (const auto [v, partial_score] : flood_start)           // iterate over every potential cheat start poin
    if (partial_score <= max_cheat)                           // ignore cases where we can't save time
      for (int y = -cheat_distance; y <= cheat_distance; y++) // iterate over our search diamond vertically
        for (int xd = cheat_distance - std::abs(y), x = -xd; x <= xd;
             x++) // iterate over our search diamond horizontally
          if (const auto it = flood_end.find(v + vec{x, y}); it != flood_end.end()) // lookup cost to finish the maze
            count += partial_score + it->second + std::abs(x) + std::abs(y) <=
                     max_cheat; // check whether we've saved any time

  std::cout << count << std::endl;
}
