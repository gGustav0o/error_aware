#pragma once

#include <cstddef>
#include <span>
#include <ranges>
#include <type_traits>
#include <iterator>
#include <memory>

#include <ea/config.hpp>
#include <ea/meta.hpp>
#include <ea/concepts.hpp>

namespace ea::ranges {

    template <typename R>
    inline constexpr bool is_contiguous_range_v =
        std::ranges::contiguous_range<R>;

    template <typename R>
    inline constexpr bool is_sized_range_v =
        std::ranges::sized_range<R>;

    template <std::ranges::contiguous_range R>
    constexpr auto data(R& r) noexcept
        -> ea::meta::range_value_t<R>*
    {
        using std::data;
        if constexpr (requires { data(r); }) {
            return data(r);
        }
        else {
            auto it = std::ranges::begin(r);
            return std::to_address(it);
        }
    }

    template <std::ranges::contiguous_range R>
    constexpr auto data(R const& r) noexcept
        -> ea::meta::range_value_t<R> const*
    {
        using std::data;
        if constexpr (requires { data(r); }) {
            return data(r);
        }
        else {
            auto it = std::ranges::begin(r);
            return std::to_address(it);
        }
    }

    template <std::ranges::sized_range R>
    constexpr auto size(R const& r) noexcept
        -> ea::meta::range_size_t<R>
    {
        return std::ranges::size(r);
    }

    template <concepts::sized_contiguous_range R>
    constexpr auto as_span(R& r) noexcept
        -> std::span<ea::meta::range_value_t<R>>
    {
        using T = ea::meta::range_value_t<R>;
        const auto n = std::ranges::size(r);
        if (n == 0) {
            return std::span<T>{};
        }
        return std::span<T>{ data(r), n };
    }

    template <concepts::sized_contiguous_range R>
    constexpr auto as_span(R const& r) noexcept
        -> std::span<ea::meta::range_value_t<R> const>
    {
        using T = ea::meta::range_value_t<R> const;
        const auto n = std::ranges::size(r);
        if (n == 0) {
            return std::span<T>{};
        }
        return std::span<T>{ data(r), n };
    }


    template <typename T, std::size_t Extent>
    constexpr auto as_span(std::span<T, Extent> s) noexcept
        -> std::span<T, Extent>
    {
        return s;
    }

    template <typename T, std::size_t N>
    constexpr auto as_span(T(&arr)[N]) noexcept
        -> std::span<T, N>
    {
        return std::span<T, N>{ arr };
    }

    template <typename T, std::size_t N>
    constexpr auto as_span(T(&arr)[N]) noexcept -> std::span<T, N> {
        return { arr };
    }

    template <typename T, std::size_t N>
    constexpr auto as_span(T const(&arr)[N]) noexcept -> std::span<T const, N> {
        return { arr };
    }

    template <typename T, std::size_t N>
    constexpr auto as_span(std::array<T, N>& arr) noexcept -> std::span<T, N> {
        return { arr.data(), N };
    }

    template <typename T, std::size_t N>
    constexpr auto as_span(std::array<T, N> const& arr) noexcept -> std::span<T const, N> {
        return { arr.data(), N };
    }


    template <typename T>
    constexpr auto make_span(T* ptr, std::size_t n) noexcept
        -> std::span<T>
    {
        EA_ASSERT(ptr != nullptr || n == 0 && "make_span: nullptr input pointer and nonzero size");
        return std::span<T>{ ptr, n };
    }

    template <typename T>
    constexpr auto make_span(T const* ptr, std::size_t n) noexcept
        -> std::span<T const>
    {
        EA_ASSERT(ptr != nullptr || n == 0 && "make_span: nullptr input pointer and nonzero size");
        return std::span<T const>{ ptr, n };
    }

    template <typename T, std::size_t N>
    constexpr auto make_span(T(&arr)[N]) noexcept
        -> std::span<T, N>
    {
        return std::span<T, N>{ arr };
    }

} // namespace ea::ranges


namespace ea {

    namespace rng = ranges;

    using ranges::is_contiguous_range_v;
    using ranges::is_sized_range_v;

    using ranges::data;
    using ranges::size;
    using ranges::as_span;
    using ranges::make_span;

} // namespace ea
