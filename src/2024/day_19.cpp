#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace {
class trie {
  const trie &root;
  std::array<std::unique_ptr<trie>, 26> children;
  bool term = false;

  mutable std::unordered_map<std::string_view, int64_t> cache;

public:
  trie() : root{*this} {}
  trie(const trie &root) : root{root} {}

  void insert(const char *s) {
    if (!*s) {
      term = true;
      return;
    }

    std::unique_ptr<trie> &child = children[*s - 'a'];
    if (!child)
      child = std::make_unique<trie>(root);
    child->insert(s + 1);
  }

  int64_t operator()(std::string_view s) const {
    if (s.empty())
      return term;
    auto [it, ins] = cache.try_emplace(s, 0);
    if (!ins)
      return it->second;
    if (term)
      it->second += root(s);
    if (const trie *child = children[s.front() - 'a'].get(); child) {
      s.remove_prefix(1);
      it->second += (*child)(s);
    }
    return it->second;
  }
};

std::vector<std::string> read_towels(std::ifstream &file) {
  std::vector<std::string> r;
  if (std::string line; std::getline(file, line)) {
    std::stringstream ss{line};
    for (std::string token; std::getline(ss, token, ' ');) {
      if (token.back() == ',')
        token.pop_back();
      r.push_back(std::move(token));
    }
  }
  return r;
}

std::vector<std::string> read_patterns(std::ifstream &file) {
  std::vector<std::string> r;
  for (std::string line; std::getline(file, line);)
    if (!line.empty())
      r.push_back(std::move(line));
  return r;
}

} // namespace

void aoc_2024_19() {
  std::ifstream file{"day_19.txt"};
  trie t;
  for (const auto &towel : read_towels(file))
    t.insert(towel.c_str());
  int64_t count = 0;
  for (const auto &pattern : read_patterns(file))
    count += t(pattern);
  std::cout << count << std::endl;
}
