#pragma once

#include <cstddef>
#include <initializer_list>
#include <utility>

#include <ea/config.hpp>
#include <ea/concepts.hpp>
#include <ea/meta.hpp>
#include <ea/ranges.hpp>
#include <ea/policies.hpp>

#include <ea/detail/sum/dispatch.hpp>

namespace ea {

    template <ScalarRange R,
        typename Policy = sum_policy::auto_select>
    [[nodiscard]]
    constexpr auto sum(R&& r, Policy policy = {})
        -> scalar_type_t<R>
    {
        return detail::sum::dispatch(std::forward<R>(r), policy);
    }

    template <Scalar T,
        typename Policy = sum_policy::auto_select>
    [[nodiscard]]
    constexpr auto sum(const T* data, std::size_t n, Policy policy = {})
        -> T
    {
        auto span = ea::make_span(data, n);
        return ea::sum(span, policy);
    }

    template <Scalar T,
        typename Policy = sum_policy::auto_select>
    [[nodiscard]]
    constexpr auto sum(std::initializer_list<T> il, Policy policy = {})
        -> T
    {
        return ea::sum(ea::make_span(il.begin(), il.size()), policy);
    }

} // namespace ea
