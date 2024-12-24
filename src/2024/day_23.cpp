#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>;

namespace {

constexpr uint32_t s_to_i(const char *s) {
  uint32_t r = static_cast<uint8_t>(*s);
  r |= static_cast<uint8_t>(s[1]) << 8;
  return r;
}
std::string i_to_s(const uint16_t i) {
  std::string s;
  s.resize(2);
  s[0] = i & 255;
  s[1] = i >> 8;
  return s;
}

struct clique_visitor {
  static inline std::vector<std::string> longest_clique;

  void clique(const auto &c, const Graph &) {
    if (c.size() > longest_clique.size()) {
      longest_clique.clear();
      for (size_t i = 0; i < c.size(); i++)
        longest_clique.push_back(i_to_s(c[i]));
    }
  }

  static void report_longest() {
    std::cout << "part 2:\n";
    std::sort(longest_clique.begin(), longest_clique.end());
    int count = 0;
    for (const auto &n : longest_clique) {
      if (count++)
        std::cout << ',' << n;
      else
        std::cout << n;
    }
    std::cout << '\n';
  }
};

} // namespace

void aoc_2024_23() {
  const auto start_time = std::chrono::steady_clock::now();
  Graph g;
  std::ifstream file{"day_23.txt"};

  auto lines_view = std::views::istream<std::string>(file);
  for (const auto &line : lines_view)
    if (line.size() == 5)
      boost::add_edge(s_to_i(line.data()), s_to_i(line.data() + 3), g);

  boost::bron_kerbosch_all_cliques(g, clique_visitor{});
  clique_visitor::report_longest();
  const auto stop_time = std::chrono::steady_clock::now();
  std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time) << std::endl;
}
