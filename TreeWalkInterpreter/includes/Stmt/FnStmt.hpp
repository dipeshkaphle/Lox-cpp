#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

using stmt_ptr = std::unique_ptr<Stmt>;

class fn_stmt : public Stmt {
public:
  Token name;
  vector<Token> params;
  // can't store a block because of the possibility of early returns
  vector<stmt_ptr> body;

  fn_stmt(Token fn_name, vector<Token> params, vector<stmt_ptr> fn_body);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
