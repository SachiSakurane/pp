#pragma once

#include <pp/observable/cold_observable.hpp>

namespace pp {
template <std::copy_constructible Type>
class subject : public pp::observable::cold_observable<Type> {
public:
  using value_type = Type;
  using observer_value_type = value_type;
  using observer_type = pp::observer<observer_value_type>;
  using subscription_type = std::shared_ptr<observer_type>;
  using observable_type = pp::observable::cold;

  subject() = default;
  subject(const subject &) = default;
  subject(subject &&) = default;

  subject &operator=(const value_type &value) {
    this->next(value);
    return *this;
  }

  subject &operator=(value_type &&value) {
    this->next(std::move(value));
    return *this;
  }

  void next(const value_type &v) override { pp::observable::cold_observable<Type>::notify(v); }
  void next(value_type &&v) override { pp::observable::cold_observable<Type>::notify(v); }
};
} // namespace pp
