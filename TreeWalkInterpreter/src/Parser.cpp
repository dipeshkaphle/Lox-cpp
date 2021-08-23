#include "Parser.hpp"

bool Parser::check(TokenType tk) {
  if (is_at_end())
    return false;
  return peek().type == tk;
}

Token Parser::advance() {
  if (!is_at_end())
    this->cur++;
  return previous();
}

bool Parser::match(const vector<TokenType> &tok) {
  for (auto &tk : tok) {
    if (check(tk)) {
      advance();
      return true;
    }
  }
  return false;
}

std::unique_ptr<Expr> Parser::equality() {
  auto expr = comparision();
  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    auto right = move(comparision());
    expr = std::make_unique<binary_expr>(op, move(expr), move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::comparision() { return nullptr; }
std::unique_ptr<Expr> Parser::term() { return nullptr; }
