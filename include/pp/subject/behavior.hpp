#pragma once

#include <pp/observable/hot_observable.hpp>

namespace pp {
template <std::copy_constructible Type>
class behavior : public pp::observable::hot_observable<Type> {
public:
  using value_type = Type;
  using observer_value_type = value_type;
  using observer_type = pp::observer<observer_value_type>;
  using subscription_type = pp::subscription<value_type, observer_type>;
  using observable_type = pp::observable::hot;

  behavior(value_type initial_value) { current_value.emplace(initial_value); }

  template <class... Args>
  behavior(Args &&...args) {
    current_value.emplace(std::forward<Args>(args)...);
  }

  behavior(const behavior &) = default;
  behavior(behavior &&) = default;

  behavior &operator=(const value_type &value) {
    this->next(value);
    return *this;
  }

  behavior &operator=(value_type &&value) {
    this->next(std::move(value));
    return *this;
  }

  operator observer_value_type() const { return current_value.value(); }
  observer_value_type get() const override { return current_value.value(); }

  void next(const value_type &v) override {
    current_value.emplace(v);
    pp::observable::hot_observable<Type>::notify(current_value.value());
  }

  void next(value_type &&v) override {
    current_value.emplace(std::move(v));
    pp::observable::hot_observable<Type>::notify(current_value.value());
  }

private:
  std::optional<value_type> current_value;
};
} // namespace pp
