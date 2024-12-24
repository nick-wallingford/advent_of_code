set(SOURCES main.cpp util/a_star.cpp util/maze.cpp util/util.cpp util/vec.cpp)

foreach(year RANGE 2015 2024)
  set(SOURCES ${SOURCES} ${year}/main.cpp)
  foreach(day RANGE 1 25)
    set(SOURCES ${SOURCES} ${year}/day_${day}.cpp)
  endforeach()
endforeach()

add_executable(advent_of_code ${SOURCES})
target_include_directories(advent_of_code PRIVATE include)

target_compile_options(advent_of_code PRIVATE -Wall -Wextra -Wno-maybe-uninitialized -Wno-dangling-else -Wno-unused-function)
target_compile_options(advent_of_code PRIVATE -march=native)

option(sanitize "Use sanitizers")

if(sanitize)
  target_compile_options(advent_of_code PRIVATE -fsanitize=address)
  target_link_libraries(advent_of_code PRIVATE asan)

  target_compile_options(advent_of_code PRIVATE -fsanitize=undefined)
  target_link_libraries(advent_of_code PRIVATE ubsan)
endif()
