#include <iostream>

void aoc_2015(int);
void aoc_2016(int);
void aoc_2017(int);
void aoc_2018(int);
void aoc_2019(int);
void aoc_2020(int);
void aoc_2021(int);
void aoc_2022(int);
void aoc_2023(int);
void aoc_2024(int);

static void usage(std::string_view argv) {
  std::cout << "usage: " << argv << "[year] <days>\n"
            << " for instance, " << argv
            << " 2024 3 7 2015 9 12\n"
               "to run problems 3 and 7 from 2024 and 9 and 12 from 2015.\n";
}

int main(const int argc, const char **argv) {
  if (argc <= 1)
    usage(*argv);

  void (*current_year)(int) = &aoc_2024;

  for (int i = 1; i < argc; i++)
    if (const int n = atoi(argv[i]); 1 <= n && n <= 25)
      current_year(n);
    else if (2015 <= n && n <= 2024)
      switch (n) {
      case 2015:
        current_year = &aoc_2015;
        break;
      case 2016:
        current_year = &aoc_2016;
        break;
      case 2017:
        current_year = &aoc_2017;
        break;
      case 2018:
        current_year = &aoc_2018;
        break;
      case 2019:
        current_year = &aoc_2019;
        break;
      case 2020:
        current_year = &aoc_2020;
        break;
      case 2021:
        current_year = &aoc_2021;
        break;
      case 2022:
        current_year = &aoc_2022;
        break;
      case 2023:
        current_year = &aoc_2023;
        break;
      case 2024:
        current_year = &aoc_2024;
        break;
      }
    else
      usage(*argv);

  return 0;
}
