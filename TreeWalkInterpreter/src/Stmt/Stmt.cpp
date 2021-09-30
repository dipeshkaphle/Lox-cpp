#include "includes/Stmt/Stmt.hpp"
#include "includes/Stmt/ExprStmt.hpp"
#include "includes/Stmt/LetStmt.hpp"
#include "includes/Stmt/PrintStmt.hpp"

/*
 * =============================================================================
 * Expression Statement
 */

expr_stmt::expr_stmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

std::any expr_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_expr_stmt(*this);
}

/*
 * Expression Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Print Statement
 */

print_stmt::print_stmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {}

std::any print_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_print_stmt(*this);
}

/*
 * Print Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Let Statement
 */
let_stmt::let_stmt(Token _name, std::unique_ptr<Expr> _expr)
    : name(std::move(_name)), initializer_expr(std::move(_expr)) {}
std::any let_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_let_stmt(*this);
}

/*
 * Let Statement End
 *  =============================================================================
 */
