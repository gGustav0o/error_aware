#pragma once

#include <span>
#include <cmath>

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>
#include <ea/ranges.hpp>
#include <ea/detail/sum/kahan_impl.hpp>

namespace ea::detail::sum {

    template <typename T>
    constexpr T pairwise_span(std::span<const T> s)
    {
        const std::size_t n = s.size();
        if (n == 0) {
            return T(0);
        }
        if (n == 1) {
            return s[0];
        }
        const std::size_t mid = n / 2;
        auto left = s.first(mid);
        auto right = s.last(n - mid);
        return pairwise_span<T>(left) + pairwise_span<T>(right);
    }

    template <typename T>
    constexpr T pairwise_with_compensation_span(std::span<const T> s,
        std::size_t block_limit = 64)
    {
        const std::size_t n = s.size();
        if (n == 0) {
            return T(0);
        }
        if (n <= block_limit) {
            return kahan(s);
        }

        const std::size_t mid = n / 2;
        auto left = s.first(mid);
        auto right = s.last(n - mid);
        return pairwise_with_compensation_span<T>(left, block_limit)
            + pairwise_with_compensation_span<T>(right, block_limit);
    }

    template <ea::ScalarRange R>
    constexpr auto pairwise(R&& r)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        auto s = ea::as_span(r);
        return pairwise_span<T>(std::span<const T>{s.data(), s.size()});
    }

    template <ea::ScalarRange R>
    constexpr auto pairwise_with_compensation(R&& r,
        std::size_t block_limit = 64)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        static_assert(ea::RealScalar<T>,
            "pairwise_with_compensation requires a real (floating-point) scalar type");

        auto s = ea::as_span(r);
        return pairwise_with_compensation_span<T>(
            std::span<const T>{s.data(), s.size()},
            block_limit
        );
    }

} // namespace ea::detail::sum
