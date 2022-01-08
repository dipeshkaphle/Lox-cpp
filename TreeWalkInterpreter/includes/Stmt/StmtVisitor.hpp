#pragma once

#include <any>
class expr_stmt;
class print_stmt;
class let_stmt;
class while_stmt;
class break_stmt;
class continue_stmt;
class block_stmt;
class if_stmt;
class fn_stmt;
class return_stmt;

template <typename T> class stmt_visitor {
public:
  virtual T visit_print_stmt(print_stmt &stmt) = 0;
  virtual T visit_expr_stmt(expr_stmt &stmt) = 0;
  virtual T visit_let_stmt(let_stmt &stmt) = 0;
  virtual T visit_block_stmt(block_stmt &stmt) = 0;
  virtual T visit_if_stmt(if_stmt &stmt) = 0;
  virtual T visit_while_stmt(while_stmt &stmt) = 0;
  virtual T visit_break_stmt(break_stmt &stmt) = 0;
  virtual T visit_continue_stmt(continue_stmt &stmt) = 0;
  virtual T visit_fn_stmt(fn_stmt &stmt) = 0;
  virtual T visit_return_stmt(return_stmt &stmt) = 0;
};
