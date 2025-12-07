#include "util.hpp"
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

void aoc_2025_6() {
#if 1
  std::ifstream input{get_filename(2025, 6)};
#else
  const std::string data{R"(123 328  51 64
 45 64  387 23
  6 98  215 314
*   +   *   +)"};
  std::istringstream input{data};
#endif

  int64_t part1{}, part2{};

  std::vector<std::vector<std::string>> lines;
  std::vector<std::string> full_lines;

  for (std::string line; std::getline(input, line);) {
    std::istringstream line_stream{line};
    std::vector<std::string> line_vector;
    std::copy(std::istream_iterator<std::string>(line_stream), std::istream_iterator<std::string>(),
              std::back_inserter(line_vector));
    lines.push_back(std::move(line_vector));
    full_lines.push_back(std::move(line));
  }

  const std::vector<std::string> operations = std::move(lines.back());
  lines.pop_back();
  size_t size = operations.size();

  for (size_t i = 0; i < size; i++) {
    const bool mul = operations[i][0] == '*';
    size_t n = mul ? 1 : 0;
    for (auto &line : lines) {
      if (mul)
        n *= std::stoul(line[i]);
      else
        n += std::stoul(line[i]);
    }
    part1 += n;
  }

  size = full_lines.front().size();
  for (const auto &line : full_lines)
    size = std::max(size, line.size());
  ++size;
  for (auto &line : full_lines)
    while (line.size() < size)
      line.push_back(' ');

  bool is_mul;
  uint64_t n = 0;
  for (size_t i = 0; i < size; i++) {
    uint64_t o = 0;
    for (size_t j = 0; j < full_lines.size() - 1; j++) {
      if (full_lines[j][i] != ' ') {
        o *= 10;
        o += full_lines[j][i] ^ '0';
      }
    }
    if (!n) {
      n = o;
      is_mul = full_lines[full_lines.size() - 1][i] == '*';
    } else if (o) {
      if (is_mul)
        n *= o;
      else
        n += o;
    } else {
      part2 += n;
      n = 0;
    }
  }

  std::cout << "part 1: " << part1 << "\npart 2: " << part2 << '\n';
}
