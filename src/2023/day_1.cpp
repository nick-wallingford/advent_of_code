#include "util.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>

void aoc_2023_1() {
#if 1
  std::ifstream input{get_filename(2023, 1)};
#else
#if 0
  const std::string data{R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)"};
#else

  const std::string data{R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)"};
#endif

  std::istringstream input{data};
#endif

  const std::regex regex{
      "((?:one)|(?:two)|(?:three)|(?:four)|(?:five)|(?:six)|(?:seven)|(?:eight)|(?:nine)|[0-9])",
  };

  int64_t part1{}, part2{};

  for (std::string line; std::getline(input, line);) {
    int64_t digit1{}, digit2{};
    for (char c : line)
      if ('1' <= c && c <= '9') {
        c ^= '0';
        if (!digit1)
          digit1 = c;
        digit2 = c;
      }
    part1 += digit1 * 10 + digit2;

    digit1 = 0;
    const std::unordered_map<std::string, int64_t> lookup{
        {"one", 1},   {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6},
        {"seven", 7}, {"eight", 8}, {"nine", 9},  {"1", 1},    {"2", 2},    {"3", 3},
        {"4", 4},     {"5", 5},     {"6", 6},     {"7", 7},    {"8", 8},    {"9", 9},
    };

    std::smatch match;
    for (auto it = line.cbegin(); std::regex_search(it, line.cend(), match, regex);) {
      const auto s = match[0].str();
      digit2 = lookup.find(s)->second;
      if (!digit1)
        digit1 = digit2;
      it += match.position() + 1;
    }
    part2 += digit1 * 10 + digit2;
  }

  std::cout << "part 1: " << part1 << "\npart 2: " << part2 << '\n';
}
