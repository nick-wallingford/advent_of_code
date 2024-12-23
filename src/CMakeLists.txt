add_executable(advent_of_code main.cpp)

target_compile_options(advent_of_code PRIVATE -Wall -Wextra -Wno-maybe-uninitialized -Wno-dangling-else -Wno-unused-function)
target_compile_options(advent_of_code PRIVATE -march=native)

option(sanitize "Use sanitizers")

if(sanitize)
  target_compile_options(advent_of_code PRIVATE -fsanitize=address)
  target_link_libraries(advent_of_code PRIVATE asan)

  target_compile_options(advent_of_code PRIVATE -fsanitize=undefined)
  target_link_libraries(advent_of_code PRIVATE ubsan)
endif()
