#pragma once

#include "includes/Callable.hpp"
#include "includes/Environment.hpp"

#include <chrono>
#include <memory>

class clock_fn : public Callable {
  size_t arity() final { return 0; }

  std::any call([[maybe_unused]] interpreter &intrptr,
                [[maybe_unused]] std::vector<std::any> &arguments) final {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return (double)millis;
  }
  std::string to_string() const final { return "<native_fn clock >"; }
};

namespace GlobalEnv {
static inline clock_fn clock;

static inline Environment init() {
  Environment global{};
  global.define("clock", make_any<Callable *>(&clock));
  return global;
}
} // namespace GlobalEnv
