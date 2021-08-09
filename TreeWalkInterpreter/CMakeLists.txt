cmake_minimum_required(VERSION "3.19.2")

project(lox)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -fsanitize=address -O3")

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/includes)

add_executable(main main.cpp ${PROJECT_SOURCE_DIR}/includes)
