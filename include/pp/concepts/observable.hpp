#pragma once

#include <concepts>
#include <memory>

#include <pp/observable/type.hpp>
#include <pp/observer.hpp>

namespace pp::concepts {
template <class ObservableType>
concept observable = std::copy_constructible<typename ObservableType::value_type> &&
                     requires(ObservableType o, typename ObservableType::observer_type &&obs,
                              typename ObservableType::value_type v,
                              const typename ObservableType::observer_value_type &ov) {
                       o.next(v);
                       o.notify(ov);
                       {
                         o.subscribe(std::move(obs))
                       } -> std::convertible_to<typename ObservableType::subscription_type>;
                     };

template <class ObservableType>
concept hot_observable =
    observable<ObservableType> &&
    std::is_same_v<typename ObservableType::observable_type, pp::observable::hot> &&
    requires(ObservableType o, const ObservableType co) {
      { co.get() } -> std::convertible_to<typename ObservableType::observer_value_type>;
    };

template <class ObservableType>
concept cold_observable =
    observable<ObservableType> &&
    std::is_same_v<typename ObservableType::observable_type, pp::observable::cold>;
} // namespace pp::concepts
