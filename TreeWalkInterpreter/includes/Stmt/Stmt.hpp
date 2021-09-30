#pragma once
#include <any>

#include "StmtVisitor.hpp"

class Stmt {
public:
  virtual ~Stmt() = default;
  virtual std::any accept(stmt_visitor<std::any> &visitor) = 0;
};
