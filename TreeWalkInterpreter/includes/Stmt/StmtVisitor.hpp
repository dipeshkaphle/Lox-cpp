#pragma once

#include <any>
class expr_stmt;
class print_stmt;
class let_stmt;
class while_stmt;
class block_stmt;
class if_stmt;

template <typename T> class stmt_visitor {
public:
  virtual T visit_print_stmt(print_stmt &stmt) = 0;
  virtual T visit_expr_stmt(expr_stmt &stmt) = 0;
  virtual T visit_let_stmt(let_stmt &stmt) = 0;
  virtual T visit_block_stmt(block_stmt &stmt) = 0;
  virtual T visit_if_stmt(if_stmt &stmt) = 0;
  virtual T visit_while_stmt(while_stmt &stmt) = 0;
};
