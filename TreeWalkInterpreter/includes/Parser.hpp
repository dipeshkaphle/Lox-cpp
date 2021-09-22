#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expr/Expr.hpp"
#include "Expr/binary_expr.hpp"
#include "Expr/grouping_expr.hpp"
#include "Expr/literal_expr.hpp"
#include "Expr/unary_expr.hpp"
#include "Token.hpp"
#include "TokenTypes.hpp"

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
    parse_error(const char *err_msg) : runtime_error(err_msg) {}
  };

private:
  vector<Token> tokens;
  int cur = 0;

  bool check(TokenType tk);

  Token advance();

  bool is_at_end() { return peek().type == TokenType::ENDOFFILE; }

  Token peek() { return tokens[cur]; }

  Token previous() { return tokens[cur - 1]; }

  bool match(const vector<TokenType> &tok);

  Token consume(TokenType tok, const char *err_msg);

  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparision();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();

  parse_error error(Token tok, const char *err_msg);

public:
  Parser(vector<Token> _toks) : tokens(_toks), cur(0) {}
  std::unique_ptr<Expr> parse();
};
