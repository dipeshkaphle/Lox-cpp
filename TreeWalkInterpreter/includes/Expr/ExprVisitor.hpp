#pragma once

#include <any>
struct binary_expr;
struct call_expr;
struct unary_expr;
struct grouping_expr;
struct literal_expr;
struct variable_expr;
struct assign_expr;
struct logical_expr;

template <typename T> class expr_visitor {
public:
  virtual T visit_binary_expr(binary_expr &exp) = 0;
  virtual T visit_unary_expr(unary_expr &exp) = 0;
  virtual T visit_grouping_expr(grouping_expr &exp) = 0;
  virtual T visit_literal_expr(literal_expr &exp) = 0;
  virtual T visit_variable_expr(variable_expr &exp) = 0;
  virtual T visit_assign_expr(assign_expr &exp) = 0;
  virtual T visit_logical_expr(logical_expr &exp) = 0;
  virtual T visit_call_expr(call_expr &exp) = 0;
};
