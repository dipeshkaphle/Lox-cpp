#pragma once

#include "../Expr/Expr.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class print_stmt : public Stmt {
public:
  std::unique_ptr<Expr> expr;
  bool has_newline;
  print_stmt(std::unique_ptr<Expr> _expr, bool new_line = false);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
