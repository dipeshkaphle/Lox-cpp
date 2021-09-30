#pragma once

#include <any>
class expr_stmt;
class print_stmt;
class let_stmt;

template <typename T> class stmt_visitor {
public:
  virtual T visit_print_stmt(print_stmt &exp) = 0;
  virtual T visit_expr_stmt(expr_stmt &exp) = 0;
  virtual T visit_let_stmt(let_stmt &exp) = 0;
};
