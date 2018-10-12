#ifndef FN_FUNCTION_HPP
#define FN_FUNCTION_HPP

#include <fn/detail/invocable.hpp>
#include <fn/detail/invoke.hpp>
#include <fn/detail/small_unique_ptr.hpp>

#include <cstddef>

#include <memory>

namespace fn {

template <typename>
class Function;

template <typename T, typename ...Ts>
class Function<T(Ts...)> {
private:
    using Invocable = detail::Invocable<T, Ts...>;

public:
    template <
        typename C,
        std::enable_if_t<
            detail::is_invocable<const std::decay_t<C>&, Ts...>::value
            && std::is_convertible<
                detail::invoke_result_t<const std::decay_t<C>&, Ts...>,
                T
            >::value,
            int
        > = 0
    >
    explicit Function(C &&c) : invocable_ptr_{
        detail::ValueTag<detail::Invoker<std::decay_t<C>, Ts...>>{ },
        std::forward<C>(c)
    } { }

    T operator()(Ts ...args) const {
        return (*invocable_ptr_)(std::forward<Ts>(args)...);
    }

private:
    detail::SmallUniquePtr<Invocable> invocable_ptr_;
};

} // namespace fn

#endif
