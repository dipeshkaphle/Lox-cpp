#pragma once

#include "../Expr/Expr.hpp"
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

#include <memory>

class continue_stmt : public Stmt {
private:
public:
  continue_stmt();
  std::any accept(stmt_visitor<std::any> &visitor) override;
};
