#ifndef FN_DETAIL_INVOCABLE_HPP
#define FN_DETAIL_INVOCABLE_HPP

#include <fn/detail/invoke.hpp>

#include <tuple>
#include <type_traits>

namespace fn {
namespace detail {

template <typename T, typename ...Ts>
class Invocable {
public:
    using Result = T;
    using Args = std::tuple<Ts...>;

    T operator()(Ts ...ts) const {
        return do_invoke(ts...);
    }

    virtual ~Invocable() = default;

private:
    virtual T do_invoke(Ts ...ts) const = 0;
};

template <typename C, typename ...Ts>
class Invoker : public Invocable<invoke_result_t<const C&, Ts...>, Ts...> {
public:
    static_assert(
        is_invocable<const C&, Ts...>::value,
        "const C& must be invocable!"
    );

    template <
    	typename ...Us,
    	std::enable_if_t<std::is_constructible<C, Us&&...>::value, int> = 0
    >
    constexpr explicit Invoker(Us &&...us)
    noexcept(std::is_nothrow_constructible<C, Us&&...>::value)
    : callable_(std::forward<Us>(us)...) { }

    virtual ~Invoker() = default;

private:
    invoke_result_t<C&, Ts...> do_invoke(Ts ...ts) const override {
        return invoke(callable_, ts...);
    }

    C callable_;
};

} // namespace detail
} // namespace fn

#endif
