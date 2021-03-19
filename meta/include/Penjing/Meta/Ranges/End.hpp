// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <iterator>
#include <type_traits>

#include "Helpers.hpp"

namespace Penjing {
namespace Meta {
namespace Ranges {

namespace Details {

template< typename T >
concept MemberEnd = requires(T& t)
{
    {
        Details::decayCopy(t.end())
        } -> std::input_or_output_iterator;
};

template< typename T >
concept AdlEnd = ClassOrEnum< T > && requires(T& t)
{
    {
        Details::decayCopy(end(t));
        } -> std::input_or_output_iterator;
};

} // namespace Details

namespace CPO {

class End
{
private:
    template< typename T >
    static constexpr bool _isNoExcept()
    {
        if constexpr (std::is_bounded_array_v< std::remove_reference_t< T > >) {
            return true;
        } else if constexpr (Details::HasMemberEndV< T >) {
            return noexcept(std::declval< T >().end());
        } else {
            return noexcept(end(std::declval< T >()));
        }
    }

public:
    template< typename T >
        requires(
            Details::MemberEnd< T > || Details::AdlEnd< T > ||
            std::is_bounded_array_v< std::remove_reference_t< T > >)
    constexpr auto end(T&& t) const noexcept(_isNoExcept< T >())
    {
        if constexpr (std::is_bounded_array_v< std::remove_reference_t< T > >) {
            static_assert(
                std::is_lvalue_reference_v< T >,
                "Cannot use an rvalue array");

            using UnderlyingT = std::remove_all_extents_t< T >;

            return t + std::extent_v< std::remove_reference_t< T > >;
        } else if constexpr (Details::MemberEnd< T >) {
            return t.end();
        } else {
            return end(t);
        }
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::End end{};

} // namespace Cust

} // namespace Ranges
} // namespace Meta
} // namespace Penjing
