#include "util.hpp"
#include <fstream>
#include <iostream>
#include <regex>

void aoc_2024_3() {
  std::ifstream file{get_filename(2024, 3)};
  std::regex pattern{R"((do(?:n't)?\(\))|(mul\((\d+),(\d+)\)))"};

  int sum = 0;
  bool enabled = true;

  for (std::string line; std::getline(file, line);) {
    if (std::cmatch match; std::regex_search(static_cast<const char *>(line.data()),
                                             static_cast<const char *>(line.data() + line.size()), match, pattern)) {
      do {
        if (match[0] == "do()")
          enabled = true;
        else if (match[0] == "don't()")
          enabled = false;
        else if (enabled)
          sum += stoi(match[3]) * stoi(match[4]);
      } while (std::regex_search(static_cast<const char *>(match[0].second),
                                 static_cast<const char *>(line.data() + line.size()), match, pattern));
    }
  }

  std::cout << sum << '\n';
}
