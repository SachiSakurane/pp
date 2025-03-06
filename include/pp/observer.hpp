#pragma once

#include <functional>

namespace pp {
  template <typename T>
  using observer = std::function<void(T)>;
} // namespace pp
