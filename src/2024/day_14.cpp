#include "util.hpp"
#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

static constexpr int64_t width = 101;
static constexpr int64_t height = 103;
static constexpr int64_t duration = 40000;

namespace {
void part1() {
  int64_t x, y, dx, dy;
  int64_t quad[4] = {0, 0, 0, 0};
  static constexpr int64_t mid_x = width >> 1;
  static constexpr int64_t mid_y = height >> 1;
  const auto file = fopen(get_filename(2024, 14).c_str(), "r");

  while (4 == fscanf(file, "p=%" PRId64 ",%" PRId64 " v=%" PRId64 ",%" PRId64 "\n", &x, &y, &dx, &dy)) {
    x += duration * dx;
    x %= width;
    if (x < 0)
      x += width;
    y += duration * dy;
    y %= height;
    if (y < 0)
      y += height;

    if (x < mid_x) {
      if (y < mid_y)
        quad[0]++;
      else if (y > mid_y)
        quad[1]++;
    } else if (x > mid_x) {
      if (y < mid_y)
        quad[2]++;
      else if (y > mid_y)
        quad[3]++;
    }
  }

  fclose(file);
  printf("safety factor: %" PRId64 "\n", quad[0] * quad[1] * quad[2] * quad[3]);
}

void part2() {
  FILE *file = fopen(get_filename(2024, 14).c_str(), "r");
  int64_t x, y, dx, dy;

  int64_t n = 0;
  int64_t sum_x[duration];
  int64_t sum_sqr_x[duration];
  int64_t sum_y[duration];
  int64_t sum_sqr_y[duration];
  memset(sum_x, 0, sizeof(sum_x));
  memset(sum_sqr_x, 0, sizeof(sum_sqr_x));
  memset(sum_y, 0, sizeof(sum_y));
  memset(sum_sqr_y, 0, sizeof(sum_sqr_y));

  while (4 == fscanf(file, "p=%" PRId64 ",%" PRId64 " v=%" PRId64 ",%" PRId64 "\n", &x, &y, &dx, &dy)) {
    ++n;
    for (int64_t i = 0; i < duration; i++) {
      sum_x[i] += x;
      sum_y[i] += y;
      sum_sqr_x[i] += x * x;
      sum_sqr_y[i] += y * y;

      x += dx;
      while (x < 0)
        x += width;
      while (x >= width)
        x -= width;
      y += dy;
      while (y < 0)
        y += height;
      while (y >= height)
        y -= height;
    }
  }

  fclose(file);

  const double N = 1.0 / n;
  const double N2 = 1.0 / (n * n);
  for (int64_t i = 0; i < duration; i++) {
    const double stddev_x = sqrt(sum_sqr_x[i] * N - sum_x[i] * sum_x[i] * N2);
    const double stddev_y = sqrt(sum_sqr_y[i] * N - sum_y[i] * sum_y[i] * N2);
    if (stddev_x + stddev_y < 40)
      printf("%ld\t%16f\t%16f\n", i, stddev_x, stddev_y);
  }
}

} // namespace

void aoc_2024_14() {
  part1();
  part2();
}
