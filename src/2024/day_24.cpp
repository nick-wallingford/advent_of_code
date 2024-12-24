#include "util.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <unordered_map>
#include <unordered_set>

using map = std::map<std::string, bool>;
using it = map::iterator;

namespace {
enum class op { AND, XOR, OR };
struct rule {
  std::string a, b, res;
  op o;
  bool applied = false;

  bool apply(bool m, bool n) const {
    switch (o) {
    case op::AND:
      return m & n;
    case op::OR:
      return m | n;
    case op::XOR:
      return m ^ n;
    default:
      assert(false);
      __builtin_unreachable();
    }
  }

  rule(std::string &&a, std::string &&b, std::string &&r, op o) noexcept
      : a{std::move(a)}, b{std::move(b)}, res{std::move(r)}, o{o} {}
  rule(const rule &) = default;
  rule &operator=(const rule &) = default;
  rule(rule &&) noexcept = default;
  rule &operator=(rule &&) noexcept = default;

  bool operator()(map &c) const {
    if (applied || c.contains(res))
      return false;
    auto it_a = c.find(a);
    if (it_a == c.end())
      return false;
    auto it_b = c.find(b);
    if (it_b == c.end())
      return false;
    c.emplace(res, apply(it_a->second, it_b->second));
    return true;
  }
};

op get_op(const std::ssub_match &m) {
  using std::literals::string_view_literals::operator""sv;
  if (m == "XOR")
    return op::XOR;
  else if (m == "OR")
    return op::OR;
  else if (m == "AND")
    return op::AND;

  std::cerr << m << '\n';
  assert(false);
  __builtin_unreachable();
}
} // namespace

void aoc_2024_24() {
  const std::regex constant_parser{"([a-z0-9]+): ([01])"};
  const std::regex rule_parser{"([a-z0-9]+) (AND|OR|XOR) ([a-z0-9]+) -> ([a-z0-9]+)"};

  map constants;
  std::vector<rule> rules;
  std::unordered_set<std::string> variables;

  std::string line;
  for (std::ifstream file{get_filename(2024, 24)}; std::getline(file, line);)
    if (std::smatch result; std::regex_match(line, result, constant_parser))
      constants.emplace(result[1], *result[2].first == '1');
    else if (std::regex_match(line, result, rule_parser)) {
      rules.emplace_back(result[1], result[3], result[4], get_op(result[2]));
      variables.emplace(result[4]);
    }

  while (!variables.empty())
    for (const auto &r : rules)
      if (r(constants))
        variables.erase(r.res);

  int64_t result = 0;
  for (auto it = constants.crbegin(); it != constants.crend(); ++it)
    if (it->first.front() != 'z')
      break;
    else {
      result <<= 1;
      result |= it->second;
    }
  std::cout << "part 1: " << result << '\n';
}
