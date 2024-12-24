#include "vec.hpp"

#include <ostream>

std::ostream &operator<<(std::ostream &o, const vec &v) {
  return o << '(' << v.x << ',' << v.y << ')';
}
