#include "includes/AstPrinter.hpp"
#include "includes/Lox.hpp"
#include "includes/Parser.hpp"
#include "includes/Scanner.hpp"
#include "includes/Token.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <tl/expected.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace tl;

using std::string;
using std::string_view;
using namespace std;

void run(const string &source, bool is_repl = false) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

#ifdef DEBUG
  fmt::print("=======================================\n");
  fmt::print("All the Tokens\n");
  fmt::print("=======================================\n");
  fmt::print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
  std::ranges::for_each(tokens,
                        [](auto &x) { fmt::print("{}\n", x.to_string()); });
  fmt::print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
  fmt::print("=======================================\n");
#endif

  Parser parser(tokens);

  auto maybe_statements = parser.parse();

  if (Lox::hadError) {
    return;
  }
  vector<std::unique_ptr<Stmt>> statements;
  statements.reserve(maybe_statements.size());

#ifdef DEBUG
  fmt::print("=======================================\n");
  fmt::print("All the Statements\n");
  fmt::print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
#endif

  std::ranges::for_each(maybe_statements, [&](auto &maybe_stmt) {
    auto s = move(maybe_stmt.value());

#ifdef DEBUG
    ast_printer printer;
    fmt::print("{}\n", printer.print(*s));
#endif

    statements.emplace_back(std::move(s));
  });

#ifdef DEBUG
  fmt::print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
  fmt::print("=======================================\n");
#endif

#ifdef DEBUG
  fmt::print("=======================================\n");
  fmt::print("Interpreters output\n");
  fmt::print("=======================================\n");
#endif

  Lox::interp.interpret(statements, is_repl);
}

void runFile(const char *filename) {
  std::ifstream inp(filename);

  if (inp.is_open()) {
    string all_code((std::istreambuf_iterator<char>(inp)),
                    (std::istreambuf_iterator<char>()));
    try {
      run(all_code, false);
    } catch (std::exception &e) {
      fmt::print(std::cerr, "Exception: {}", e.what());
    }
    if (Lox::hadError) {
      exit(20);
    }
    if (Lox::hadRuntimeError) {
      exit(21);
    }
  } else {
    throw std::runtime_error("Couldnt open the file " + string(filename));
  }
}

void runPrompt() {
  Lox::hadError = false;
  string line;
  while (true) {
    fmt::print(">>> ");
    // cout << ">>> ";
    // cout.flush();
    std::getline(std::cin, line);
    if (line.empty()) {
      break;
    }
    try {
      run(line, true);
    } catch (std::exception &e) {
      cerr << "Exception: " << e.what() << '\n';
    }
    Lox::hadError = false;
  }
}

int main(int argc, char **argv) {

  if (argc > 2) {
    fmt::print("Usage: cpplox [script]");
    exit(255);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}
