#pragma once

#include <functional>
#include <optional>

#include <pp/observer.hpp>

namespace pp {
namespace detail {
  template <class F, class O>
  concept connectable_observable_to_function =
      observable<O> && std::is_invocable_v<F, typename O::value_type> &&
      !std::is_same_v<std::invoke_result_t<F, typename O::value_type>, void>;

  template <observable O, connectable_observable_to_function<O> F>
  struct connection_observable_traits {
    using value_type = typename O::value_type;
    using invoke_result_type = std::invoke_result_t<F, value_type>;

    using observer_type = observer<invoke_result_type>;
    using subscription_type = subscription<value_type>;
  };

  template <observable O, connectable_observable_to_function<O> F>
  class observable_connection_r {
  public:
    using value_type = connection_observable_traits<O, F>::invoke_result_type;

    class subscription_impl : public subscription<typename O::value_type>,
                              public connection_observable_traits<O, F>::observer_type {
    public:
      constexpr subscription_impl(O &o, F &&f) : obs{o}, func{std::forward<F>(f)} {}

      value_type observe() const override { return func(obs->get().observe()); }
      void receive(typename connection_observable_traits<O, F>::value_type v) override {
        connection_observable_traits<O, F>::observer_type::notify(func(v));
      }

    private:
      std::optional<std::reference_wrapper<O>> obs;
      const F func;
    };

    constexpr observable_connection_r(O &o, F &&f)
        : sub{std::make_shared<subscription_impl>(o, std::forward<F>(f))} {
      o.subscribe(sub);
    }

    operator value_type() const { return sub->observe(); }
    value_type observe() const { return sub->observe(); }
    void subscribe(std::weak_ptr<pp::subscription<value_type>> s) { sub->subscribe(s); }
    std::size_t subscriptions_count() const { return sub->subscriptions_count(); }
    void notify(value_type v) { sub->notify(v); }
    void fetch() { sub->fetch(); }

  private:
    std::shared_ptr<subscription_impl> sub;
  };

  template <observable O, connectable_observable_to_function<O> F>
  class observable_connection {
  public:
    using value_type = connection_observable_traits<O, F>::invoke_result_type;

    class subscription_impl : public subscription<typename O::value_type>,
                              public connection_observable_traits<O, F>::observer_type {
    public:
      constexpr subscription_impl(O &&o, F &&f)
          : obs{std::forward<O>(o)}, func{std::forward<F>(f)} {}

      value_type observe() const override { return func(obs->observe()); }
      void receive(typename connection_observable_traits<O, F>::value_type v) override {
        connection_observable_traits<O, F>::observer_type::notify(func(v));
      }

      auto &get_observer() { return obs; }

    private:
      std::optional<O> obs;
      const F func;
    };

    constexpr observable_connection(O &&o, F &&f)
        : sub{std::make_shared<subscription_impl>(std::forward<O>(o), std::forward<F>(f))} {
      sub->get_observer()->subscribe(sub);
    }

    operator value_type() const { return sub->observe(); }
    value_type observe() const { return sub->observe(); }
    void subscribe(std::weak_ptr<pp::subscription<value_type>> s) { sub->subscribe(s); }
    std::size_t subscriptions_count() const { return sub->subscriptions_count(); }
    void notify(value_type v) { sub->notify(v); }
    void fetch() { sub->fetch(); }

  private:
    std::shared_ptr<subscription_impl> sub;
  };

} // namespace detail

template <observable O, detail::connectable_observable_to_function<O> F>
[[nodiscard]] inline auto operator|(O &o, F &&f) {
  return detail::observable_connection_r<O, F>{o, std::forward<F>(f)};
}

template <observable O, detail::connectable_observable_to_function<O> F>
[[nodiscard]] inline auto operator|(O &&o, F &&f) {
  return detail::observable_connection<O, F>{std::forward<O>(o), std::forward<F>(f)};
}
} // namespace pp
