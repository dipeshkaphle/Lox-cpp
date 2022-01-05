#include "includes/AstPrinter.hpp"

std::string ast_printer::parenthesize(std::string name, Expr &expr) {
  std::string str;
  str.append("( ").append(name);
  str.append(" ");
  str.append(this->print(expr));
  str.append(")");
  return str;
}

std::string ast_printer::parenthesize(std::string name, Expr &expr1,
                                      Expr &expr2) {
  std::string str{};
  str.append("( ").append(name);
  str.append(" ");
  str.append(this->print(expr1));
  str.append(" ");
  str.append(this->print(expr2));
  str.append(")");
  return str;
}

std::string ast_printer::print(Expr &expr) {
  return std::any_cast<std::string>(expr.accept(*this));
}

std::any ast_printer::visit_binary_expr(binary_expr &expr) {
  return this->parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::any ast_printer::visit_unary_expr(unary_expr &expr) {
  return parenthesize(expr.op.lexeme, *expr.right);
}

std::any ast_printer::visit_grouping_expr(grouping_expr &expr) {
  return parenthesize("group", *expr.expression);
}

std::any ast_printer::visit_variable_expr(variable_expr &exp) {
  return exp.name.lexeme;
}

std::any ast_printer::visit_literal_expr(literal_expr &expr) {
  if (!expr.value.has_value())
    return std::string("nil");
  if (expr.value.type() == typeid(double)) {
    return to_string(any_cast<double>(expr.value));
  } else if (expr.value.type() == typeid(std::string)) {
    return any_cast<std::string>(expr.value);
  } else if (expr.value.type() == typeid(bool)) {
    if (any_cast<bool>(expr.value) == true)
      return std::string("true");
    else
      return std::string("false");
  }
  return std::string("nil");
}
