#pragma once

#include <cmath>

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>

namespace ea::detail::sum {

    template <ea::ScalarRange R>
    constexpr auto neumaier(R&& r)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        static_assert(ea::RealScalar<T>,
            "Neumaier summation requires a real (floating-point) scalar type");

        T sum = T(0);
        T c = T(0);

        for (auto&& x_raw : r) {
            T x = static_cast<T>(x_raw);
            T t = sum + x;
            if (std::fabs(sum) >= std::fabs(x)) {
                c += (sum - t) + x;
            }
            else {
                c += (x - t) + sum;
            }
            sum = t;
        }

        return sum + c;
    }

} // namespace ea::detail::sum
