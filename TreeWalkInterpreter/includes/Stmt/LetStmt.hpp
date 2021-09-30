#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class let_stmt : public Stmt {
public:
  Token name;
  std::unique_ptr<Expr> initializer_expr;
  let_stmt(Token _name, std::unique_ptr<Expr> _expr);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
