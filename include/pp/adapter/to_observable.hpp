#pragma once

#include <optional>

#include <pp/concepts/observable.hpp>
#include <pp/observable/hot_observable.hpp>

namespace pp {
namespace detail {
  class to_observable_ptr_tag {};

  template <pp::concepts::hot_observable O>
  class hot_observable_wrapper_r
      : public pp::observable::hot_observable<typename O::value_type,
                                              typename O::observer_value_type> {
  public:
    using value_type = typename O::value_type;
    using observer_value_type = typename O::observer_value_type;
    using observer_type = pp::observer<observer_value_type>;
    using subscription_type = std::shared_ptr<observer_type>;
    using observable_type = pp::observable::hot;

    hot_observable_wrapper_r(O &o) : obs{o} {
      subscription = obs->get().subscribe([&](const auto &v) {
        pp::observable::hot_observable<typename O::value_type,
                                       typename O::observer_value_type>::notify(v);
      });
    }

    observer_value_type get() const override { return obs->get().get(); }
    void next(const value_type &v) override { obs->get().next(v); }
    void next(value_type &&v) override { obs->get().next(v); }

  private:
    O::subscription_type subscription;
    std::optional<std::reference_wrapper<O>> obs;
  };

  template <pp::concepts::hot_observable O>
  class hot_observable_wrapper
      : public pp::observable::hot_observable<typename O::value_type,
                                              typename O::observer_value_type> {
  public:
    using value_type = typename O::value_type;
    using observer_value_type = typename O::observer_value_type;
    using observer_type = pp::observer<observer_value_type>;
    using subscription_type = std::shared_ptr<observer_type>;
    using observable_type = pp::observable::hot;

    hot_observable_wrapper(O &&o) : obs{std::move(o)} {
      subscription = obs->subscribe([&](const auto &v) {
        pp::observable::hot_observable<typename O::value_type,
                                       typename O::observer_value_type>::notify(v);
      });
    }

    observer_value_type get() const override { return obs->get(); }
    void next(const value_type &v) override { obs->next(v); }
    void next(value_type &&v) override { obs->next(v); }

  private:
    O::subscription_type subscription;
    std::optional<O> obs;
  };
} // namespace detail

constexpr static auto to_observable_ptr = detail::to_observable_ptr_tag{};

template <std::copy_constructible Type, std::copy_constructible ObserverValueType>
using hot_observer_ptr = std::unique_ptr<pp::observable::hot_observable<Type, ObserverValueType>>;

template <pp::concepts::hot_observable O>
[[nodiscard]] inline auto operator|(O &o, detail::to_observable_ptr_tag) {
  return std::make_unique<detail::hot_observable_wrapper_r<O>>(o);
}

template <pp::concepts::hot_observable O>
[[nodiscard]] inline auto operator|(O &&o, detail::to_observable_ptr_tag) {
  return std::make_unique<detail::hot_observable_wrapper<O>>(std::forward<O>(o));
}
} // namespace pp
