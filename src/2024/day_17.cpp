#include "util.hpp"
#include <atomic>
#include <boost/parser/parser.hpp>
#include <cassert>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#define ADV 0
#define BXL 1
#define BST 2
#define JNZ 3
#define BXC 4
#define OUT 5
#define BDV 6
#define CDV 7

namespace {
class computer {
  const std::vector<int8_t> &program;
  int64_t A, B, C;
  int64_t IP{0};
  constexpr int64_t get_combo(int literal) const noexcept {
    switch (literal) {
    default:
      assert(false);
    case 0:
    case 1:
    case 2:
    case 3:
      return literal;
    case 4:
      return A;
    case 5:
      return B;
    case 6:
      return C;
    }
  }

public:
  computer(const std::vector<int8_t> &prog, int64_t a, int64_t b, int64_t c) : program{prog}, A{a}, B{b}, C{c} {}

  void reset(int64_t a) {
    A = a;
    B = 0;
    C = 0;
    IP = 0;
  }

  bool run(std::ostream *o) {
    size_t count = 0;
    for (;;) {
      if (IP < 0 || IP > static_cast<int64_t>(program.size()) - 1) {
        if (o)
          *o << '\n';
        return count == program.size();
      }

      const int opcode = program[IP];
      const int literal = program[IP + 1];

      switch (opcode) {
      case ADV:
        A >>= get_combo(literal);
        break;
      case BXL:
        B ^= literal;
        break;
      case BST:
        B = get_combo(literal) & 7;
        break;
      case JNZ:
        if (!A)
          break;
        IP = literal;
        continue;
      case BXC:
        B ^= C;
        break;
      case OUT:
        if (const int output = get_combo(literal) & 7; o) {
          if (count)
            *o << ',';
          *o << output;
        } else {
          if (count >= program.size() || program[count] != output)
            return false;
        }

        ++count;
        break;
      case BDV:
        B = A >> get_combo(literal);
        break;
      case CDV:
        C = A >> get_combo(literal);
        break;
      }

      IP += 2;
    }
  }
};

template <bool do_output> bool decompiled_is_quine(int64_t a) {
  int64_t b = 0;
  int64_t c = 0;
  size_t count = 0;

  static constexpr std::array<int8_t, 16> prog{2, 4,  // bst
                                               1, 2,  // bxl
                                               7, 5,  // cdv
                                               4, 1,  // bxc
                                               1, 3,  // bxl
                                               5, 5,  // out
                                               0, 3,  // adv
                                               3, 0}; // jnz

  do {
    b = a & 7;
    b ^= 2;
    c = a >> b;
    b ^= c;
    b ^= 3;
    if constexpr (do_output) {
      if (count)
        std::cout << ',';
      std::cout << (b & 7);
    } else {
      if (count >= prog.size() || (b & 7) != prog[count])
        return false;
    }
    ++count;
    a >>= 3;
  } while (a);
  if constexpr (do_output)
    std::cout << std::endl;
  return count == prog.size();
}

std::vector<int8_t> day_17_prog(int64_t a) {
  int64_t b = 0;
  int64_t c = 0;
  std::vector<int8_t> r;
  r.reserve(16);

  do {
    b = a & 7;
    b ^= 2;
    c = a >> b;
    b ^= c;
    b ^= 3;
    r.push_back(b & 7);
    a >>= 3;
  } while (a);
  return r;
}

#define LITERAL (int64_t)(program[ip + 1])
#define COMBO (LITERAL == 4 ? a : LITERAL == 5 ? b : LITERAL == 6 ? c : LITERAL)
#define DISPATCH()                                                                                                     \
  do {                                                                                                                 \
    goto *dispatch_table[program[ip += 2]];                                                                            \
  } while (false)

template <bool do_output> bool day_17_run_program(int64_t a, const int8_t *__restrict program, size_t program_size) {
  int64_t b = 0;
  int64_t c = 0;
  size_t ip = 0;
  size_t count = 0;

  static const void *dispatch_table[] = {&&adv, &&bxl, &&bst, &&jnz, &&bxc, &&out, &&bdv, &&cdv, &&halt};
  goto *dispatch_table[*program];
adv:
  a >>= COMBO;
  DISPATCH();
bxl:
  b ^= LITERAL;
  DISPATCH();
bst:
  b = COMBO & 7;
  DISPATCH();
jnz:
  if (a)
    goto *dispatch_table[program[ip = LITERAL]];
  DISPATCH();
bxc:
  b ^= c;
  DISPATCH();
out:
  if constexpr (do_output)
    if (count)
      std::cout << ',' << (COMBO & 7);
    else
      std::cout << (COMBO & 7);
  else if (count >= program_size || program[count] != (COMBO & 7))
    return false;
  ++count;
  DISPATCH();
bdv:
  b = a >> COMBO;
  DISPATCH();
cdv:
  c = a >> COMBO;
  DISPATCH();
halt:
  if constexpr (do_output)
    std::cout << std::endl;
  return count == program_size;
}

void brute_force(std::vector<int8_t> program_vec) {
  static std::atomic<int64_t> N = 0;
  static std::mutex output_mutex;

  std::vector<std::future<void>> futs;
  constexpr int block_size = 32;

  const size_t prog_length = program_vec.size();
  program_vec.push_back(8);
  program_vec.push_back(8);
  for (; program_vec.size() <= 11; program_vec.push_back(8))
    ;

  assert(!day_17_run_program<false>(37221261688307, program_vec.data(), prog_length));
  assert(day_17_run_program<false>(37221261688308, program_vec.data(), prog_length));

  for (auto i = std::thread::hardware_concurrency(); i--;)
    futs.push_back(std::async([program_vec{program_vec.data()}, prog_length, i]() {
      pin_cpu_thread(i);

      const auto start_time = std::chrono::steady_clock::now();

      for (;;) {
        int64_t n = N++;
        n <<= block_size;
        for (int64_t i = n; i < n + (1ll << block_size); i++) {
          if (!(i & (i - 1))) [[unlikely]] {
            std::lock_guard lock{output_mutex};
            const auto current_time = std::chrono::steady_clock::now();
            std::cout << "status:\t\t" << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time)
                      << ' ' << i << '\t' << std::flush;
            day_17_run_program<true>(i, program_vec, prog_length);
          }
          if (day_17_run_program<false>(i, program_vec, prog_length)) [[unlikely]] {
            std::lock_guard lock{output_mutex};
            const auto current_time = std::chrono::steady_clock::now();
            std::cout << "found result:\t"
                      << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time) << ' ' << i << '\t'
                      << std::flush;
            day_17_run_program<true>(i, program_vec, prog_length);
          }
        }
      }
    }));
}

bool knowledge(const int64_t A, int depth) {
  static constexpr std::array<int8_t, 16> prog{2, 4,  // bst
                                               1, 2,  // bxl
                                               7, 5,  // cdv
                                               4, 1,  // bxc
                                               1, 3,  // bxl
                                               5, 5,  // out
                                               0, 3,  // adv
                                               3, 0}; // jnz
  for (int64_t i = 0; i < 8; i++) {
    int64_t a = A;
    a |= i << 3 * depth;
    const auto v = day_17_prog(a);
#if 1
    for (size_t i = 0; i < v.size(); i++)
      if (i)
        std::cout << ',' << static_cast<uint16_t>(v[i]);
      else
        std::cout << static_cast<uint16_t>(v[i]);
    std::cout << '\n';
#endif

    if (depth < static_cast<int64_t>(v.size()) && v[depth] == prog[depth]) {
      if (depth) {
        if (!knowledge(a, depth - 1))
          return false;
      } else {
        std::cout << a << std::endl;
        return false;
      }
    }
  }
  return true;
}

} // namespace

void aoc_2024_17() {
#if 0
  knowledge(0, 15);
#else
  namespace bp = boost::parser;
  using bp::literals::operator""_l;
  const std::string input = read_entire_file(get_data(2024, 17));
  constexpr auto parser =
      // clang-format off
    "Register A:"_l >> bp::ulong_ >>
    "Register B:"_l >> bp::ulong_ >>
    "Register C:"_l >> bp::ulong_ >>
    "Program:"_l    >> bp::digit % ','
      // clang-format on
      ;

  const auto result = bp::parse(input, parser, bp::ws);
  if (!result) {
    std::cout << "Unable to parse input" << std::endl;
    std::cout << input << std::endl;
    return;
  }

  const auto &input_prog = std::get<3>(*result);
  std::vector<int8_t> prog;
  prog.reserve(input_prog.size());
  std::transform(input_prog.begin(), input_prog.end(), std::back_inserter(prog), [](const auto x) { return x - '0'; });
  std::cout << std::endl;

  brute_force(prog);

#endif
}
