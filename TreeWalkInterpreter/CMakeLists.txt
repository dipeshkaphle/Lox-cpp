cmake_minimum_required(VERSION "3.19.2")

project(lox)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -fsanitize=address -O3")

set(CMAKE_CXX_STANDARD 20)
include_directories(includes)

add_subdirectory(src)

add_executable(lox main.cpp)
