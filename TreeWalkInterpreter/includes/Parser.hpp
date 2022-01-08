#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Expr/Expr.hpp"
#include "Expr/assign_expr.hpp"
#include "Expr/binary_expr.hpp"
#include "Expr/call_expr.hpp"
#include "Expr/grouping_expr.hpp"
#include "Expr/literal_expr.hpp"
#include "Expr/logical_expr.hpp"
#include "Expr/unary_expr.hpp"
#include "Expr/variable_expr.hpp"
#include "Stmt/BlockStmt.hpp"
#include "Stmt/BreakStmt.hpp"
#include "Stmt/ContinueStmt.hpp"
#include "Stmt/ExprStmt.hpp"
#include "Stmt/FnStmt.hpp"
#include "Stmt/IfStmt.hpp"
#include "Stmt/LetStmt.hpp"
#include "Stmt/PrintStmt.hpp"
#include "Stmt/ReturnStmt.hpp"
#include "Stmt/Stmt.hpp"
#include "Stmt/WhileStmt.hpp"
#include "Token.hpp"
#include "TokenTypes.hpp"

#include <tl/expected.hpp>
using namespace tl;

using namespace std;
/*
 *
 *
 *
 *
 *  expression     → assignment ;
 *
 *  assignment     → IDENTIFIER "=" assignment
 *                 | logic_or ;
 *  logic_or       → logic_and ( "or" logic_and )* ;
 *  logic_and      → equality ( "and" equality )* ;
 *	equality       → comparison ( ( "!=" | "==" ) comparison )* ;
 *	comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
 *	term           → factor ( ( "-" | "+" ) factor )* ;
 *	factor         → unary ( ( "/" | "*" ) unary )* ;
 *	unary          → ( "!" | "-" ) unary | call;
 *	call           → primary ( "(" arguments? ")" )*;
 *	arguments      → expression ( "," expression )* ;
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
  int cur{0};
  int loop_nesting_count{0};

  bool check(TokenType tk);

  Token advance();

  bool is_at_end() { return peek().type == TokenType::ENDOFFILE; }

  Token peek() { return tokens[cur]; }

  Token previous() { return tokens[cur - 1]; }

  bool match(const vector<TokenType> &tok);

  void synchronize();

  expected<Token, parse_error> consume(TokenType tok, const char *err_msg);

  expr_or_err expression();
  expr_or_err assignment();
  expr_or_err logic_or();
  expr_or_err logic_and();
  expr_or_err equality();
  expr_or_err comparision();
  expr_or_err term();
  expr_or_err factor();
  expr_or_err unary();
  expr_or_err call();
  expr_or_err primary();

  stmt_or_err statement();
  stmt_or_err break_statement();
  stmt_or_err continue_statement();
  stmt_or_err expression_statement();
  stmt_or_err print_statement(bool new_line);
  stmt_or_err return_statement();
  stmt_or_err declaration();
  stmt_or_err fn_declaration(const string &type);
  stmt_or_err let_declaration();
  stmt_or_err if_statement();
  stmt_or_err while_statement();
  stmt_or_err for_statement();
  tl::expected<vector<stmt_ptr>, parse_error> block();

  static parse_error error(Token tok, const char *err_msg);

public:
  explicit Parser(vector<Token> _toks) : tokens(move(_toks)), cur(0) {}
  vector<stmt_or_err> parse();
  expr_or_err finish_call(unique_ptr<Expr> callee);
};
