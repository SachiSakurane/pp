#pragma once

#include <pp/concepts/observable.hpp>

namespace pp {
namespace detail {
  template <class F, class O>
  concept subscribable_pipeline_function =
      pp::concepts::observable<O> && std::is_invocable_v<F, typename O::observer_value_type> &&
      std::is_same_v<std::invoke_result_t<F, typename O::observer_value_type>, void>;

  template <pp::concepts::hot_observable O, subscribable_pipeline_function<O> F>
  struct subscribable_pipeline_traits {
    using value_type = typename O::value_type;
    using observer_value_type = typename O::observer_value_type;
  };

  template <pp::concepts::hot_observable O, subscribable_pipeline_function<O> F>
  class hot_subscribe_r {
  public:
    using observer_value_type = subscribable_pipeline_traits<O, F>::observer_value_type;

    class hot_subscribe_r_impl {
    public:
      hot_subscribe_r_impl(O &o, F &&f) : obs{o}, func{std::forward<F>(f)} {}

      void f(const observer_value_type &v) { func(v); }
      auto &get_observable() { return obs->get(); }

    private:
      std::optional<std::reference_wrapper<O>> obs;
      const F func;
    };

    constexpr hot_subscribe_r(O &o, F &&f)
        : impl{std::make_unique<hot_subscribe_r_impl>(o, std::forward<F>(f))},
          subscription{impl->get_observable().subscribe([&](auto v) { impl->f(v); })} {}

  private:
    std::unique_ptr<hot_subscribe_r_impl> impl;
    O::subscription_type subscription;
  };

  template <pp::concepts::hot_observable O, subscribable_pipeline_function<O> F>
  class hot_subscribe {
  public:
    using observer_value_type = subscribable_pipeline_traits<O, F>::observer_value_type;

    class hot_subscribe_impl {
    public:
      hot_subscribe_impl(O &&o, F &&f) : obs{std::forward<O>(o)}, func{std::forward<F>(f)} {}

      void f(const observer_value_type &v) { func(v); }
      auto &get_observable() { return *obs; }

    private:
      std::optional<O> obs;
      const F func;
    };

    constexpr hot_subscribe(O &&o, F &&f)
        : impl{std::make_unique<hot_subscribe_impl>(std::forward<O>(o), std::forward<F>(f))},
          subscription{impl->get_observable().subscribe([&](auto v) { impl->f(v); })} {}

  private:
    std::unique_ptr<hot_subscribe_impl> impl;
    O::subscription_type subscription;
  };

} // namespace detail

template <pp::concepts::hot_observable O, detail::subscribable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &o, F &&f) {
  return detail::hot_subscribe_r<O, F>{o, std::forward<F>(f)};
}

template <pp::concepts::hot_observable O, detail::subscribable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &&o, F &&f) {
  return detail::hot_subscribe<O, F>{std::forward<O>(o), std::forward<F>(f)};
}
} // namespace pp
