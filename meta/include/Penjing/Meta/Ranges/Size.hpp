// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <type_traits>

#include "Helpers.hpp"

namespace Penjing {
namespace Meta {
namespace Ranges {

namespace Details {

template< typename T >
concept MemberSize = requires(T& t)
{
    {
        Details::decayCopy(t.size())
        } -> std::integral;
};

template< typename T >
concept AdlSize = ClassOrEnum< T > && requires(T& t)
{
    {
        Details::decayCopy(size(t));
        } -> std::integral;
};

} // namespace Details

namespace CPO {

class Size
{
private:
    template< typename T >
    static constexpr bool _isNoExcept()
    {
        if constexpr (std::is_bounded_array_v< std::remove_reference_t< T > >) {
            return true;
        } else if constexpr (Details::HasMemberSizeV< T >) {
            return noexcept(std::declval< T >().size());
        } else {
            return noexcept(size(std::declval< T >()));
        }
    }

public:
    template< typename T >
        requires(
            Details::MemberSize< T > || Details::AdlSize< T > ||
            std::is_bounded_array_v< std::remove_reference_t< T > >)
    constexpr auto size(T&& t) const noexcept(_isNoExcept< T >())
    {
        if constexpr (std::is_bounded_array_v< std::remove_reference_t< T > >) {
            static_assert(
                std::is_lvalue_reference_v< T >,
                "Cannot use an rvalue array");

            return std::extent_v< std::remove_reference_t< T > >;
        } else if constexpr (Details::MemberSize< T >) {
            return t.size();
        } else {
            return size(t);
        }
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::Size size{};

} // namespace Cust

} // namespace Ranges
} // namespace Meta
} // namespace Penjing
