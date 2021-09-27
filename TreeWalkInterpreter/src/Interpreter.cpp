#include "includes/Interpreter.hpp"
#include "includes/Expr/binary_expr.hpp"
#include "includes/Expr/grouping_expr.hpp"
#include "includes/Expr/literal_expr.hpp"
#include "includes/Expr/unary_expr.hpp"
#include "includes/Lox.hpp"
#include "includes/TokenTypes.hpp"
#include "includes/runtime_err.hpp"

#define REF(x) std::reference_wrapper((x))

template <std::size_t N>
void interpreter::check_number_operands(
    const Token &tok, std::array<std::reference_wrapper<std::any>, N> exprs) {
  if (all_of(exprs.begin(), exprs.end(),
             [](const std::reference_wrapper<std::any> &expr) {
               return expr.get().type() == typeid(double);
             })) {
    return;
  }
  throw Lox_runtime_err(tok, "Operand must be number");
}

bool interpreter::is_truthy(const std::any &val) {
  // Lox follows Ruby’s simple rule: false and nil are falsey, and everything
  // else is truthy. We implement that like so:

  if (!val.has_value()) {
    return false;
  }
  if (val.type() == typeid(bool)) {
    return any_cast<bool>(val);
  }
  if (val.type() == typeid(nullptr)) {
    return false;
  }
  return true;
}

bool interpreter::is_equal(const std::any &l, const std::any &r) {
  if (!l.has_value() && !r.has_value())
    return true;
  if (!l.has_value() || !r.has_value())
    return false;
  if (l.type() == r.type()) {
    if (l.type() == typeid(double)) {
      return any_cast<double>(l) == any_cast<double>(r);
    }
    if (l.type() == typeid(string)) {
      return any_cast<string>(l) == any_cast<string>(r);
    }
    if (l.type() == typeid(bool)) {
      return any_cast<bool>(l) == any_cast<bool>(r);
    }
  }
  return false;
}

std::any interpreter::visit_binary_expr(const binary_expr &exp) const {
  std::any left = this->evaluate(*exp.left);
  std::any right = this->evaluate(*exp.right);
  switch (exp.op.type) {
  case TokenType::GREATER:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) > any_cast<double>(right);
  case TokenType::GREATER_EQUAL:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) >= any_cast<double>(right);
  case TokenType::LESS:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) < any_cast<double>(right);
  case TokenType::LESS_EQUAL:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) <= any_cast<double>(right);
  case TokenType::BANG_EQUAL:
    return !interpreter::is_equal(left, right);
  case TokenType::EQUAL_EQUAL:
    return interpreter::is_equal(left, right);
  case TokenType::MINUS:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) - any_cast<double>(right);
  case TokenType::PLUS:
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
      return any_cast<double>(left) + any_cast<double>(right);
    }
    if (left.type() == typeid(string) && right.type() == typeid(string)) {
      return any_cast<string>(left) + any_cast<string>(right);
    }
    throw Lox_runtime_err(exp.op,
                          "Operands must be two numbers or two strings");

  case TokenType::SLASH:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) / any_cast<double>(right);
  case TokenType::STAR:
    check_number_operands(exp.op, array{REF(left), REF(right)});
    return any_cast<double>(left) * any_cast<double>(right);
  default:
    break;
  }
  return {};
}

std::any interpreter::visit_unary_expr(const unary_expr &exp) const {
  std::any right = this->evaluate(*exp.right);
  switch (exp.op.type) {
  case TokenType::MINUS:
    return -(std::any_cast<double>(right));
  case TokenType::BANG:
    return !interpreter::is_truthy(right);
  default:
    break;
  }
  return {};
}

std::any interpreter::visit_grouping_expr(const grouping_expr &exp) const {
  return this->evaluate(*exp.expression);
}

std::any interpreter::visit_literal_expr(const literal_expr &exp) const {
  return exp.value;
}

std::any interpreter::evaluate(const Expr &exp) const {
  return exp.accept(*this);
}

void interpreter::interpret(const Expr &exp) const {
  try {
    std::any val = this->evaluate(exp);
    cout << interpreter::stringify(val) << '\n';
  } catch (Lox_runtime_err &err) {
    // Lox::report_runtime_error(error);
  }
}
