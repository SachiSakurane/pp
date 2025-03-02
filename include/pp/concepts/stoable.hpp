#pragma once

#include <type_traits>

#include <pp/store.hpp>

namespace pp {
template <class Type>
concept stoable = std::is_base_of_v<pp::store, Type>;
} // namespace pp
