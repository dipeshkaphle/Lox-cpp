#include "includes/Interpreter.hpp"
#include "includes/Expr/assign_expr.hpp"
#include "includes/Expr/binary_expr.hpp"
#include "includes/Expr/grouping_expr.hpp"
#include "includes/Expr/literal_expr.hpp"
#include "includes/Expr/logical_expr.hpp"
#include "includes/Expr/unary_expr.hpp"
#include "includes/Expr/variable_expr.hpp"
#include "includes/Lox.hpp"
#include "includes/Stmt/BlockStmt.hpp"
#include "includes/Stmt/ExprStmt.hpp"
#include "includes/Stmt/IfStmt.hpp"
#include "includes/Stmt/LetStmt.hpp"
#include "includes/Stmt/PrintStmt.hpp"
#include "includes/Stmt/WhileStmt.hpp"
#include "includes/TokenTypes.hpp"
#include "includes/runtime_err.hpp"

#include "fmt/core.h"

// TODO : Make all the visit expr and visit stmt return tl::expected<any,Err>

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

string interpreter::stringify(const std::any &obj) {
  if (!obj.has_value()) {
    return "nil";
  }
  if (obj.type() == typeid(double)) {
    auto num = any_cast<double>(obj);
    if (num == (long long)(num)) {
      return to_string((long long)num);
    }
    return to_string(num);
  }
  if (obj.type() == typeid(bool)) {
    return (any_cast<bool>(obj)) ? "true" : "false";
  }
  return any_cast<string>(obj);
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
  if (!l.has_value() && !r.has_value()) {
    return true;
  }
  if (!l.has_value() || !r.has_value()) {
    return false;
  }
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
    if (any_cast<double>(right) == 0) {
      throw Lox_runtime_err(exp.op, "Division by zero");
    }
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

std::any interpreter::visit_variable_expr(const variable_expr &exp) const {
  auto maybe_val = this->env.get(exp.name);
  /*
   *
   * Will be trigerred when i do
   * >>> x;
   *
   */
  if (maybe_val.has_value()) {
    return maybe_val.value();
  }
  throw move(maybe_val.error());
}

std::any interpreter::visit_assign_expr(const assign_expr &exp) const {
  auto val = this->evaluate(*exp.value);
  auto res = this->env.assign(exp.name, val);
  /*
   *
   * Will be trigerred when i do
   * >>> x=3;
   *
   */
  if (res.has_value()) {
    return res.value();
  }
  throw move(res.error());
}

std::any interpreter::visit_logical_expr(const logical_expr &exp) const {
  /*
   * Doesnt return true or false necessarily
   * Will return the truthy equivalent value
   *
   * >>>  print "hi" or "hello" => "hi"
   * >>>  print nil or nil => nil
   * >>>  print "hi" and "hello"  => "hello" (because it is evaluated last)
   * >>>  print nil or "hi" => "hi"
   */
  auto lhs = this->evaluate(*exp.left);
  if (exp.op.type == TokenType::OR) {
    if (is_truthy(lhs)) {
      return lhs;
    }
  } else {
    if (!is_truthy(lhs)) {
      return left;
    }
  }
  return this->evaluate(*exp.right);
}

std::any interpreter::evaluate(const Expr &exp) const {
  return exp.accept(*this);
}

std::any interpreter::execute(Stmt &stmt) { return stmt.accept(*this); }

std::any interpreter::execute_block(vector<stmt_ptr> &stmts) {
  // pushing a stack frame
  this->env.push_frame();
  try {
    std::any ret_val{};
    for (auto &stmt : stmts) {
      ret_val = this->execute(*stmt);
      /*
       * This can only ever be true when we are executing  a block inside
       * a loop
       *
       * My reasoning behind that is no break statement can be inside a block
       * that is not a block of a while loop or for loop, because I'm
       * considering that as a parse error, meaning the program cant get to
       * evaluation stage at all if the break was not inside a loop.
       */
      if (this->break_from_current_loop) {
        break;
      }
    }
    this->env.pop_frame();
    return ret_val;
  } catch (Lox_runtime_err &err) {
    // popping stack frame
    this->env.pop_frame();
    throw std::move(err);
  }
}

std::any interpreter::visit_print_stmt(print_stmt &stmt) {
  auto val = this->evaluate(*stmt.expr);
  fmt::print("{}\n", this->stringify(val));
  return val;
}

std::any interpreter::visit_expr_stmt(expr_stmt &stmt) {
  auto val = this->evaluate(*stmt.expr);
  return val;
}

std::any interpreter::visit_let_stmt(let_stmt &stmt) {
  auto val = this->evaluate(*stmt.initializer_expr);
  this->env.define(stmt.name.lexeme, val);
  return val;
}

std::any interpreter::visit_block_stmt(block_stmt &stmt) {
  auto val = this->execute_block(stmt.statements);
  return val;
}

std::any interpreter::visit_if_stmt(if_stmt &stmt) {
  if (is_truthy(this->evaluate(*stmt.condition))) {
    return this->execute(*stmt.then_branch);
  }
  if (stmt.else_branch.has_value()) {
    return this->execute(*stmt.else_branch.value());
  }
  return {};
}

std::any interpreter::visit_while_stmt(while_stmt &stmt) {
  while (this->is_truthy(this->evaluate(*stmt.condition))) {
    this->execute(*stmt.body);
    if (this->break_from_current_loop) {
      this->break_from_current_loop = false;
      break;
    }
  }
  return {};
}

std::any interpreter::visit_break_stmt(break_stmt &_stmt) {
  this->break_from_current_loop = true;
  return {};
  // while (this->is_truthy(this->evaluate(*stmt.condition))) {
  // this->execute(*stmt.body);
  // }
  // return {};
}

void interpreter::interpret(vector<std::unique_ptr<Stmt>> &stmts,
                            bool is_repl) {
  try {
    std::ranges::for_each(stmts, [&](auto &stmt) {
      auto val = this->execute(*stmt);
      if (is_repl) {
        fmt::print("=> {}\n", this->stringify(val));
      }
    });
  } catch (Lox_runtime_err &err) {
    Lox::report_runtime_error(err);
  }
}
