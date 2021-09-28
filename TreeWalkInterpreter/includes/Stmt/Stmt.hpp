#pragma once
#include <any>

#include "StmtVisitor.hpp"

class Stmt {
public:
  virtual ~Stmt() = default;
  virtual std::any accept(const stmt_visitor<std::any> &visitor) const = 0;
};
