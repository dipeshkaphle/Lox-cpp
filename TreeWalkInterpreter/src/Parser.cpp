#include "includes/Parser.hpp"
#include "includes/Lox.hpp"

// TODO : Add break statments

#include <memory>
#include <ranges>
#include <vector>

using parse_err = Parser::parse_error;
using expr_or_err = Parser::expr_or_err;
using expr_ptr = std::unique_ptr<Expr>;
using stmt_or_err = Parser::stmt_or_err;
using stmt_ptr = std::unique_ptr<Stmt>;

#define RETURN_IF_NO_VALUE(expr)                                               \
  if (!((expr).has_value()))                                                   \
    return tl::unexpected((expr).error());

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

expr_or_err Parser::expression() { return this->assignment(); }

expr_or_err Parser::assignment() {
  auto maybe_expr = this->logic_or();
  RETURN_IF_NO_VALUE(maybe_expr);
  if (match({TokenType::EQUAL})) {
    auto equals = previous();
    auto value = this->assignment();
    RETURN_IF_NO_VALUE(value);
    auto rhs = std::move(value.value());
    auto lhs = std::move(maybe_expr.value());
    try {
      auto &lval = dynamic_cast<variable_expr &>(*lhs);
      return (expr_ptr)std::make_unique<assign_expr>(lval.name, std::move(rhs));
    } catch (std::exception &e) {
      return tl::unexpected<parse_err>(this->error(
          equals, "Invalid assignment target. It is not an lvalue"));
    }
  }
  return maybe_expr;
}

expr_or_err Parser::logic_or() {
  auto maybe_lhs = this->logic_and();
  RETURN_IF_NO_VALUE(maybe_lhs);
  while (match({TokenType::OR})) {
    auto op = this->previous();
    auto maybe_rhs = this->logic_and();
    RETURN_IF_NO_VALUE(maybe_rhs);
    maybe_lhs = std::make_unique<logical_expr>(std::move(op),
                                               std::move(maybe_lhs.value()),
                                               std::move(maybe_rhs.value()));
  }
  return maybe_lhs;
}

expr_or_err Parser::logic_and() {
  auto maybe_lhs = this->equality();
  RETURN_IF_NO_VALUE(maybe_lhs);
  while (match({TokenType::AND})) {
    auto op = this->previous();
    auto maybe_rhs = this->equality();
    RETURN_IF_NO_VALUE(maybe_rhs);
    maybe_lhs = std::make_unique<logical_expr>(std::move(op),
                                               std::move(maybe_lhs.value()),
                                               std::move(maybe_rhs.value()));
  }
  return maybe_lhs;
}

expr_or_err Parser::equality() {
  auto expr = comparision();
  RETURN_IF_NO_VALUE(expr);
  while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
    Token op = previous();
    auto right = comparision();
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
  if (match({TokenType::IDENTIFIER})) {
    return (expr_ptr)std::make_unique<variable_expr>(previous());
  }

  if (match({TokenType::LEFT_PAREN})) {
    // return expression().and_then([&](std::unique_ptr<Expr> expr) {
    // consume(TokenType::RIGHT_BRACE, "Expected ) after expression");
    // });
    auto expr = expression();
    auto maybe_right_paren =
        consume(TokenType::RIGHT_PAREN, "Expected ) after expression");
    RETURN_IF_NO_VALUE(maybe_right_paren);
    return (expr_ptr)std::make_unique<grouping_expr>(std::move(expr.value()));
  }
  return tl::make_unexpected<parse_err>(
      this->error(peek(), "Expect expression"));
}

stmt_or_err Parser::statement() {
  if (this->match({TokenType::IF})) {
    return if_statement();
  }
  if (this->match({TokenType::WHILE})) {
    return this->while_statement();
  }
  if (this->match({TokenType::FOR})) {
    return this->for_statement();
  }
  if (this->match({TokenType::PRINT})) {
    return this->print_statement();
  }
  if (this->match({TokenType::LEFT_BRACE})) {
    auto maybe_stmts = this->block();
    RETURN_IF_NO_VALUE(maybe_stmts);
    return std::make_unique<block_stmt>(std::move(maybe_stmts.value()));
  }
  return this->expression_statement();
}

tl::expected<std::vector<stmt_ptr>, parse_err> Parser::block() {
  vector<stmt_ptr> stmts;
  while (!check(TokenType::RIGHT_BRACE) && !this->is_at_end()) {
    auto maybe_declr = this->declaration();
    RETURN_IF_NO_VALUE(maybe_declr);
    stmts.emplace_back(std::move(maybe_declr.value()));
  }
  auto maybe_right_brace =
      consume(TokenType::RIGHT_BRACE, "Expect } after block");
  RETURN_IF_NO_VALUE(maybe_right_brace);
  return stmts;
}

stmt_or_err Parser::if_statement() {
  auto maybe_leftparen = consume(TokenType::LEFT_PAREN, "Expect ( after if");
  RETURN_IF_NO_VALUE(maybe_leftparen);
  auto maybe_cond = this->expression();
  RETURN_IF_NO_VALUE(maybe_cond);
  auto maybe_right_paren =
      consume(TokenType::RIGHT_PAREN, "Expect ) after if condition");
  RETURN_IF_NO_VALUE(maybe_right_paren);
  auto maybe_then_branch = this->statement();
  RETURN_IF_NO_VALUE(maybe_then_branch);
  if (match({TokenType::ELSE})) {
    auto maybe_else_branch = this->statement();
    RETURN_IF_NO_VALUE(maybe_else_branch);
    return (stmt_ptr)std::make_unique<if_stmt>(
        std::move(maybe_cond.value()), std::move(maybe_then_branch.value()),
        std::move(maybe_else_branch.value()));
  }
  return (stmt_ptr)std::make_unique<if_stmt>(
      std::move(maybe_cond.value()), std::move(maybe_then_branch.value()));
}

stmt_or_err Parser::while_statement() {
  auto maybe_leftparen = consume(TokenType::LEFT_PAREN, "Expect ( after while");
  RETURN_IF_NO_VALUE(maybe_leftparen);
  auto maybe_cond = this->expression();
  RETURN_IF_NO_VALUE(maybe_cond);
  auto maybe_right_paren =
      consume(TokenType::RIGHT_PAREN, "Expect ) after while condition");
  RETURN_IF_NO_VALUE(maybe_right_paren);
  auto maybe_body = this->statement();
  RETURN_IF_NO_VALUE(maybe_body);
  return (stmt_ptr)std::make_unique<while_stmt>(std::move(maybe_cond.value()),
                                                std::move(maybe_body.value()));
}

stmt_or_err Parser::for_statement() {
  /*
   * for( <stmt1>; <expression> ; <expression>) <block>
   */

  /*
   * LEFT_PAREN
   */
  auto maybe_leftparen = consume(TokenType::LEFT_PAREN, "Expect ( after for");
  RETURN_IF_NO_VALUE(maybe_leftparen);

  /*
   * <stmt1>
   */
  std::optional<stmt_ptr> maybe_initializer = std::nullopt;
  if (this->match({TokenType::SEMICOLON})) {
  } else if (this->match({TokenType::LET})) {
    auto maybe_declr = this->let_declaration();
    RETURN_IF_NO_VALUE(maybe_declr);
    maybe_initializer = std::move(maybe_declr.value());
  } else {
    auto maybe_stmt = this->expression_statement();
    RETURN_IF_NO_VALUE(maybe_stmt);
    maybe_initializer = std::move(maybe_stmt.value());
  }

  /*
   * <expression>
   * Its an expression statement
   */
  std::optional<expr_ptr> maybe_condition = std::nullopt;
  if (!this->check(TokenType::SEMICOLON)) {
    auto maybe_expr_stmt = this->expression();
    RETURN_IF_NO_VALUE(maybe_expr_stmt);
    maybe_condition = std::move(maybe_expr_stmt.value());
  }
  auto maybe_semicolon =
      consume(TokenType::SEMICOLON, "Expect ; after conditional in for ");
  RETURN_IF_NO_VALUE(maybe_semicolon);

  /*
   * <expression>
   */
  std::optional<stmt_ptr> maybe_change_fn;
  if (!check(TokenType::RIGHT_PAREN)) {
    auto maybe_change_expr = this->expression();
    RETURN_IF_NO_VALUE(maybe_change_expr);
    maybe_change_fn = (stmt_ptr)std::make_unique<expr_stmt>(
        std::move(maybe_change_expr.value()));
  }

  /*
   * RIGHT_PAREN
   */
  auto maybe_right_paren = consume(
      TokenType::RIGHT_PAREN, "Expect closing paren ')' after for clauses");
  RETURN_IF_NO_VALUE(maybe_right_paren);

  /*
   * <block>
   */
  auto maybe_body = this->statement();
  RETURN_IF_NO_VALUE(maybe_body);

  if (maybe_change_fn.has_value()) {
    std::array<stmt_ptr, 2> stmts = {std::move(maybe_body.value()),
                                     std::move(maybe_change_fn.value())};
    auto stmts_in_new_body = vector<stmt_ptr>(make_move_iterator(stmts.begin()),
                                              make_move_iterator(stmts.end()));
    maybe_body = std::make_unique<block_stmt>(std::move(stmts_in_new_body));
  }

  if (!maybe_condition.has_value()) {
    maybe_condition = std::make_unique<literal_expr>(true);
  }

  maybe_body = std::make_unique<while_stmt>(std::move(maybe_condition.value()),
                                            std::move(maybe_body.value()));

  if (maybe_initializer.has_value()) {
    std::array<stmt_ptr, 2> stmts = {std::move(maybe_initializer.value()),
                                     std::move(maybe_body.value())};
    auto stmts_in_new_body = vector<stmt_ptr>(make_move_iterator(stmts.begin()),
                                              make_move_iterator(stmts.end()));
    maybe_body = std::make_unique<block_stmt>(std::move(stmts_in_new_body));
  }
  return maybe_body;
}

stmt_or_err Parser::print_statement() {
  auto expr = this->expression();
  RETURN_IF_NO_VALUE(expr);
  auto maybe_semicolon =
      consume(TokenType::SEMICOLON, "Expect ; after statement");
  RETURN_IF_NO_VALUE(maybe_semicolon);
  return move(expr).and_then([&](expr_or_err &&exp) -> stmt_or_err {
    return std::make_unique<print_stmt>(move(exp.value()));
  });
}

stmt_or_err Parser::expression_statement() {
  auto expr = this->expression();
  RETURN_IF_NO_VALUE(expr);
  auto maybe_semicolon =
      consume(TokenType::SEMICOLON, "Expect ; after expression");
  RETURN_IF_NO_VALUE(maybe_semicolon);
  return move(expr).and_then([&](expr_or_err &&exp) -> stmt_or_err {
    return std::make_unique<expr_stmt>(move(exp.value()));
  });
}

stmt_or_err Parser::declaration() {
  if (match({TokenType::LET})) {
    return this->let_declaration();
  }
  return statement();
}

stmt_or_err Parser::let_declaration() {
  /*
   * I wont allow empty declaration
   * let x; is not allowed
   * It must be let x = 2; (something like this)
   */
  auto maybe_id = consume(TokenType::IDENTIFIER,
                          "Expected identifier after let declaration");
  RETURN_IF_NO_VALUE((maybe_id));
  auto maybe_equal_to = consume(
      TokenType::EQUAL,
      "Cannot have empty declartion. It must be of form let <id> = <expr>;");
  RETURN_IF_NO_VALUE(maybe_equal_to);
  auto initializer_expr = this->expression();
  RETURN_IF_NO_VALUE(initializer_expr);
  auto maybe_semicolon = consume(TokenType::SEMICOLON,
                                 "Expected ; after the end of let declaration");
  RETURN_IF_NO_VALUE(maybe_semicolon);
  return (stmt_ptr)std::make_unique<let_stmt>(
      maybe_id.value(), std::move(initializer_expr.value()));
}

vector<stmt_or_err> Parser::parse() {
  vector<stmt_or_err> maybe_statements;
  while (!is_at_end()) {
    auto stmt = this->declaration();
    if (!stmt.has_value()) {
      this->synchronize();
    }
    maybe_statements.emplace_back(std::move(stmt));
  }
  return maybe_statements;
}
