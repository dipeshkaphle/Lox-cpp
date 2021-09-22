#include "includes/Parser.hpp"
#include "includes/Lox.hpp"

using parse_err = Parser::parse_error;
using expr_or_err = Parser::expr_or_err;
using expr_ptr = std::unique_ptr<Expr>;

#define RETURN_IF_NO_VALUE(expr)                                               \
  if (!((expr).has_value()))                                                   \
    return (expr);

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

expected<Token, Parser::parse_error> Parser::consume(TokenType tok,
                                                     const char *err_msg) {
  if (check(tok)) {
    return this->advance();
  }
  throw this->error(peek(), err_msg);
}

Parser::parse_error Parser::error(Token tok, const char *err_msg) {
  Lox::error(move(tok), err_msg);
  return parse_error(err_msg);
}

expr_or_err Parser::expression() { return Parser::equality(); }

expr_or_err Parser::equality() {
  auto expr = comparision();
  RETURN_IF_NO_VALUE(expr);
  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    auto right = move(comparision());
    // expr = std::make_unique<binary_expr>(op, move(expr), move(right));
    expr = std::move(right).and_then(
        [&](std::unique_ptr<Expr> rgt) -> expr_or_err {
          return (expr_ptr)std::make_unique<binary_expr>(
              op, std::move(expr.value()), std::move(rgt));
        });
    RETURN_IF_NO_VALUE(expr);
  }
  return expr;
}

expr_or_err Parser::comparision() {
  auto expr = this->term();
  RETURN_IF_NO_VALUE(expr);
  while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                TokenType::LESS_EQUAL})) {
    auto op = this->previous();
    auto right = this->term();
    // expr = std::make_unique<binary_expr>(op, std::move(expr),
    // std::move(right));
    expr = std::move(right).and_then(
        [&](std::unique_ptr<Expr> rgt) -> expr_or_err {
          return (expr_ptr)std::make_unique<binary_expr>(
              op, std::move(expr.value()), std::move(rgt));
        });
    RETURN_IF_NO_VALUE(expr);
  }
  return expr;
}

expr_or_err Parser::term() {
  auto expr = this->factor();
  RETURN_IF_NO_VALUE(expr);
  while (this->match({TokenType::PLUS, TokenType::MINUS})) {
    auto op = this->previous();
    auto right = this->factor();
    expr = std::move(right).and_then(
        [&](std::unique_ptr<Expr> rgt) -> expr_or_err {
          return (expr_ptr)std::make_unique<binary_expr>(
              op, std::move(expr.value()), std::move(rgt));
        });
    RETURN_IF_NO_VALUE(expr);
  }
  return expr;
}

expr_or_err Parser::factor() {
  auto expr = this->unary();
  RETURN_IF_NO_VALUE(expr);
  while (match({TokenType::SLASH, TokenType::STAR})) {
    auto op = this->previous();
    auto right = this->unary();
    // expr = std::make_unique<binary_expr>(op, std::move(expr),
    // std::move(right));
    expr = std::move(right).and_then(
        [&](std::unique_ptr<Expr> rgt) -> expr_or_err {
          return (expr_ptr)std::make_unique<binary_expr>(
              op, std::move(expr.value()), std::move(rgt));
        });
    RETURN_IF_NO_VALUE(expr);
  }
  return expr;
}

expr_or_err Parser::unary() {
  if (match({TokenType::BANG, TokenType::MINUS})) {
    auto op = this->previous();
    // auto right = this->unary();

    return this->unary().and_then(
        [&](std::unique_ptr<Expr> right) -> expr_or_err {
          return (expr_ptr)std::make_unique<unary_expr>(op, std::move(right));
        });
    // return right.and_then([&](auto &&rgt))
    // return make_unique<unary_expr>(op, std::move(right));
  }
  return primary();
}

expr_or_err Parser::primary() {
  if (match({TokenType::TRUE}))
    return (expr_ptr)std::make_unique<literal_expr>(true);
  else if (match({TokenType::FALSE}))
    return (expr_ptr)std::make_unique<literal_expr>(false);
  else if (match({TokenType::NIL}))
    return (expr_ptr)std::make_unique<literal_expr>(std::any{});
  else if (match({TokenType::NUMBER, TokenType::STRING})) {
    auto prev_literal = previous().literal;
    return (expr_ptr)std::make_unique<literal_expr>(std::move(prev_literal));
  } else if (match({TokenType::LEFT_PAREN})) {
    // return expression().and_then([&](std::unique_ptr<Expr> expr) {
    // consume(TokenType::RIGHT_BRACE, "Expected ) after expression");
    // });
    auto expr = expression();
    auto got_consumed =
        consume(TokenType::RIGHT_PAREN, "Expected ) after expression");
    if (!got_consumed)
      return tl::unexpected<parse_err>(got_consumed.error());
    return (expr_ptr)std::make_unique<grouping_expr>(std::move(expr.value()));
  }
  return tl::unexpected<parse_err>(error(peek(), "Expect expression"));
}

expr_or_err Parser::parse() { return this->expression(); }
