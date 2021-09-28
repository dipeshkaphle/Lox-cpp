#include "includes/Parser.hpp"
#include "includes/Lox.hpp"

#include <ranges>

using parse_err = Parser::parse_error;
using expr_or_err = Parser::expr_or_err;
using expr_ptr = std::unique_ptr<Expr>;
using stmt_or_err = Parser::stmt_or_err;
using stmt_ptr = std::unique_ptr<Stmt>;

#define RETURN_IF_NO_VALUE(expr)                                               \
  if (!((expr).has_value()))                                                   \
    return (expr);

bool Parser::check(TokenType tk) {
  if (is_at_end()) {
    return false;
  }
  return peek().type == tk;
}

Token Parser::advance() {
  if (!is_at_end()) {
    this->cur++;
  }
  return previous();
}

bool Parser::match(const vector<TokenType> &tok) {
  if (std::ranges::any_of(tok, [&](auto &tk) { return this->check(tk); })) {
    advance();
    return true;
  }
  return false;
}

void Parser::synchronize() {
  advance();
  while (!this->is_at_end()) {
    if (previous().type == TokenType::SEMICOLON) {
      return;
    }
    switch (this->peek().type) {
    case TokenType::CLASS:
    case TokenType::FN:
    case TokenType::LET:
    case TokenType::FOR:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::PRINT:
    case TokenType::RETURN:
      return;
    default:
      advance();
    }
  }
}

expected<Token, Parser::parse_error> Parser::consume(TokenType tok,
                                                     const char *err_msg) {
  if (check(tok)) {
    return this->advance();
  }
  return tl::unexpected<Parser::parse_error>(Parser::error(peek(), err_msg));
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
  if (match({TokenType::TRUE})) {
    return (expr_ptr)std::make_unique<literal_expr>(true);
  }
  if (match({TokenType::FALSE})) {
    return (expr_ptr)std::make_unique<literal_expr>(false);
  }
  if (match({TokenType::NIL})) {
    return (expr_ptr)std::make_unique<literal_expr>(std::any{});
  }
  if (match({TokenType::NUMBER, TokenType::STRING})) {
    auto prev_literal = previous().literal;
    return (expr_ptr)std::make_unique<literal_expr>(std::move(prev_literal));
  }
  if (match({TokenType::LEFT_PAREN})) {
    // return expression().and_then([&](std::unique_ptr<Expr> expr) {
    // consume(TokenType::RIGHT_BRACE, "Expected ) after expression");
    // });
    auto expr = expression();
    auto got_consumed =
        consume(TokenType::RIGHT_PAREN, "Expected ) after expression");
    if (!got_consumed.has_value()) {
      return tl::unexpected<parse_err>(got_consumed.error());
    }
    return (expr_ptr)std::make_unique<grouping_expr>(std::move(expr.value()));
  }
  return tl::unexpected<parse_err>(this->error(peek(), "Expect expression"));
}

stmt_or_err Parser::statement() {
  if (this->match({TokenType::PRINT})) {
    return this->print_statement();
  }
  return this->expression_statement();
}

stmt_or_err Parser::print_statement() {
  auto expr = this->expression();
  consume(TokenType::SEMICOLON, "Expect ; after statement");
  return move(expr).and_then([&](expr_or_err &&exp) -> stmt_or_err {
    return std::make_unique<print_stmt>(move(exp.value()));
  });
}

stmt_or_err Parser::expression_statement() {
  auto expr = this->expression();
  consume(TokenType::SEMICOLON, "Expect ; after expression\n");
  return move(expr).and_then([&](expr_or_err &&exp) -> stmt_or_err {
    return std::make_unique<expr_stmt>(move(exp.value()));
  });
}

vector<stmt_or_err> Parser::parse() {
  vector<stmt_or_err> maybe_statements;
  while (!is_at_end()) {
    auto stmt = this->statement();
    if (!stmt.has_value()) {
      this->synchronize();
    }
    maybe_statements.emplace_back(std::move(stmt));
  }
  return maybe_statements;
}
