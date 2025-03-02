#pragma once

#include <pp/observer.hpp>

namespace pp {
template <std::copy_constructible Type, observable ObserverType = pp::observer<Type>>
class subject : public ObserverType {
public:
  using value_type = ObserverType::value_type;
  using subscription_type = pp::subscription<value_type>;

  subject() {}

  subject(const subject &) = default;
  subject(subject &&) = default;

  subject &operator=(const value_type &value) {
    this->current_value = value;
    this->notify(observe());
    return *this;
  }
  subject &operator=(value_type &&value) {
    this->current_value = std::move(value);
    this->notify(observe());
    return *this;
  }

  void subscribe(std::weak_ptr<subscription_type> s) override {
    ObserverType::subscriptions.emplace_back(s);
  }

  value_type observe() const override { return current_value.value(); }

private:
  std::optional<value_type> current_value;
};
} // namespace pp
