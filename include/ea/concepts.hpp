#pragma once

#include <cstddef>
#include <concepts>
#include <ranges>
#include <type_traits>

#include <ea/config.hpp>
#include <ea/meta.hpp>

namespace ea::concepts {

    // Basic scalar concepts ------------------------------------------------

    template <typename T>
    using remove_cvref_t = ea::meta::remove_cvref_t<T>;

    // Real floating-point scalar (float, double, long double, ...)
    template <typename T>
    concept RealScalar =
        std::is_floating_point_v<remove_cvref_t<T>>;

    // Integer scalar, excluding bool (indices, counters, etc.)
    template <typename T>
    concept IntegerScalar =
        std::is_integral_v<remove_cvref_t<T>> &&
        !std::is_same_v<remove_cvref_t<T>, bool>;

    // Generic scalar used in the library.
    // For now: real or integer. Later mayde add std::complex è etc
    template <typename T>
    concept Scalar =
        RealScalar<T> || IntegerScalar<T>;

    // Range concepts -------------------------------------------------------

    template <typename R>
    concept Range =
        std::ranges::range<R>;

    template <typename R>
    concept SizedRange =
        std::ranges::sized_range<R>;

    template <typename R>
    concept ContiguousRange =
        std::ranges::contiguous_range<R>;

    template <typename R>
    concept BorrowedRange =
        std::ranges::borrowed_range<R>;

    // Range whose value_type is a Scalar
    template <typename R>
    concept ScalarRange =
        SizedRange<R> &&
        Scalar<ea::meta::range_value_t<R>>;


    template <typename R>
    concept sized_contiguous_range =
        std::ranges::contiguous_range<R> &&
        std::ranges::sized_range<R>;

} // namespace ea::concepts


namespace ea {

    template <typename T>
    concept RealScalar = concepts::RealScalar<T>;

    template <typename T>
    concept IntegerScalar = concepts::IntegerScalar<T>;

    template <typename T>
    concept Scalar = concepts::Scalar<T>;

    template <typename R>
    concept Range = concepts::Range<R>;

    template <typename R>
    concept SizedRange = concepts::SizedRange<R>;

    template <typename R>
    concept ContiguousRange = concepts::ContiguousRange<R>;

    template <typename R>
    concept BorrowedRange = concepts::BorrowedRange<R>;

    template <typename R>
    concept ScalarRange = concepts::ScalarRange<R>;

} // namespace ea
