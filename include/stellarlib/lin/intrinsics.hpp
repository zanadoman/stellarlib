/* clang-format off */

/*
  stellarlib
  Copyright (C) 2025-2026 Domán Zana

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef STELLARLIB_LIN_INTRINSICS_HPP
#define STELLARLIB_LIN_INTRINSICS_HPP

#include <stellarlib/lin/matrix.hpp>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin::internal
{
template <typename T>
[[nodiscard]]
constexpr auto abs(const T x) noexcept
{
	return x < 0 ? -x : x;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto abs(const matrix<T, M, N> &x) noexcept
	-> matrix<T, M, N>
{
	matrix<T, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = abs(x);
	}

	return ret;
}

#define STELLARLIB_LIN_INTRINSICS_UNARY_OPERATION_IMPL(op)\
template <typename T>\
[[nodiscard]]\
constexpr auto op(const T x) noexcept\
	requires (std::is_same_v<T, float> || std::is_same_v<T, double>)\
{\
	if constexpr (std::is_same_v<T, float>) {\
		return std::op##f(x);\
	}\
	else if constexpr (std::is_same_v<T, double>) {\
		return std::op(x);\
	}\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const matrix<T, M, N> &x) noexcept\
	-> matrix<T, M, N>\
{\
	matrix<T, M, N> ret;\
\
	for (const auto [ret, x] : std::views::zip(ret, x)) {\
		ret = op(x);\
	}\
\
	return ret;\
}

STELLARLIB_LIN_INTRINSICS_UNARY_OPERATION_IMPL(acos);

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const matrix<T, M, N> &x) noexcept
{
	return !std::ranges::contains(x, false);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto any(const matrix<T, M, N> &x) noexcept
{
	return std::ranges::contains(x, true);
}

[[nodiscard]]
constexpr auto asdouble(const std::uint32_t lowbits, const std::uint32_t highbits)
{
	return std::bit_cast<double>(static_cast<std::uint64_t>(highbits) << std::uint64_t{32} | static_cast<std::uint64_t>(lowbits));
}
}

#endif
