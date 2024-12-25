#include "util.hpp"
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <unordered_map>
#include <unordered_set>

#define VISUAL_OUTPUT

using constants_map = std::map<std::string, bool>;
using it = constants_map::iterator;

namespace {
enum class op { NOP, AND, XOR, OR };

std::string op_to_string(const op &o) { return o == op::AND ? "AND" : o == op::XOR ? "XOR" : " OR"; }

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

  rule(std::string_view r) noexcept : res{r}, o{op::NOP} {}
  rule(std::string_view A, std::string_view B, op o) noexcept : a{A}, b{B}, o{o} {
    if (a > b)
      std::swap(a, b);
  }

  rule(std::string &&a, std::string &&b, std::string &&r, op o) noexcept
      : a{std::move(a)}, b{std::move(b)}, res{std::move(r)}, o{o} {
    if (this->a > this->b)
      std::swap(this->a, this->b);
  }
  rule(const rule &) = default;
  rule &operator=(const rule &) = default;
  rule(rule &&) noexcept = default;
  rule &operator=(rule &&) noexcept = default;

  bool operator()(constants_map &c) const {
    if (res.empty())
      throw std::exception{};
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

std::ostream &operator<<(std::ostream &o, const rule &r) {
  return o << r.a << ' ' << op_to_string(r.o) << ' ' << r.b << " -> " << r.res;
}

struct rule_eq_forward {
  [[nodiscard]] bool operator()(const rule &a, const rule &b) const noexcept {
    return a.a == b.a && a.b == b.b && a.o == b.o;
  }
};

struct rule_hash_forward {
  [[nodiscard]] size_t operator()(const rule &r) const noexcept {
    size_t h1 = static_cast<int>(r.o);
    for (const uint8_t c : r.a) {
      h1 <<= 8;
      h1 |= c;
    }
    for (const uint8_t c : r.b) {
      h1 <<= 8;
      h1 |= c;
    }
    __uint128_t h = h1;
    h *= 0xdeadbeefdeadbeefllu;
    h += h >> 64;
    return h;
  }
};

using rule_fwd = std::unordered_set<rule, rule_hash_forward, rule_eq_forward>;
using rule_fwd_it = rule_fwd::const_iterator;

op get_op(const std::ssub_match &m) {
  using enum op;
  if (m == "XOR")
    return XOR;
  else if (m == "OR")
    return OR;
  else if (m == "AND")
    return AND;

  std::cerr << m << '\n';
  assert(false);
  __builtin_unreachable();
}

[[nodiscard]] static constexpr std::string make_variable(char c, uint8_t n) noexcept {
  std::string r(3, c);
  r[1] = '0' | (n / 10);
  r[2] = '0' | (n % 10);

  return r;
}

class mismatch_fixer {
  struct rule_eq_backward {
    [[nodiscard]] bool operator()(const rule &a, const rule &b) const noexcept { return a.res == b.res; }
  };

  struct rule_hash_backward {
    [[nodiscard]] size_t operator()(const rule &r) const noexcept { return std::hash<std::string>{}(r.res); }
  };
  using rule_bwd = std::unordered_set<rule, rule_hash_backward, rule_eq_backward>;
  using rule_bwd_it = rule_fwd::const_iterator;

  rule_fwd rf;
  rule_bwd rb;

  std::pair<int, rule_bwd_it> find_backward(std::string_view start, std::string_view target) const {
    std::queue<std::pair<int, rule_bwd_it>> q;
    q.emplace(1, rb.find({start}));
    while (!q.empty()) {
      auto state = q.front();
      q.pop();

      if (state.second->a == target || state.second->b == target)
        return state;
      if (auto it = rb.find({state.second->a}); it != rb.end())
        q.emplace(state.first + 1, it);
      if (auto it = rb.find({state.second->b}); it != rb.end())
        q.emplace(state.first + 1, it);
    }
    return {std::numeric_limits<int>::max(), rb.end()};
  }

  std::vector<std::string> mismatches;

  void fix_carry(const rule &r, int n) {
    auto it1 = rb.find({r.a});
    auto it2 = rb.find({r.b});
    if (it2->a[0] == 'x')
      std::swap(it1, it2);
    if (it2->o != op::AND)
      mismatches.push_back(it2->res);
    std::cout << *it1 << ' ' << *it2 << std::endl;
  }

public:
  mismatch_fixer(rule_fwd &&rules) : rf{std::move(rules)} {
    for (const rule &r : rf)
      rb.insert(r);
    assert(rf.size() == rb.size());
  }

  void fix() {
    using enum op;
    for (int i = 1; i < 45; i++) {
      const std::string x = make_variable('x', i);
      const std::string y = make_variable('y', i);
      const std::string z = make_variable('z', i);

      auto z_it = rb.find({z});
      if (z_it->o != XOR) {
        std::cout << "1: " << *z_it << std::endl;
        mismatches.push_back(z);
      } else {
        auto it_xor = rf.find({x, y, XOR});
        if (it_xor->res != z_it->a && it_xor->res != z_it->b) {
          std::cout << "2: " << *it_xor << ' ' << *z_it << ' ' << std::endl;
          mismatches.push_back(it_xor->res);
        }

        if (auto it1 = rb.find({z_it->a}); it1->o == AND)
          mismatches.push_back(it1->res);
        else if (auto it2 = rb.find({z_it->b}); it2->o == AND)
          mismatches.push_back(it2->res);
        else {
          if (it2->o == XOR)
            std::swap(it1, it2);
          if (it2->o == XOR)
            mismatches.push_back(it2->res);
          else {
            assert(it1->a == x);
            assert(it1->b == y);
            fix_carry(*it2, i);
          }
        }
      }
    }

    std::sort(mismatches.begin(), mismatches.end());
    for (size_t i = 0; i < mismatches.size(); i++)
      if (i)
        std::cout << ',' << mismatches[i];
      else
        std::cout << mismatches[i];
    std::cout << std::endl;
  }
};

} // namespace

void aoc_2024_24() {
  const std::regex constant_parser{"([a-z0-9]+): ([01])"};
  const std::regex rule_parser{"([a-z0-9]+) (AND|OR|XOR) ([a-z0-9]+) -> ([a-z0-9]+)"};

  constants_map constants;
  rule_fwd rules;
  std::unordered_set<std::string> variables;

  std::string line;
  for (std::ifstream file{get_filename(2024, 24)}; std::getline(file, line);)
    if (std::smatch result; std::regex_match(line, result, constant_parser))
      constants.emplace(result[1], *result[2].first == '1');
    else if (std::regex_match(line, result, rule_parser)) {
      rules.emplace(result[1], result[3], result[4], get_op(result[2]));
      variables.emplace(result[4]);
    }

#ifdef VISUAL_OUTPUT
  std::cout << "digraph {\n";
  std::cout << "rankdir=LR;\n";
  for (int i = 1; i < 45; i++) {
    std::cout << make_variable('x', i) << " [shape=ellipse];\n";
    std::cout << make_variable('y', i) << " [shape=ellipse];\n";
  }
#endif

  while (!variables.empty())
    for (const auto &r : rules)
      if (r(constants)) {
        using enum op;
#ifdef VISUAL_OUTPUT
        std::string shape = r.o == XOR   ? " [shape=diamond];\n"
                            : r.o == AND ? " [shape=ellipse];\n"
                                         : " [shape=rectangle];\n";
        std::cout << r.res << shape;
        std::cout << r.a << " -> " << r.res << ";\n";
        std::cout << r.b << " -> " << r.res << ";\n";
#endif

        variables.erase(r.res);
      }

#ifdef VISUAL_OUTPUT
#if 1
  std::cout << "subgraph input {\n";
  std::cout << "rank=source;\n";
  for (int i = 0; i < 45; i++) {
    std::cout << make_variable('x', i) << ";\n";
    std::cout << make_variable('y', i) << ";\n";
  }
  std::cout << "}\n";

  std::cout << "subgraph output {\n";
  std::cout << "rank=sink;\n";
  for (int i = 0; i < 45; i++)
    std::cout << make_variable('z', i) << ";\n";
  std::cout << "}\n";

  std::cout << "subgraph carry {\n";
  std::cout << "rank=same;\n";
  for (const rule &r : rules)
    if (r.o == op::OR)
      std::cout << r.res << ";\n";
  std::cout << "}\n";
#endif
  std::cout << "}\n";

#else
  int64_t result = 0;
  for (auto it = constants.crbegin(); it != constants.crend(); ++it)
    if (it->first.front() != 'z')
      break;
    else {
      result <<= 1;
      result |= it->second;
    }

  std::cout << "part 1: " << result << '\n';

  mismatch_fixer fixer{std::move(rules)};
  fixer.fix();
#endif
}

// dqr,dtk,pfw,shh,vgs,z21,z33,z39
