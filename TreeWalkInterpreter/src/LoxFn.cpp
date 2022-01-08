#include "includes/LoxFn.hpp"
#include "includes/Interpreter.hpp"

#include <algorithm>
#include <fmt/core.h>

LoxFn::LoxFn(fn_stmt fn) : fn_decl(move(fn)) {}

size_t LoxFn::arity() { return this->fn_decl.params.size(); }

std::any LoxFn::call([[maybe_unused]] interpreter &intrptr,
                     [[maybe_unused]] std::vector<std::any> &arguments) {
  try {
    intrptr.get_env().push_frame();

    vector<any> evaluated;
    // std::ranges::transform(arguments,back_inserter(evaluated), [](auto &arg){
    // return intrptr.get_env().define()
    // } );
    std::ranges::for_each(fn_decl.params, [&, i = 0](auto &arg) mutable {
      intrptr.get_env().define(arg.lexeme, arguments[i++]);
    });

    auto ret_val = intrptr.execute_block(fn_decl.body);

    intrptr.get_env().pop_frame();
    return ret_val;
  } catch (Lox_runtime_err &err) {
    intrptr.get_env().pop_frame();
    throw std::move(err);
  }
}

std::string LoxFn::to_string() const {
  return fmt::format("<fn {} >", fn_decl.name.lexeme);
}
