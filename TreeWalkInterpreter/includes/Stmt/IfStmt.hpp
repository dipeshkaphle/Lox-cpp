
#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>
#include <optional>

class if_stmt : public Stmt {
public:
  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> then_branch;
  std::optional<std::unique_ptr<Stmt>> else_branch;

  if_stmt(std::unique_ptr<Expr> condition_, std::unique_ptr<Stmt> then_branch_,
          std::optional<std::unique_ptr<Stmt>> else_branch_ = std::nullopt);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
