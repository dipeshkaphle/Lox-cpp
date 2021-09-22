#pragma once

#include "Expr/ExprVisitor.hpp"
#include <any>

class interpreter : public expr_visitor<std::any> {
private:
  std::any visit_binary_expr(const binary_expr &exp) const override;
  std::any visit_unary_expr(const unary_expr &exp) const override;
  std::any visit_grouping_expr(const grouping_expr &exp) const override;
  std::any visit_literal_expr(const literal_expr &exp) const override;
};
