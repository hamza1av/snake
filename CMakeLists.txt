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

# Build the executable and link ncurses
add_executable(snake ${SOURCES})
target_link_libraries(snake ${CURSES_LIBRARIES})

