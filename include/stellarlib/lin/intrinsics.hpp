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
#include <cmath>
#include <cstddef>
#include <functional>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin
{
template <typename T>
[[nodiscard]]
constexpr auto abs(const T arg) noexcept
{
	return arg < 0 ? -arg : arg;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto abs(const internal::matrix<T, M, N> &matrix) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> res;

	for (const auto [res, elem] : std::views::zip(res, matrix)) {
		res = abs(elem);
	}

	return res;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const internal::matrix<T, M, N> &matrix) noexcept
{
	return !std::ranges::contains(matrix, false);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto any(const internal::matrix<T, M, N> &matrix) noexcept
{
	return std::ranges::contains(matrix, true);
}

template <typename T, typename U, typename V>
[[nodiscard]]
constexpr auto mad(T x, U y, V z) noexcept
{
	return x * y + z;
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M, N> &lhs, const internal::matrix<U, M, N> &rhs) noexcept
	requires (M == 1 || N == 1)
{
	return internal::matrix<std::common_type_t<T, U>, 1, 1>{std::ranges::fold_left(lhs * rhs, std::common_type_t<T, U>{}, std::plus{})};
}

/* template <typename T, typename U, std::size_t M, std::size_t N, std::size_t P> */
/* [[nodiscard]] */
/* constexpr auto mul(const internal::matrix<T, N, M> &lhs, const internal::matrix<U, P, N> &rhs) noexcept */
/* 	-> internal::matrix<std::common_type_t<T, U>, M, P> */
/* 	requires (M == 1 || P == 1) */
/* { */
/* 	internal::matrix<std::common_type_t<T, U>, M, P> res{}; */
/**/
/* 	for (const auto m : std::views::iota(std::size_t{}, M)) { */
/* 		for (const auto n : std::views::iota(std::size_t{}, N)) { */
/* 			for (const auto p : std::views::iota(std::size_t{}, P)) { */
/* 				res[mad(m, P, p)] += lhs[mad(m, N, n)] * rhs[mad(n, P, p)]; */
/* 			} */
/* 		} */
/* 	} */
/**/
/* 	return res; */
/* } */

template <typename T, typename U, std::size_t M, std::size_t N, std::size_t P>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M, N> &lhs, const internal::matrix<U, N, P> &rhs) noexcept
	-> internal::matrix<std::common_type_t<T, U>, M, P>
{
	internal::matrix<std::common_type_t<T, U>, M, P> res{};

	for (const auto m : std::views::iota(std::size_t{}, M)) {
		for (const auto n : std::views::iota(std::size_t{}, N)) {
			for (const auto p : std::views::iota(std::size_t{}, P)) {
				res[m, p] += lhs[m, n] * rhs[n, p];
			}
		}
	}

	return res;
}

#define STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(fn)\
template <typename T>\
[[nodiscard]]\
constexpr auto fn(const T arg) noexcept\
	requires (std::is_same_v<T, float> || std::is_same_v<T, double>)\
{\
	if constexpr (std::is_same_v<T, float>) {\
		return std::fn##f(arg);\
	}\
	else if constexpr (std::is_same_v<T, double>) {\
		return std::fn(arg);\
	}\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto fn(const internal::matrix<T, M, N> &matrix) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, elem] : std::views::zip(res, matrix)) {\
		res = fn(elem);\
	}\
\
	return res;\
}

STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(acos);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(asin);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(atan);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(ceil);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(cos);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(cosh);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(exp);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(exp2);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(floor);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(log);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(log10);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(log2);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(modf);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(sin);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(sinh);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(sqrt);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(tan);
STELLARLIB_LIN_INTRINSICS_TRIGONOMETRIC_IMPL(tanh);
}

#endif
