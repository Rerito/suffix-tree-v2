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
concept MemberBegin = requires(T& t)
{
    { Details::decayCopy(t.begin()) } -> std::input_or_output_iterator;
};

template< typename T >
concept AdlBegin = ClassOrEnum< T > && requires(T& t)
{
    { Details::decayCopy(begin(t)); } -> std::input_or_output_iterator;
};

} // namespace Details

namespace CPO {

class Begin
{
private:
    template< typename T >
    static constexpr bool _isNoExcept()
    {
        if constexpr(std::is_array_v< std::remove_reference_t< T > >) {
            return true;
        } else if constexpr(Details::HasMemberBeginV< T >) {
            return noexcept(std::declval< T >().begin());
        } else {
            return noexcept(begin(std::declval<T>()));
        }
    }

public:
    template< typename T >
        requires(
            Details::MemberBegin< T > || Details::AdlBegin< T > ||
            std::is_array_v< std::remove_reference_t< T > >)
    constexpr auto begin(T&& t) const noexcept(_isNoExcept< T >())
    {
        if constexpr (std::is_array_v< std::remove_reference_t< T > >) {
            static_assert(
                std::is_lvalue_reference_v< T >,
                "Cannot use an rvalue array");

            using UnderlyingT = std::remove_all_extents_t< T >;
            static_assert(
                sizeof(UnderlyingT) > 0,
                "Cannot use an array of incomplete type");

            return t + 0;
        } else if constexpr (Details::MemberBegin< T >) {
            return t.begin();
        } else {
            return begin(t);
        }
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::Begin begin{};

} // namespace Cust

} // namespace Ranges
} // namespace Meta
} // namespace Penjing
