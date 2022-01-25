cmake_minimum_required(VERSION "3.19.2")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(lox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address ")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

option(DEBUG "Enable DEBUG in the source files" OFF)

if(DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
endif()

# clang-format boilerplate
find_program(
  CLANG_FORMAT_EXE
  NAMES "clang-format"
  DOC "Path to clang-format executable")

if(NOT CLANG_FORMAT_EXE)
  message(STATUS "clang-format not found.")
else()
  message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")

  file(GLOB_RECURSE FILES_TO_FORMAT includes/*.hpp src/*.cpp)

  add_custom_target(
    clang-format-project-files COMMAND ${CLANG_FORMAT_EXE} -i -style=LLVM
                                       ${FILES_TO_FORMAT})
endif()

# enable cache system
include(${CMAKE_SOURCE_DIR}/ccache.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
conan_basic_setup()

add_subdirectory(src)
