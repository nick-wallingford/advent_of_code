#pragma once

#include <chrono>
#include <string>
#include <filesystem>

void pin_cpu_thread(unsigned t);
struct timer {
  const std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
  ~timer();
};

std::string read_entire_file(const char *);

// this is currently hardcoded to ~/.local/share/advent_of_code/<year>/<day>[_<part>].txt
// probably one day I'll make this configurable maybe.
// maybe not.
std::filesystem::path get_data(int year, int day, int part = 0);
