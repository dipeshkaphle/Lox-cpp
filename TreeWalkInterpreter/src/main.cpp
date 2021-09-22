#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "Lox.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include "includes/AstPrinter.hpp"

using std::string;
using std::string_view;
using namespace std;

void run(const string &source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  for (Token token : tokens) {
    std::cout << token.to_string() << '\n';
  }
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
    cout << all_code << '\n';
    run(all_code);
    if (Lox::hadError) {
      exit(20);
    }
  } else {
    throw std::runtime_error("Couldnt open the file " + string(filename));
  }
}

void runPrompt() {
  Lox::hadError = false;
  string line;
  while (true) {
    std::getline(std::cin, line);
    if (line == "") {
      break;
    } else {
      run(line);
      Lox::hadError = false;
    }
  }
}

void f() {
  std::unique_ptr<Expr> lhs = std::make_unique<literal_expr>(double(-123));
  std::unique_ptr<Expr> lit = std::make_unique<literal_expr>(double(45.67));
  std::unique_ptr<Expr> rhs = std::make_unique<grouping_expr>(std::move(lit));
  std::unique_ptr<Expr> bin_exp = std::make_unique<binary_expr>(
      Token(TokenType::PLUS, "+", std::string("+"), 0), std::move(lhs),
      std::move(rhs));
  auto ast = new AstPrinter();
  cout << ast->print(*bin_exp) << '\n';
  auto litrl_bool = literal_expr(true);
  cout << ast->print(litrl_bool) << '\n';
}

int main(int argc, char **argv) {
  // f();
  if (argc > 2) {
    std::cout << "Usage: cpplox [script]";
    exit(255);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}
