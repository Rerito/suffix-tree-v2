// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>
#include <utility>

namespace Penjing {
namespace Meta {
namespace Ranges {

namespace Details {

template< typename T >
constexpr std::decay_t< T > decayCopy(T&& t) noexcept(
    std::is_nothrow_convertible_v< T, std::decay_t< T > >)
{
    return std::forward< T >(t);
}

template< typename T >
concept ClassOrEnum = std::is_class_v< std::remove_reference_t< T > > ||
    std::is_enum_v< std::remove_reference_t< T > >;

}

} // namespace Ranges
} // namespace Meta
} // namespace Penjing
