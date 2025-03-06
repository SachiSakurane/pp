#pragma once

#include <vector>

#include <pp/observable/type.hpp>
#include <pp/observer.hpp>

namespace pp {
template <std::copy_constructible Type>
class behavior {
public:
  using value_type = Type;
  using observer_value_type = value_type;
  using observer_type = pp::observer<observer_value_type>;
  using subscription_type = std::shared_ptr<observer_type>;
  using observable_type = pp::observable::hot;

  behavior(value_type initial_value) : current_value{initial_value} {}

  template <class... Args>
  behavior(Args &&...args) : current_value{std::forward<Args>(args)...} {}

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

  operator observer_value_type() const { return current_value; }
  observer_value_type get() const { return current_value; }

  void next(const value_type &v) {
    current_value = v;
    notify(current_value);
  }

  void next(value_type &&v) {
    current_value = std::move(v);
    notify(current_value);
  }

  subscription_type subscribe(observer_type &&o) {
    auto subscription = std::make_shared<observer_type>(std::move(o));
    auto &oo = observers.emplace_back(subscription);
    if (auto l = oo.lock(); l) {
      (*l)(current_value);
    }
    return subscription;
  }

  void notify(const observer_value_type &v) {
    observers.erase(std::remove_if(std::begin(observers), std::end(observers),
                                   [](auto s) { return s.expired(); }),
                    std::end(observers));
    for (auto o : observers) {
      if (auto l = o.lock(); l) {
        (*l)(v);
      }
    }
  }

private:
  value_type current_value;
  std::vector<std::weak_ptr<observer_type>> observers;
};
} // namespace pp
