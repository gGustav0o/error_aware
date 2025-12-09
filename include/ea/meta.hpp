#pragma once

#include <cstddef>
#include <ranges>
#include <type_traits>
#include <utility>

#include <ea/config.hpp>

namespace ea::meta {

    template <typename T>
    using remove_cvref_t =
        std::remove_cv_t<std::remove_reference_t<T>>;

    template <typename T, template <typename...> class Template>
    struct is_specialization_of : std::false_type {};

    template <template <typename...> class Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template> : std::true_type {};

    template <typename T, template <typename...> class Template>
    inline constexpr bool is_specialization_of_v =
        is_specialization_of<T, Template>::value;

    template <typename R>
    using range_value_t =
        std::ranges::range_value_t<std::remove_reference_t<R>>;

    template <typename R>
    using range_reference_t =
        std::ranges::range_reference_t<std::remove_reference_t<R>>;

    template <typename R>
    using range_difference_t =
        std::ranges::range_difference_t<std::remove_reference_t<R>>;

    template <std::ranges::sized_range R>
    using range_size_t =
        decltype(std::ranges::size(std::declval<R&>()));

    template <typename T, bool IsRange = std::ranges::range<std::remove_reference_t<T>>>
    struct scalar_type;

    template <typename T>
    struct scalar_type<T, false> {
        using type = remove_cvref_t<T>;
    };

    template <typename R>
    struct scalar_type<R, true> {
        using type = range_value_t<R>;
    };

    template <typename T>
    using scalar_type_t = typename scalar_type<T>::type;

} // namespace ea::meta


namespace ea {

    template <typename T>
    using remove_cvref_t = meta::remove_cvref_t<T>;

    template <typename T, template <typename...> class Template>
    inline constexpr bool is_specialization_of_v =
        meta::is_specialization_of_v<T, Template>;

    template <typename R>
    using range_value_t = meta::range_value_t<R>;

    template <typename R>
    using range_reference_t = meta::range_reference_t<R>;

    template <typename R>
    using range_difference_t = meta::range_difference_t<R>;

    template <std::ranges::sized_range R>
    using range_size_t = meta::range_size_t<R>;

    template <typename T>
    using scalar_type_t = meta::scalar_type_t<T>;

} // namespace ea
