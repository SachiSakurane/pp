#pragma once

#include <algorithm>

#include <pp/concepts/observable.hpp>
#include <pp/observable/hot_observable.hpp>

namespace pp {
namespace detail {
  // return voidを許さない(return voidはsubscribeする)
  template <class F, class O>
  concept connectable_pipeline_function =
      pp::concepts::observable<O> && std::is_invocable_v<F, typename O::observer_value_type> &&
      !std::is_same_v<std::invoke_result_t<F, typename O::observer_value_type>, void>;

  template <pp::concepts::hot_observable O, connectable_pipeline_function<O> F>
  struct connectable_pipeline_traits {
    using value_type = typename O::value_type;
    using invoke_result_type = std::invoke_result_t<F, typename O::observer_value_type>;
  };

  template <pp::concepts::hot_observable O, connectable_pipeline_function<O> F>
  class hot_pipeline_r {
  public:
    using value_type = connectable_pipeline_traits<O, F>::value_type;
    using observer_value_type = typename connectable_pipeline_traits<O, F>::invoke_result_type;
    using observer_type = pp::observer<observer_value_type>;
    using subscription_type = std::shared_ptr<observer_type>;
    using observable_type = pp::observable::hot;

    using _impl_observable = pp::observable::hot_observable<value_type, observer_value_type>;

    class hot_pipeline_r_impl : public _impl_observable {
    public:
      hot_pipeline_r_impl(O &o, F &&f) : obs{o}, func{std::forward<F>(f)} {
        subscription = obs->get().subscribe([&](auto v) { _impl_observable::notify(func(v)); });
      }

      observer_value_type get() const override { return func(obs->get().get()); }
      void next(const value_type &v) override { obs->get().next(v); }
      void next(value_type &&v) override { obs->get().next(std::move(v)); }

    private:
      O::subscription_type subscription;
      std::optional<std::reference_wrapper<O>> obs;
      const F func;
    };

    constexpr hot_pipeline_r(O &o, F &&f)
        : impl{std::make_shared<hot_pipeline_r_impl>(o, std::forward<F>(f))} {}

    observer_value_type get() const { return impl->get(); }
    void next(const value_type &v) { impl->next(v); }
    void next(value_type &&v) { impl->next(std::move(v)); }
    subscription_type subscribe(observer_type &&o) { return impl->subscribe(std::move(o)); }
    void notify(const observer_value_type &v) { impl->notify(v); }

  private:
    std::shared_ptr<hot_pipeline_r_impl> impl;
  };

  template <pp::concepts::hot_observable O, connectable_pipeline_function<O> F>
  class hot_pipeline {
  public:
    using value_type = connectable_pipeline_traits<O, F>::value_type;
    using observer_value_type = typename connectable_pipeline_traits<O, F>::invoke_result_type;
    using observer_type = pp::observer<observer_value_type>;
    using subscription_type = std::shared_ptr<observer_type>;
    using observable_type = pp::observable::hot;

    using _impl_observable = pp::observable::hot_observable<value_type, observer_value_type>;

    class hot_pipeline_impl : public _impl_observable {
    public:
      hot_pipeline_impl(O &&o, F &&f) : obs{std::forward<O>(o)}, func{std::forward<F>(f)} {
        subscription = obs->subscribe([&](auto v) { _impl_observable::notify(func(v)); });
      }

      observer_value_type get() const { return func(obs->get()); }
      void next(const value_type &v) { obs->next(v); }
      void next(value_type &&v) { obs->next(std::move(v)); }

    private:
      O::subscription_type subscription;
      std::optional<O> obs;
      const F func;
      std::vector<std::weak_ptr<observer_type>> observers;
    };

    constexpr hot_pipeline(O &&o, F &&f)
        : impl{std::make_shared<hot_pipeline_impl>(std::forward<O>(o), std::forward<F>(f))} {}

    observer_value_type get() const { return impl->get(); }
    void next(const value_type &v) { impl->next(v); }
    void next(value_type &&v) { impl->next(std::move(v)); }
    subscription_type subscribe(observer_type &&o) { return impl->subscribe(std::move(o)); }
    void notify(const observer_value_type &v) { impl->notify(v); }

  private:
    std::shared_ptr<hot_pipeline_impl> impl;
  };
} // namespace detail

template <pp::concepts::hot_observable O, detail::connectable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &o, F &&f) {
  return detail::hot_pipeline_r<O, F>{o, std::forward<F>(f)};
}

template <pp::concepts::hot_observable O, detail::connectable_pipeline_function<O> F>
[[nodiscard]] inline auto operator|(O &&o, F &&f) {
  return detail::hot_pipeline<O, F>{std::forward<O>(o), std::forward<F>(f)};
}
} // namespace pp
