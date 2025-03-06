#pragma once

#include <memory>
#include <vector>

#include <pp/observable/type.hpp>
#include <pp/observer.hpp>

namespace pp::observable {
template <std::copy_constructible Type, std::copy_constructible ObserverType = Type>
class hot_observable {
public:
  using value_type = Type;
  using observer_type = pp::observer<ObserverType>;
  using subscription_type = std::shared_ptr<observer_type>;
  using observable_type = pp::observable::hot;

  virtual void next(const value_type &v) { notify(current_value); }
  virtual void next(value_type &&v) { notify(current_value); }

  virtual subscription_type subscribe(observer_type &&o) {
    auto subscription = std::make_shared<observer_type>(std::move(o));
    observers.emplace_back(subscription);
    return subscription;
  }

  virtual void notify() = 0;

protected:
  std::vector<std::weak_ptr<observer_type>> observers;
};
} // namespace pp::observable
