#include "util.hpp"
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

int search(const std::string_view s) {
  int count = 0;
  for (size_t i = 4; i <= s.size(); i++)
    if ((s[i - 4] == 'X' && s[i - 3] == 'M' && s[i - 2] == 'A' && s[i - 1] == 'S') ||
        (s[i - 4] == 'S' && s[i - 3] == 'A' && s[i - 2] == 'M' && s[i - 1] == 'X'))
      count++;
  return count;
}

int search_ul(const std::deque<std::string> &d) {
  int count = 0;
  for (size_t i = 4; i <= d.front().size(); i++)
    if ((d[0][i - 4] == 'X' && d[1][i - 3] == 'M' && d[2][i - 2] == 'A' && d[3][i - 1] == 'S') ||
        (d[0][i - 4] == 'S' && d[1][i - 3] == 'A' && d[2][i - 2] == 'M' && d[3][i - 1] == 'X'))
      count++;
  return count;
}

int search_ur(const std::deque<std::string> &d) {
  int count = 0;
  for (size_t i = 4; i <= d.front().size(); i++)
    if ((d[0][i - 1] == 'X' && d[1][i - 2] == 'M' && d[2][i - 3] == 'A' && d[3][i - 4] == 'S') ||
        (d[0][i - 1] == 'S' && d[1][i - 2] == 'A' && d[2][i - 3] == 'M' && d[3][i - 4] == 'X'))
      count++;
  return count;
}

int search_up(const std::deque<std::string> &d) {
  int count = 0;
  for (size_t i = 0; i < d.front().size(); i++)
    if ((d[0][i] == 'X' && d[1][i] == 'M' && d[2][i] == 'A' && d[3][i] == 'S') ||
        (d[0][i] == 'S' && d[1][i] == 'A' && d[2][i] == 'M' && d[3][i] == 'X'))
      count++;
  return count;
}

void day_04_p1() {
  std::deque<std::string> lines;

  std::ifstream file{get_data(2024, 4)};
  size_t linelength = 0;
  int count = 0;
  for (;;) {
    if (lines.size() == 4) {
      lines.push_back(std::move(lines.front()));
      lines.pop_front();
    } else
      lines.push_back({});

    if (!std::getline(file, lines.back()))
      break;

    if (lines.size() == 1)
      linelength = lines.back().size();
    else if (lines.back().size() != linelength) {
      std::cout << "can't handle mismatched lines" << std::endl;
      return;
    }

    count += search(lines.back());

    if (lines.size() == 4) {
      count += search_up(lines);
      count += search_ul(lines);
      count += search_ur(lines);
    }
  }

  std::cout << count << std::endl;
}

void day_04_p2() {
  std::deque<std::string> lines;
  std::ifstream file{get_data(2024, 4)};
  size_t linelength = 0;
  int count = 0;

  for (;;) {
    if (lines.size() == 3) {
      lines.push_back(std::move(lines.front()));
      lines.pop_front();
    } else
      lines.push_back({});

    if (!std::getline(file, lines.back()))
      break;

    if (lines.size() == 1)
      linelength = lines.back().size();
    else if (lines.back().size() != linelength) {
      std::cout << "can't handle mismatched lines" << std::endl;
      return;
    }

    if (lines.size() == 3)
      for (size_t i = 2; i < linelength; i++)
        if (lines[1][i - 1] == 'A' && lines[0][i - 2] != lines[2][i] && lines[0][i] != lines[2][i - 2] &&
            (lines[0][i] == 'M' || lines[0][i] == 'S') && (lines[0][i - 2] == 'M' || lines[0][i - 2] == 'S') &&
            (lines[2][i] == 'M' || lines[2][i] == 'S') && (lines[2][i - 2] == 'M' || lines[2][i - 2] == 'S'))
          count++;
  }
  std::cout << count << std::endl;
}

void aoc_2024_4() { day_04_p2(); }
