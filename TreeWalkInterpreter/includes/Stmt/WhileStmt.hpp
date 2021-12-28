#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>
#include <optional>

class while_stmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;
  optional<std::unique_ptr<Stmt>> change_fn;

  while_stmt(std::unique_ptr<Expr> condition_, std::unique_ptr<Stmt> body_,
             optional<std::unique_ptr<Stmt>> change_fn_ = std::nullopt);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
