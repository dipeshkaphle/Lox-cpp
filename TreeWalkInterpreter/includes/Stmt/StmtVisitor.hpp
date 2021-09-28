#pragma once

#include <any>
class expr_stmt;
class print_stmt;

template <typename T> class stmt_visitor {
public:
  virtual T visit_print_stmt(const print_stmt &exp) const = 0;
  virtual T visit_expr_stmt(const expr_stmt &exp) const = 0;
};
