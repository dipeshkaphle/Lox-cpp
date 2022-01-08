#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>
#include <optional>

class return_stmt : public Stmt {
public:
  Token keyword;
  std::optional<std::unique_ptr<Expr>> value;
  return_stmt(Token keyword, optional<unique_ptr<Expr>> val = nullopt);
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
