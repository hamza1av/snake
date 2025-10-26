cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
set(SOURCES
    src/main.cpp
    src/agent.cpp
    src/snake.cpp
)

# Include headers
include_directories(src)

# Find ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(snake_lib src/snake.cpp src/agent.cpp)

# Build the executable and link ncurses
add_executable(snake src/main.cpp)
target_link_libraries(snake snake_lib ${CURSES_LIBRARIES})

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tests
  tests/test_snake.cpp
)

target_link_libraries(
  tests
  snake_lib
  GTest::gtest_main
  ${CURSES_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(tests)

