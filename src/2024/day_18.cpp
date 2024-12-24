#include "a_star.hpp"
#include "maze.hpp"
#include "util.hpp"
#include "vec.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <queue>

#if 1
static constexpr int take_n = 1024;
static constexpr const vec sz{73, 73};
#else
static constexpr int take_n = 12;
static constexpr const vec sz{9, 9};
#endif

void aoc_2024_18() {
  maze m{sz};
  for (int x = 0; x < sz.x; x++) {
    m[vec{x, 0}] = '#';
    m[vec{x, sz.y - 1}] = '#';
  }
  for (int y = 0; y < sz.y; y++) {
    m[vec{0, y}] = '#';
    m[vec{sz.x - 1, y}] = '#';
  }
  for (int y = 1; y < sz.y - 1; y++)
    for (int x = 1; x < sz.x - 1; x++)
      m[vec{x, y}] = '.';
  m.set_start({1, 1});
  m.set_end(sz - vec{2, 2});

  FILE *f = fopen(get_filename(2024, 18).native().c_str(), "r");

#if 0
  // part 1
  for (int i = 0; i < take_n; i++) {
    vec v;
    if (2 != fscanf(f, "%d,%d\n", &v.x, &v.y))
      break;
    m[v + vec{1, 1}] = '#';
  }
  fclose(f);

  // std::cout << m;

  a_star a{m};
  const auto solution = a.get_path();
  std::cout << solution.size() - 1 << std::endl;
#elif 0
  for (int i = 0;; i++) {
    if (!(i & (i - 1)))
      std::cout << i << std::endl;
    vec v;
    if (2 != fscanf(f, "%d,%d\n", &v.x, &v.y))
      break;
    m[v + vec{1, 1}] = '#';
    if (!a_star{m}) {
      std::cout << v.x << ',' << v.y << std::endl;
      break;
    }
  }
#else
  std::vector<vec> blocks;
  for (vec v; 2 == fscanf(f, "%d,%d\n", &v.x, &v.y); blocks.push_back(v + vec{1, 1}))
    ;
  fclose(f);
  int l = 0;
  int r = blocks.size() - 1;
  while (l != r) {
    const int mid = (l + r + 1) >> 1;
    maze mid_maze{m};
    for (int i = l; i < mid; i++)
      mid_maze[blocks[i]] = '#';
    if (a_star{mid_maze}) {
      l = mid;
      m = std::move(mid_maze);
    } else {
      r = mid - 1;
    }
  }

  std::cout << blocks[l].x - 1 << ',' << blocks[l].y - 1 << std::endl;
#endif
}
