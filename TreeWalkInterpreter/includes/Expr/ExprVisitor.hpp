#pragma once

#include <any>
class binary_expr;
class unary_expr;
class grouping_expr;
class literal_expr;

template <typename T> class expr_visitor {
public:
  virtual T visit_binary_expr(const binary_expr &exp) const = 0;
  virtual T visit_unary_expr(const unary_expr &exp) const = 0;
  virtual T visit_grouping_expr(const grouping_expr &exp) const = 0;
  virtual T visit_literal_expr(const literal_expr &exp) const = 0;
};