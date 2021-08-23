add_library(Lox)

target_include_directories(
  Lox
  PUBLIC ${lox_SOURCE_DIR}/includes
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Expr)

target_sources(Lox PUBLIC Lox.cpp Scanner.cpp Parser.cpp)

add_executable(lox_repl)
set_target_properties(lox_repl PROPERTIES OUTPUT_NAME "lox")

target_include_directories(
  lox_repl
  PUBLIC ${lox_SOURCE_DIR}/includes
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(lox_repl PRIVATE main.cpp)

target_link_libraries(lox_repl PRIVATE Lox)
