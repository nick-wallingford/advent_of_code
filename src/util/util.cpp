#include "util.hpp"

#include <fstream>
#include <iostream>
#include <pthread.h>
#include <pwd.h>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>

void pin_cpu_thread(unsigned t) {
  cpu_set_t cpuset;
  pthread_t thread = pthread_self();

  CPU_ZERO(&cpuset);
  CPU_SET(t, &cpuset);

  int s = pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);

  if (s) {
    std::cout << "pthread_setaffinity_np failed" << std::endl;
    exit(s);
  }
}

timer::~timer() {
  using namespace std::chrono;
  const steady_clock::time_point stop = steady_clock::now();
  if (const auto ms = duration_cast<milliseconds>(stop - start_time); ms.count() >= 1000)
    std::cout << "time: " << ms << std::endl;
  else if (const auto us = duration_cast<microseconds>(stop - start_time); us.count() >= 1000)
    std::cout << "time: " << us << std::endl;
  else
    std::cout << "time: " << duration_cast<nanoseconds>(stop - start_time) << std::endl;
}

std::string read_entire_file(const std::filesystem::path &s) {
  std::ostringstream sstr;
  sstr << std::ifstream{s}.rdbuf();
  return sstr.str();
}

std::filesystem::path get_filename(int year, int day, int part) {
  static const std::filesystem::path root = []() -> std::filesystem::path {
    passwd *pw = getpwuid(getuid());
    if (!pw) {
      std::cerr << "cannot find home directory\n";
      exit(-1);
    }

    std::filesystem::path ret{pw->pw_dir};
    ret /= ".local";
    if (!std::filesystem::exists(ret))
      std::cerr << ret << " does not exist\n";
    ret /= "share";
    if (!std::filesystem::exists(ret))
      std::cerr << ret << " does not exist\n";
    ret /= "advent_of_code";
    if (!std::filesystem::exists(ret))
      std::cerr << ret << " does not exist\n";
    return ret;
  }();

  std::filesystem::path ret = root;
  ret /= std::to_string(year);
  if (!std::filesystem::exists(ret))
    std::cerr << ret << " does not exist\n";
  ret /= std::to_string(day);
  if (part) {
    ret += "_";
    ret += std::to_string(part);
  }
  ret += ".txt";
  if (!std::filesystem::exists(ret))
    std::cerr << ret << " does not exist\n";
  return ret;
}
