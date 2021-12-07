#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "includes/AstPrinter.hpp"
#include "includes/Lox.hpp"
#include "includes/Parser.hpp"
#include "includes/Scanner.hpp"
#include "includes/Token.hpp"

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/ostream.h"
#include "tl/expected.hpp"
using namespace tl;

using std::string;
using std::string_view;
using namespace std;

void run(const string &source, bool is_repl = false) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

  // for (Token token : tokens) {
  // std::cout << token.to_string() << endl;
  // }

  Parser parser(tokens);

  auto maybe_statements = parser.parse();

  if (Lox::hadError) {
    return;
  }
  vector<std::unique_ptr<Stmt>> statements;
  statements.reserve(maybe_statements.size());
  for (auto &maybe_stmt : maybe_statements) {
    statements.emplace_back(std::move(maybe_stmt.value()));
  }
  Lox::interp.interpret(statements, is_repl);
}

void runFile(char *filename) {
  std::ifstream inp(filename);

  if (inp.is_open()) {
    string all_code;
    string tmp;
    while (std::getline(inp, tmp)) {
      all_code.append(tmp);
      all_code.push_back('\n');
    }
    // cout << all_code << '\n';
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

/*
 * void f() {
 *   std::unique_ptr<Expr> lhs = std::make_unique<literal_expr>(double(-123));
 *   std::unique_ptr<Expr> lit = std::make_unique<literal_expr>(double(45.67));
 *   std::unique_ptr<Expr> rhs =
 * std::make_unique<grouping_expr>(std::move(lit)); std::unique_ptr<Expr>
 * bin_exp = std::make_unique<binary_expr>( Token(TokenType::PLUS, "+",
 * std::string("+"), 0), std::move(lhs), std::move(rhs)); auto ast =
 * ast_printer(); cout << ast.print(*bin_exp) << '\n'; auto litrl_bool =
 * literal_expr(true); cout << ast.print(litrl_bool) << '\n';
 * }
 */

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
