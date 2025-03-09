#pragma once

#include <pp/observable/type.hpp>
#include <pp/observer.hpp>
#include <pp/subscription.hpp>

namespace pp::observable {
template <std::copy_constructible Type, std::copy_constructible ObserverType = Type>
class hot_observable {
public:
  using value_type = Type;
  using observer_value_type = ObserverType;
  using observer_type = pp::observer<observer_value_type>;
  using subscription_type = pp::subscription<value_type, observer_type>;
  using observable_type = pp::observable::hot;

  virtual ~hot_observable() = default;

  virtual observer_value_type get() const = 0;
  virtual void next(const value_type &v) = 0;
  virtual void next(value_type &&v) = 0;

  virtual subscription_type subscribe(observer_type &&o) {
    auto subscription = subscription_type{std::forward<observer_type>(o)};
    auto &oo = observers.emplace_back(subscription.get_weak());
    if (auto l = oo.lock(); l) {
      (*l)(get());
    }
    return subscription;
  }

  virtual void notify(const observer_value_type &v) {
    observers.erase(std::remove_if(std::begin(observers), std::end(observers),
                                   [](auto s) { return s.expired(); }),
                    std::end(observers));
    for (auto o : observers) {
      if (auto l = o.lock(); l) {
        (*l)(v);
      }
    }
  }

protected:
  subscription_type::observers_type observers;
};
} // namespace pp::observable
