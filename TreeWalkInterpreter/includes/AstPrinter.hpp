#pragma once
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Expr/binary_expr.hpp"
#include "Expr/grouping_expr.hpp"
#include "Expr/literal_expr.hpp"
#include "Expr/unary_expr.hpp"
#include "Expr/variable_expr.hpp"

#include <string>

class ast_printer : public expr_visitor<std::any> {
private:
  std::string parenthesize(std::string name, Expr &expr);

  std::string parenthesize(std::string name, Expr &expr1, Expr &expr2);

public:
  std::string print(Expr &expr);
  std::any visit_binary_expr(binary_expr &expr) override;
  std::any visit_unary_expr(unary_expr &expr) override;
  std::any visit_grouping_expr(grouping_expr &expr) override;
  std::any visit_literal_expr(literal_expr &expr) override;
  std::any visit_variable_expr(variable_expr &exp) override;
};
