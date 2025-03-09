#pragma once

#include <memory>
#include <vector>

#include <pp/storable.hpp>

namespace pp {
template <class ValueType, class ObserverType>
class subscription : public pp::storable {
public:
  using value_type = ValueType;
  using observer_type = ObserverType;
  using weak_type = std::weak_ptr<observer_type>;
  using observers_type = std::vector<weak_type>;

  subscription() = default;
  subscription(observer_type &&o) : observer{std::make_shared<observer_type>(o)} {}

  weak_type get_weak() const { return observer; }

private:
  std::shared_ptr<observer_type> observer;
};
} // namespace pp
