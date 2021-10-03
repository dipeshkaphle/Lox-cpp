#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class while_stmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;

  while_stmt(std::unique_ptr<Expr> condition_, std::unique_ptr<Stmt> body_);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
