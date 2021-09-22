#include "includes/Parser.hpp"
#include "includes/Lox.hpp"

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

Token Parser::consume(TokenType tok, const char *err_msg) {
  if (check(tok)) {
    return this->advance();
  }
  throw this->error(peek(), err_msg);
}

Parser::parse_error Parser::error(Token tok, const char *err_msg) {
  Lox::error(move(tok), err_msg);
  return parse_error(err_msg);
}

std::unique_ptr<Expr> Parser::expression() { return Parser::equality(); }

std::unique_ptr<Expr> Parser::equality() {
  auto expr = comparision();
  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    auto right = move(comparision());
    expr = std::make_unique<binary_expr>(op, move(expr), move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::comparision() {
  auto expr = this->term();
  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
    auto op = this->previous();
    auto right = this->term();
    expr = std::make_unique<binary_expr>(op, std::move(expr), std::move(right));
  }
  return expr;
}
std::unique_ptr<Expr> Parser::term() {
  auto expr = this->factor();
  while (this->match({TokenType::PLUS, TokenType::MINUS})) {
    auto op = this->previous();
    auto right = this->factor();
    expr = std::make_unique<binary_expr>(op, std::move(expr), std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::factor() {
  auto expr = this->unary();
  while (match({TokenType::SLASH, TokenType::STAR})) {
    auto op = this->previous();
    auto right = this->unary();
    expr = std::make_unique<binary_expr>(op, std::move(expr), std::move(right));
  }
  return expr;
}

std::unique_ptr<Expr> Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    auto op = this->previous();
    auto right = this->unary();
    return make_unique<unary_expr>(op, std::move(right));
  }
  return primary();
}

std::unique_ptr<Expr> Parser::primary() {
  if (match({TokenType::TRUE}))
    return std::make_unique<literal_expr>(true);
  else if (match({TokenType::FALSE}))
    return std::make_unique<literal_expr>(false);
  else if (match({TokenType::NIL}))
    return std::make_unique<literal_expr>(std::any{});
  else if (match({TokenType::NUMBER, TokenType::STRING})) {
    auto prev_literal = previous().literal;
    return std::make_unique<literal_expr>(std::move(prev_literal));
  } else if (match({TokenType::LEFT_PAREN})) {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ) after expression");
    return std::make_unique<grouping_expr>(std::move(expr));
  }
  throw error(peek(), "Expect expression");
}

std::unique_ptr<Expr> Parser::parse() {
  try {
    return this->expression();
  } catch (const Parser::parse_error &err) {
    return nullptr;
  }
}
