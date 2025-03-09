#pragma once

#include <any>
#include <vector>

#include <pp/concepts/storable.hpp>

namespace pp {
class storage {
public:
  storage() = default;

  storage &operator+=(pp::concepts::storable auto &&s) {
    this->store.emplace_back(std::move(s));
    return *this;
  }

  std::size_t size() const { return store.size(); }

private:
  std::vector<std::any> store;
};
} // namespace pp
