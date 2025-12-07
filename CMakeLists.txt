cmake_minimum_required(VERSION 3.24.2)

project(advent_of_code)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES src/main.cpp src/util/a_star.cpp src/util/maze.cpp src/util/util.cpp src/util/vec.cpp)

foreach(year RANGE 2015 2025)
  set(SOURCES ${SOURCES} src/${year}/main.cpp)
  foreach(day RANGE 1 25)
    set(SOURCES ${SOURCES} src/${year}/day_${day}.cpp)
  endforeach()
endforeach()

add_executable(advent_of_code ${SOURCES})
target_include_directories(advent_of_code PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

target_compile_options(advent_of_code PRIVATE -Wall -Wextra -Wno-maybe-uninitialized -Wno-dangling-else -Wno-unused-function)
target_compile_options(advent_of_code PRIVATE -march=native)

option(sanitize "Use sanitizers" OFF)
if(sanitize)
  target_compile_options(advent_of_code PRIVATE -fsanitize=address)
  target_link_libraries(advent_of_code PRIVATE asan)

  target_compile_options(advent_of_code PRIVATE -fsanitize=undefined)
  target_link_libraries(advent_of_code PRIVATE ubsan)
endif()

option(valgrind "Disable instructions not usable by valgrind" OFF)
if (valgrind)
  target_compile_options(advent_of_code PRIVATE -mno-avx512f)
endif()
