#pragma once

#include "Expr/ExprVisitor.hpp"
#include "includes/Expr/Expr.hpp"
#include "includes/Token.hpp"

#include <any>
#include <array>

class interpreter : public expr_visitor<std::any> {
private:
  std::any visit_binary_expr(const binary_expr &exp) const override;
  std::any visit_unary_expr(const unary_expr &exp) const override;
  std::any visit_grouping_expr(const grouping_expr &exp) const override;
  std::any visit_literal_expr(const literal_expr &exp) const override;
  std::any evaluate(const Expr &exp) const;
  static bool is_truthy(const std::any &val);
  static bool is_equal(const std::any &l, const std::any &r);
  static string stringify(const std::any &obj) {
    if (!obj.has_value())
      return "nil";
    if (obj.type() == typeid(double)) {
      auto num = any_cast<double>(obj);
      if (num == (long long)(num)) {
        return to_string((long long)num);
      }
      return to_string(num);
    }
    if (obj.type() == typeid(bool))
      return (any_cast<bool>(obj)) ? "true" : "false";
    return any_cast<string>(obj);
  }

  template <std::size_t N>
  static void
  check_number_operands(const Token &tok,
                        std::array<std::reference_wrapper<std::any>, N>);

public:
  void interpret(const Expr &exp) const;
};
