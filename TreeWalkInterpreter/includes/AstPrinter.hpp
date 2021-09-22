#pragma once
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Expr/binary_expr.hpp"
#include "Expr/grouping_expr.hpp"
#include "Expr/literal_expr.hpp"
#include "Expr/unary_expr.hpp"

#include <string>

class ast_printer : public expr_visitor<std::any> {
private:
  std::string parenthesize(std::string name, const Expr &expr) const;

  std::string parenthesize(std::string name, const Expr &expr1,
                           const Expr &expr2) const;

public:
  std::string print(const Expr &expr) const;
  std::any visit_binary_expr(const binary_expr &expr) const override;
  std::any visit_unary_expr(const unary_expr &expr) const override;
  std::any visit_grouping_expr(const grouping_expr &expr) const override;
  std::any visit_literal_expr(const literal_expr &expr) const override;
};
