#pragma once

#include <pp/concepts/observable.hpp>

namespace pp {
namespace detail {
  template <class F, class O>
  concept subscribable_pipeline_function =
      pp::concepts::observable<O> && std::is_invocable_v<F, typename O::observer_value_type> &&
      std::is_same_v<std::invoke_result_t<F, typename O::observer_value_type>, void>;

  template <pp::concepts::observable O, subscribable_pipeline_function<O> F>
  struct subscribable_pipeline_traits {
    using value_type = typename O::value_type;
    using observer_value_type = typename O::observer_value_type;
  };

  template <pp::concepts::observable O, subscribable_pipeline_function<O> F>
  class subscribe_r {
  public:
    constexpr subscribe_r(O &o, F &&f) : obs{o}, func{std::forward<F>(f)} {
      subscription = obs->get().subscribe([&](const auto &v) { func(v); });
    }

  private:
    O::subscription_type subscription;
    std::optional<std::reference_wrapper<O>> obs;
    const F func;
  };

  template <pp::concepts::observable O, subscribable_pipeline_function<O> F>
  class subscribe {
  public:
    constexpr subscribe(O &&o, F &&f) : obs{std::forward<O>(o)}, func{std::forward<F>(f)} {
      subscription = obs->subscribe([&](const auto &v) { func(v); });
    }

  private:
    O::subscription_type subscription;
    std::optional<O> obs;
    const F func;
  };

} // namespace detail

template <pp::concepts::observable O, detail::subscribable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &o, F &&f) {
  return detail::subscribe_r<O, F>{o, std::forward<F>(f)};
}

template <pp::concepts::observable O, detail::subscribable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &&o, F &&f) {
  return detail::subscribe<O, F>{std::forward<O>(o), std::forward<F>(f)};
}
} // namespace pp
