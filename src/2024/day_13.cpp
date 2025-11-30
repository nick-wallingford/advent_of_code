#include "util.hpp"
#include <cstdint>
#include <cstdio>

void aoc_2024_13() {
  int64_t a, b, c, d, y1, y2;
  int64_t cost = 0;
  const auto filename = get_filename(2024, 13);
  FILE *file = fopen(filename.c_str(), "r");

  // input is formatted thusly:
  // a  c
  // b  d
  // y1 y2
  while (6 == fscanf(file,
                     "Button A: X%ld, Y%ld\n"
                     "Button B: X%ld, Y%ld\n"
                     "Prize: X=%ld, Y=%ld\n",
                     &a, &c, &b, &d, &y1, &y2)) {
#if 1
    y1 += 10000000000000ll;
    y2 += 10000000000000ll;
#endif

    // we solve the following linear equation:
    // [ a b | x1 ] = [ y1 ]
    // [ c d | x2 ] = [ y2 ]

    // [ x1 ] = [  d -b ] [ y1 ]
    // [ x2 ] = [ -c  a ] [ y2 ]
    //          ----------------
    //                det
    const int64_t det = a * d - b * c;
    int64_t x1 = d * y1 - b * y2;
    int64_t x2 = a * y2 - c * y1;

    if (!(x1 % det) && (x1 /= det) >= 0 && !(x2 % det) && (x2 /= det) >= 0)
      cost += 3 * x1 + x2;
  }

  fclose(file);
  printf("cost: %ld\n", cost);
}
