#pragma once

#include <pp/observable/hot_observable.hpp>

namespace pp {
template <std::copy_constructible Type>
class accessor : public pp::observable::hot_observable<Type> {
public:
  using value_type = Type;
  using observer_value_type = value_type;
  using observer_type = pp::observer<observer_value_type>;
  using subscription_type = pp::subscription<value_type, observer_type>;
  using observable_type = pp::observable::hot;

  accessor(std::function<value_type()> &&get, std::function<void(value_type)> &&set)
      : getter{std::move(get)}, setter{std::move(set)} {}

  accessor &operator=(const value_type &value) {
    this->setter(value);
    this->next(get());
    return *this;
  }
  accessor &operator=(value_type &&value) {
    this->setter(value);
    this->next(get());
    return *this;
  }

  operator observer_value_type() const { return getter(); }
  observer_value_type get() const override { return getter(); }

  void next(const value_type &v) override {
    setter(v);
    pp::observable::hot_observable<Type>::notify(getter());
  }

  void next(value_type &&v) override {
    setter(v);
    pp::observable::hot_observable<Type>::notify(getter());
  }

private:
  const std::function<value_type()> getter;
  const std::function<void(value_type)> setter;
};
} // namespace pp
