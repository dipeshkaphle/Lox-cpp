cmake_minimum_required(VERSION "3.19.2")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
