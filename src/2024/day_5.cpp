#include "util.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

struct pairless {
  const std::unordered_set<std::pair<int, int>> &pairs;

  pairless(const std::unordered_set<std::pair<int, int>> &pairs) : pairs{pairs} {}
  bool operator()(const int a, const int b) const { return pairs.end() != pairs.find(std::pair<int, int>{a, b}); }
};

void aoc_2024_5() {
  std::ifstream file{get_data(2024, 5)};
  std::string line;
  std::unordered_set<std::pair<int, int>> orders;
  while (std::getline(file, line) && !line.empty()) {
    int a, b;
    std::from_chars(line.data(), line.data() + 2, a);
    std::from_chars(line.data() + 3, line.data() + 5, b);
    orders.emplace(a, b);
  }

  std::string s;
  int sum = 0;
  pairless cmp{orders};

  for (std::vector<int> v; std::getline(file, line); v.clear()) {
    for (std::stringstream ss{line}; std::getline(ss, s, ',');) {
      int x;
      std::from_chars(s.data(), s.data() + s.size(), x);
      v.push_back(x);
    }

    bool correct = true;
    for (size_t i = v.size(); correct && --i;)
      for (size_t j = i; j--;)
        if (orders.end() != orders.find({v[i], v[j]})) {
          correct = false;
          break;
        }

    if (!correct) {
      std::stable_sort(v.begin(), v.end(), cmp);
      sum += v[v.size() >> 1];
    }
  }

  std::cout << sum << std::endl;
}
