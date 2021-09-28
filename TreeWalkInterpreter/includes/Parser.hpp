#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expr/Expr.hpp"
#include "Expr/binary_expr.hpp"
#include "Expr/grouping_expr.hpp"
#include "Expr/literal_expr.hpp"
#include "Expr/unary_expr.hpp"
#include "Stmt/ExprStmt.hpp"
#include "Stmt/PrintStmt.hpp"
#include "Stmt/Stmt.hpp"
#include "Token.hpp"
#include "TokenTypes.hpp"

#include "tl/expected.hpp"
using namespace tl;

using namespace std;
/*
 *
 *
 *
 *
 *	expression     → equality ;
 *	equality       → comparison ( ( "!=" | "==" ) comparison )* ;
 *	comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
 *	term           → factor ( ( "-" | "+" ) factor )* ;
 *	factor         → unary ( ( "/" | "*" ) unary )* ;
 *	unary          → ( "!" | "-" ) unary
 *								 | primary ;
 *	primary        → NUMBER | STRING | "true" | "false" | "nil"
 *					       | "(" expression ")"
 *;
 *
 *
 *
 */

class Parser {
public:
  struct parse_error : public runtime_error {
    explicit parse_error(const char *err_msg) : runtime_error(err_msg) {}
  };
  using expr_or_err = expected<std::unique_ptr<Expr>, parse_error>;
  using stmt_or_err = expected<std::unique_ptr<Stmt>, parse_error>;

private:
  vector<Token> tokens;
  int cur = 0;

  bool check(TokenType tk);

  Token advance();

  bool is_at_end() { return peek().type == TokenType::ENDOFFILE; }

  Token peek() { return tokens[cur]; }

  Token previous() { return tokens[cur - 1]; }

  bool match(const vector<TokenType> &tok);

  void synchronize();

  expected<Token, parse_error> consume(TokenType tok, const char *err_msg);

  expr_or_err expression();
  expr_or_err equality();
  expr_or_err comparision();
  expr_or_err term();
  expr_or_err factor();
  expr_or_err unary();
  expr_or_err primary();

  stmt_or_err statement();
  stmt_or_err expression_statement();
  stmt_or_err print_statement();

  static parse_error error(Token tok, const char *err_msg);

public:
  explicit Parser(vector<Token> _toks) : tokens(move(_toks)), cur(0) {}
  vector<stmt_or_err> parse();
};
