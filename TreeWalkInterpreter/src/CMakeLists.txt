add_library(Lox STATIC)
# target_link_libraries(Lox PRIVATE ${CONAN_LIBS})
set(CMAKE_CXX_STANDARD 20)
target_link_libraries(Lox PRIVATE CONAN_PKG::tl-expected CONAN_PKG::fmt)

target_include_directories(Lox PUBLIC ${lox_SOURCE_DIR}
                                      ${CMAKE_CURRENT_SOURCE_DIR})

# target_sources(Lox PRIVATE Lox.cpp Scanner.cpp Parser.cpp AstPrinter.cpp
# Environment.cpp Interpreter.cpp)
target_sources(Lox PRIVATE Lox.cpp Scanner.cpp Parser.cpp Environment.cpp
                           Interpreter.cpp)

add_subdirectory(Expr)
add_subdirectory(Stmt)

add_executable(lox_repl main.cpp)
set_target_properties(lox_repl PROPERTIES OUTPUT_NAME "lox")

target_link_libraries(lox_repl PUBLIC Lox)
