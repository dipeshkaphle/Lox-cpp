#pragma once
#include "Expr/Expr.hpp"
#include "Stmt/Stmt.hpp"

#include <string>

class ast_printer : public expr_visitor<std::any>, stmt_visitor<std::any> {
private:
  std::string parenthesize(std::string name, Expr &expr);

  std::string parenthesize(std::string name, Expr &expr1, Expr &expr2);

public:
  std::string print(Expr &expr);
  std::any visit_binary_expr(binary_expr &exp) final;
  std::any visit_unary_expr(unary_expr &exp) final;
  std::any visit_grouping_expr(grouping_expr &exp) final;
  std::any visit_literal_expr(literal_expr &exp) final;
  std::any visit_variable_expr(variable_expr &exp) final;
  std::any visit_assign_expr(assign_expr &exp) final;
  std::any visit_logical_expr(logical_expr &exp) final;
  std::any visit_call_expr(call_expr &exp) final;

  std::string print(Stmt &stmt);
  std::any visit_print_stmt(print_stmt &stmt) final;
  std::any visit_expr_stmt(expr_stmt &stmt) final;
  std::any visit_let_stmt(let_stmt &stmt) final;
  std::any visit_block_stmt(block_stmt &stmt) final;
  std::any visit_if_stmt(if_stmt &stmt) final;
  std::any visit_while_stmt(while_stmt &stmt) final;
  std::any visit_break_stmt(break_stmt &stmt) final;
  std::any visit_continue_stmt(continue_stmt &stmt) final;
  std::any visit_fn_stmt(fn_stmt &stmt) final;
  std::any visit_return_stmt(return_stmt &stmt) final;
};
