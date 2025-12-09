#pragma once

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>
#include <ea/ranges.hpp>

namespace ea::detail::sum {

    template <ea::ScalarRange R>
    constexpr auto naive(R&& r)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        T s = T(0);
        for (auto&& x : r) {
            s += static_cast<T>(x);
        }
        return s;
    }

} // namespace ea::detail::sum
