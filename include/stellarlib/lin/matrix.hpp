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

#ifndef STELLARLIB_LIN_MATRIX_HPP
#define STELLARLIB_LIN_MATRIX_HPP

#include <array>
#include <cstddef>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin::internal
{
template <typename T, std::size_t M, std::size_t N>
struct matrix final : public std::array<T, M * N> {};

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto operator+(const matrix<T, M, N> &self) noexcept
{
	return self;
}

#define STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &self) noexcept\
	-> matrix<T, M, N>\
{\
	matrix<T, M, N> res;\
\
	for (const auto [res, elem] : std::views::zip(res, self)) {\
		res = op elem;\
	}\
\
	return res;\
}

STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(-);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(!);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(~);

#define STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N>\
constexpr auto operator op (matrix<T, M, N> &self) noexcept\
	-> auto &\
{\
	for (auto &elem : self) {\
		op elem;\
	}\
\
	return self;\
}\
\
template <typename T, std::size_t M, std::size_t N>\
constexpr auto operator op (matrix<T, M, N> &self, int) noexcept\
	-> matrix<T, M, N>\
{\
	auto res{self};\
	op self;\
	return res;\
}

STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(++);
STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(--);

#define STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N, typename U>\
constexpr auto operator op##= (matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> auto &\
{\
	for (auto &lhs : lhs) {\
		lhs op##= rhs;\
	}\
\
	return lhs;\
}\
\
template <typename T, std::size_t M, std::size_t N, typename U>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	auto res{lhs};\
	res op##= rhs;\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const T lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	matrix<std::common_type_t<T, U>, M, N> res;\
\
	for (const auto [res, rhs] : std::views::zip(res, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
constexpr auto operator op##= (matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> auto &\
{\
	for (const auto [lhs, rhs] : std::views::zip(lhs, rhs)) {\
		lhs op##= rhs;\
	}\
\
	return lhs;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	auto res{lhs};\
	res op##= rhs;\
	return res;\
}

STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(+);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(-);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(*);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(/);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(%);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(<<);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(>>);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(&);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(|);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(^);

#define STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N, typename U>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, lhs] : std::views::zip(res, lhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const T lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, rhs] : std::views::zip(res, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, lhs, rhs] : std::views::zip(res, lhs, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}

STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(&&);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(||);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(<);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(>);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(==);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(!=);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(<=);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(>=);
}

#endif
