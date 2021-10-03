#pragma once

#include "../Expr/Expr.hpp"
#include "../Token.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>
using stmt_ptr = std::unique_ptr<Stmt>;

class block_stmt : public Stmt {
public:
  vector<stmt_ptr> statements;
  block_stmt(vector<stmt_ptr> stmts);
  block_stmt(block_stmt &&stmt) noexcept;
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
