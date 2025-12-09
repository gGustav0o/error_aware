#pragma once

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>

namespace ea::detail::sum {

    template <ea::ScalarRange R>
    constexpr auto kahan(R&& r)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        static_assert(ea::RealScalar<T>,
            "Kahan summation requires a real (floating-point) scalar type");

        T sum = T(0);
        T c = T(0);

        for (auto&& x : r) {
            T y = static_cast<T>(x) - c;
            T t = sum + y;
            c = (t - sum) - y;
            sum = t;
        }

        return sum;
    }

} // namespace ea::detail::sum
