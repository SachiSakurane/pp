#pragma once

#include <pp/observer.hpp>

namespace pp {
template <std::copy_constructible Type, observable ObserverType = pp::observer<Type>>
class behavior : public ObserverType {
public:
  using value_type = ObserverType::value_type;
  using subscription_type = pp::subscription<value_type>;

  behavior(value_type initial_value) : current_value{initial_value} {}

  template <class... Args>
  behavior(Args &&...args) : current_value{std::forward<Args>(args)...} {}

  behavior(const behavior &) = default;
  behavior(behavior &&) = default;

  behavior &operator=(const value_type &value) {
    this->current_value = value;
    this->notify(observe());
    return *this;
  }
  behavior &operator=(value_type &&value) {
    this->current_value = std::move(value);
    this->notify(observe());
    return *this;
  }

  operator value_type() const { return current_value; }
  value_type observe() const override { return current_value; }

  void subscribe(std::weak_ptr<subscription_type> s) override { ObserverType::subscribe(s); }

private:
  value_type current_value;
};
} // namespace pp
