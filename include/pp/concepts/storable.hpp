#pragma once

#include <type_traits>

#include <pp/storable.hpp>

namespace pp::concepts {
template <class Type>
concept storable = std::is_base_of_v<pp::storable, Type>;
}
