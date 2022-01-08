#include "includes/Stmt/Stmt.hpp"
#include "includes/Stmt/BlockStmt.hpp"
#include "includes/Stmt/BreakStmt.hpp"
#include "includes/Stmt/ContinueStmt.hpp"
#include "includes/Stmt/ExprStmt.hpp"
#include "includes/Stmt/FnStmt.hpp"
#include "includes/Stmt/IfStmt.hpp"
#include "includes/Stmt/LetStmt.hpp"
#include "includes/Stmt/PrintStmt.hpp"
#include "includes/Stmt/ReturnStmt.hpp"
#include "includes/Stmt/WhileStmt.hpp"

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

print_stmt::print_stmt(std::unique_ptr<Expr> expr, bool new_line)
    : expr(std::move(expr)), has_newline(new_line) {}

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

/*
 * =============================================================================
 * Block Statement
 */
block_stmt::block_stmt(std::vector<stmt_ptr> stmts)
    : statements(std::move(stmts)) {}
block_stmt::block_stmt(block_stmt &&stmt) noexcept
    : statements(std::move(stmt.statements)) {}
std::any block_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_block_stmt(*this);
}

/*
 * Block Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * If Statement
 */

if_stmt::if_stmt(std::unique_ptr<Expr> condition_,
                 std::unique_ptr<Stmt> then_branch_,
                 std::optional<std::unique_ptr<Stmt>> else_branch_)
    : condition(std::move(condition_)), then_branch(std::move(then_branch_)),
      else_branch(std::move(else_branch_)) {}

std::any if_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_if_stmt(*this);
}

/*
 * If Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * While Statement
 */

while_stmt::while_stmt(std::unique_ptr<Expr> condition_,
                       std::unique_ptr<Stmt> body_,
                       std::optional<std::unique_ptr<Stmt>> change_fn_)
    : condition(std::move(condition_)), body(std::move(body_)),
      change_fn(move(change_fn_)) {}

std::any while_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_while_stmt(*this);
}

/*
 * While Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Break Statement
 */

break_stmt::break_stmt() = default;
std::any break_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_break_stmt(*this);
};
/*
 * Break Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Continue Statement
 */

continue_stmt::continue_stmt() = default;
std::any continue_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_continue_stmt(*this);
};
/*
 * Continue Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Fn Statement
 */

fn_stmt::fn_stmt(Token fn_name, vector<Token> params, vector<stmt_ptr> fn_body)
    : name(move(fn_name)), params(move(params)), body(move(fn_body)) {}

std::any fn_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_fn_stmt(*this);
}

/*
 * Fn Statement End
 *  =============================================================================
 */

/*
 * =============================================================================
 * Return Statement
 */

return_stmt::return_stmt(Token keyword, optional<unique_ptr<Expr>> val)
    : keyword(move(keyword)), value(move(val)) {}
std::any return_stmt::accept(stmt_visitor<std::any> &visitor) {
  return visitor.visit_return_stmt(*this);
}

/*
 * Return Statement End
 *  =============================================================================
 */
