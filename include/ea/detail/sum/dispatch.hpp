#pragma once

#include <type_traits>

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>
#include <ea/policies.hpp>
#include <ea/ranges.hpp>

#include <ea/detail/sum/naive_impl.hpp>
#include <ea/detail/sum/kahan_impl.hpp>
#include <ea/detail/sum/neumaier_impl.hpp>
#include <ea/detail/sum/pairwise_impl.hpp>

namespace ea::detail::sum {

    template <typename T>
    inline constexpr bool dependent_false_v = false;

    template <ea::ScalarRange R>
    constexpr auto auto_select(R&& r)
        -> ea::scalar_type_t<R>
    {
        using T = ea::scalar_type_t<R>;
        const auto n = std::ranges::size(r);

        if constexpr (ea::RealScalar<T>) {
            if (n < 32) {
                return kahan(std::forward<R>(r));
            }
            else if (n < 1024) {
                return neumaier(std::forward<R>(r));
            }
            else {
                return pairwise_with_compensation(std::forward<R>(r));
            }
        }
        else {
            return naive(std::forward<R>(r));
        }
    }

    template <ea::ScalarRange R, typename Policy>
    constexpr auto dispatch(R&& r, Policy)
        -> ea::scalar_type_t<R>
    {
        using P = std::remove_cv_t<std::remove_reference_t<Policy>>;

        if constexpr (std::is_same_v<P, ea::sum_policy::naive>) {
            return naive(std::forward<R>(r));
        }
        else if constexpr (std::is_same_v<P, ea::sum_policy::kahan>) {
            return kahan(std::forward<R>(r));
        }
        else if constexpr (std::is_same_v<P, ea::sum_policy::neumaier>) {
            return neumaier(std::forward<R>(r));
        }
        else if constexpr (std::is_same_v<P, ea::sum_policy::pairwise>) {
            return pairwise(std::forward<R>(r));
        }
        else if constexpr (std::is_same_v<P, ea::sum_policy::pairwise_with_compensation>) {
            return pairwise_with_compensation(std::forward<R>(r));
        }
        else if constexpr (std::is_same_v<P, ea::sum_policy::auto_select>) {
            return auto_select(std::forward<R>(r));
        }
        else {
            static_assert(dependent_false_v<P>,
                "Unknown sum policy for ea::sum");
        }
    }

} // namespace ea::detail::sum
