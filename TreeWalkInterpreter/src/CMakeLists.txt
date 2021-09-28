add_library(Lox STATIC)
# target_link_libraries(Lox PRIVATE ${CONAN_LIBS})
target_link_libraries(Lox PRIVATE CONAN_PKG::tl-expected)

target_include_directories(Lox PUBLIC ${lox_SOURCE_DIR}
                                      ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Lox PRIVATE Lox.cpp Scanner.cpp Parser.cpp AstPrinter.cpp
                           Interpreter.cpp)

add_subdirectory(Expr)
add_subdirectory(Stmt)

add_executable(lox_repl main.cpp)
set_target_properties(lox_repl PROPERTIES OUTPUT_NAME "lox")

target_link_libraries(lox_repl PUBLIC Lox)
