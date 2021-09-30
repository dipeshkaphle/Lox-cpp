#pragma once

#include "../Expr/Expr.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class expr_stmt : public Stmt {
public:
  std::unique_ptr<Expr> expr;
  expr_stmt(std::unique_ptr<Expr> expr);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
