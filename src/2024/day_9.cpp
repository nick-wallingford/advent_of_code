#include "util.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace {
struct file {
  int index;
  int size;
  int id;

  constexpr file() = default;
  constexpr file(int index, int size, int id) : index{index}, size{size}, id{id} {}
  file(const file &) noexcept = default;
  file(file &&) noexcept = default;
  file &operator=(const file &) noexcept = default;
  file &operator=(file &&) noexcept = default;

  constexpr auto operator<=>(const file &o) const noexcept { return index <=> o.index; }
};

std::string read_file(const std::filesystem::path &s) {
  std::ifstream file{s};
  std::string line;
  std::getline(file, line);
  return line;
}

[[maybe_unused]] void day_09_part_1() {
  const auto v = read_file(get_data(2024, 9));
  int id = 0;
  std::vector<int> disk;
  for (size_t i = 0; i < v.size(); i++) {
    const int value = i & 1 ? -1 : id++;
    for (uint8_t j = v[i] - '0'; j--;)
      disk.push_back(value);
  }

  uint64_t checksum = 0;

  for (size_t i = 0; i < disk.size(); i++) {
    if (disk[i] == -1) {
      while (disk.back() == -1)
        disk.pop_back();
      if (i >= disk.size())
        break;
      disk[i] = disk.back();
      disk.pop_back();
    }
    checksum += i * disk[i];
  }
  std::cout << checksum << '\n';
}

[[maybe_unused]] void day_09_part_2() {
  const auto v = read_file(get_data(2024, 9));
  std::set<file> disk;
  std::vector<std::set<file>::iterator> files;

  int index = 0;
  for (size_t i = 0; i < v.size(); i++) {
    const int sz = v[i] - '0';
    if (!(i & 1)) {
      auto [it, emplaced] = disk.emplace(index, sz, i >> 1);
      files.emplace_back(it);
    }
    index += sz;
  }

  const auto find_gap = [&disk](int min_size, int max_idx) {
    return std::adjacent_find(disk.begin(), ++disk.find({max_idx, 0, 0}), [min_size](const file &a, const file &b) {
      return b.index - (a.index + a.size) >= min_size;
    });
  };

  for (; !files.empty(); files.pop_back()) {
    auto it_pop = files.back();
    auto it_push = find_gap(it_pop->size, it_pop->index);
    if (it_push != disk.end() && it_push->index < it_pop->index) {
      auto h = disk.extract(it_pop);
      h.value().index = it_push->index + it_push->size;
      disk.insert(std::move(h));
    }
  }

  int64_t checksum = 0;
  for (auto [idx, size, id] : disk)
    while (size--)
      checksum += id * idx++;
  std::cout << checksum << '\n';
}
} // namespace

void aoc_2024_9() { day_09_part_2(); }
