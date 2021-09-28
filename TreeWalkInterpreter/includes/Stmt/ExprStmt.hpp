#pragma once

#include "../Expr/Expr.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class expr_stmt : public Stmt {
public:
  std::unique_ptr<Expr> expr;
  expr_stmt(std::unique_ptr<Expr> expr);
  std::any accept(const stmt_visitor<std::any> &visitor) const override;
};
