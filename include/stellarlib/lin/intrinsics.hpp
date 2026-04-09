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

#include <SDL3/SDL_stdinc.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <numbers>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin
{
#define STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(op, arg1, expr)\
template <typename T>\
[[nodiscard]]\
constexpr auto op(const T arg1) noexcept\
	requires (std::is_arithmetic_v<T>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &(arg1)) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, arg1] : std::views::zip(res, arg1)) {\
		res = op(arg1);\
	}\
\
	return res;\
}

#define STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(op, arg1, arg2, expr)\
template <typename T, typename U>\
[[nodiscard]]\
constexpr auto op(const T arg1, const U arg2) noexcept\
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N, typename U>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &(arg1), const U arg2) noexcept\
	-> internal::matrix<std::common_type_t<T, U>, M, N>\
	requires (std::is_arithmetic_v<U>)\
{\
	internal::matrix<std::common_type_t<T, U>, M, N> res;\
\
	for (const auto [res, arg1] : std::views::zip(res, arg1)) {\
		res = op(arg1, arg2);\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const T arg1, const internal::matrix<U, M, N> &(arg2)) noexcept\
	-> internal::matrix<std::common_type_t<T, U>, M, N>\
	requires (std::is_arithmetic_v<T>)\
{\
	internal::matrix<std::common_type_t<T, U>, M, N> res;\
\
	for (const auto [res, arg2] : std::views::zip(res, arg2)) {\
		res = op(arg1, arg2);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &(arg1), const internal::matrix<U, M2, N2> &(arg2)) noexcept\
	-> internal::matrix<std::common_type_t<T, U>, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)\
{\
	internal::matrix<std::common_type_t<T, U>, M1, N1> res;\
\
	for (const auto [res, arg1, arg2] : std::views::zip(res, arg1, arg2)) {\
		res = op(arg1, arg2);\
	}\
\
	return res;\
}

#define STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(op, arg1, arg2, arg3, expr)\
template <typename T, typename U, typename V>\
[[nodiscard]]\
constexpr auto op(const T arg1, const U arg2, const V arg3) noexcept\
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N, typename U, typename V>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &(arg1), const U arg2, const V arg3) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M, N>\
	requires (std::is_arithmetic_v<U> && std::is_arithmetic_v<V>)\
{\
	internal::matrix<std::common_type_t<T, U, V>, M, N> res;\
\
	for (const auto [res, arg1] : std::views::zip(res, arg1)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N, typename V>\
[[nodiscard]]\
constexpr auto op(const T arg1, const internal::matrix<U, M, N> &(arg2), const V arg3) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M, N>\
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<V>)\
{\
	internal::matrix<std::common_type_t<T, U, V>, M, N> res;\
\
	for (const auto [res, arg2] : std::views::zip(res, arg2)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, typename U, typename V, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const T arg1, const U arg2, const internal::matrix<V, M, N> &(arg3)) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M, N>\
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)\
{\
	internal::matrix<std::common_type_t<T, U, V>, M, N> res;\
\
	for (const auto [res, arg3] : std::views::zip(res, arg3)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2, typename V>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &(arg1), const internal::matrix<U, M2, N2> &(arg2), const V arg3) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M1, N1>\
	requires (std::is_arithmetic_v<V> && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2))\
{\
	internal::matrix<std::common_type_t<T, U, V>, M1, N1> res;\
\
	for (const auto [res, arg1, arg2] : std::views::zip(res, arg1, arg2)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, typename U, typename V, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &(arg1), const U arg2, const internal::matrix<V, M2, N2> &(arg3)) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M1, N1>\
	requires (std::is_arithmetic_v<U> && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2))\
{\
	internal::matrix<std::common_type_t<T, U, V>, M1, N1> res;\
\
	for (const auto [res, arg1, arg3] : std::views::zip(res, arg1, arg3)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M1, std::size_t N1, typename V, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const T arg1, const internal::matrix<U, M1, N1> &(arg2), const internal::matrix<V, M2, N2> &(arg3)) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M1, N1>\
	requires (std::is_arithmetic_v<T> && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2))\
{\
	internal::matrix<std::common_type_t<T, U, V>, M1, N1> res;\
\
	for (const auto [res, arg2, arg3] : std::views::zip(res, arg2, arg3)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2, typename V, std::size_t M3, std::size_t N3>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &(arg1), const internal::matrix<U, M2, N2> &(arg2), const internal::matrix<V, M3, N3> &(arg3)) noexcept\
	-> internal::matrix<std::common_type_t<T, U, V>, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) && M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 || M1 == M2 && M2 == M3 && N1 == N2 && N2 == N3)\
{\
	internal::matrix<std::common_type_t<T, U, V>, M1, N1> res;\
\
	for (const auto [res, arg1, arg2, arg3] : std::views::zip(res, arg1, arg2, arg3)) {\
		res = op(arg1, arg2, arg3);\
	}\
\
	return res;\
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(abs, x, {
	if constexpr (std::is_constant_evaluated()) {
		return x < 0 ? -x : x;
	}
	else {
		return std::abs(x);
	}
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(acos, x, {
	return std::acos(x);
});

template <typename T>
[[nodiscard]]
constexpr auto all(const T x) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return static_cast<bool>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const internal::matrix<T, M, N> &x) noexcept
{
	return std::ranges::all_of(x, static_cast<bool (*)(T)>(all));
}

template <typename T>
[[nodiscard]]
constexpr auto any(const T x) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return static_cast<bool>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto any(const internal::matrix<T, M, N> &x) noexcept
{
	return std::ranges::any_of(x, static_cast<bool (*)(T)>(any));
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(asin, x, {
	return std::asin(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(atan, x, {
	return std::atan(x);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(atan2, y, x, {
	return std::atan2(y, x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(ceil, x, {
	return std::ceil(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(clamp, x, min, max, {
	return SDL_clamp(x, min, max);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(cos, x, {
	return std::cos(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(cosh, x, {
	return std::cosh(x);
});

template <typename T, typename U>
[[nodiscard]]
constexpr auto cross([[maybe_unused]] const T x, [[maybe_unused]] const U y) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
{
	return internal::matrix<std::common_type_t<T, U>, 1, 3>{};
}

template <typename T, std::size_t M, std::size_t N, typename U>
[[nodiscard]]
constexpr auto cross(const internal::matrix<T, M, N> &x, const U y) noexcept
	requires (std::is_arithmetic_v<U> && M * N == 3)
{
	return internal::matrix<std::common_type_t<T, U>, M, N>{x.y() * y - x.z() * y, x.z() * y - x.x() * y, x.x() * y - x.y() * y};
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto cross(const T x, const internal::matrix<U, M, N> &y) noexcept
	requires (std::is_arithmetic_v<T> && M * N == 3)
{
	return internal::matrix<std::common_type_t<T, U>, M, N>{x * y.z() - x * y.y(), x * y.x() - x * y.z(), x * y.y() - x * y.x()};
}

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto cross(const internal::matrix<T, M1, N1> &x, const internal::matrix<U, M2, N2> &y) noexcept
	requires (M1 * N1 == 3 && M2 * N2 == 3)
{
	return internal::matrix<std::common_type_t<T, U>, M1, N1>{x.y() * y.z() - x.z() * y.y(), x.z() * y.x() - x.x() * y.z(), x.x() * y.y() - x.y() * y.x()};
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(degrees, x, {
	return 180 / std::numbers::pi_v<T> * x;
});

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 1, 1> &m) noexcept
{
	return m.front();
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 2, 2> &m) noexcept
{
	return m[0, 0] * m[1, 1] - m[0, 1] * m[1, 0];
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 3, 3> &m) noexcept
{
	return m[0, 0] * (m[1, 1] * m[2, 2] - m[1, 2] * m[2, 1]) - m[0, 1] * (m[1, 0] * m[2, 2] - m[1, 2] * m[2, 0]) + m[0, 2] * (m[1, 0] * m[2, 1] - m[1, 1] * m[2, 0]);
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 4, 4> &m) noexcept
{
	return m[0, 0] * (m[1, 1] * (m[2, 2] * m[3, 3] - m[2, 3] * m[3, 2]) - m[1, 2] * (m[2, 1] * m[3, 3] - m[2, 3] * m[3, 1]) + m[1, 3] * (m[2, 1] * m[3, 2] - m[2, 2] * m[3, 1])) - m[0, 1] * (m[1, 0] * (m[2, 2] * m[3, 3] - m[2, 3] * m[3, 2]) - m[1, 2] * (m[2, 0] * m[3, 3] - m[2, 3] * m[3, 0]) + m[1, 3] * (m[2, 0] * m[3, 2] - m[2, 2] * m[3, 0])) + m[0, 2] * (m[1, 0] * (m[2, 1] * m[3, 3] - m[2, 3] * m[3, 1]) - m[1, 1] * (m[2, 0] * m[3, 3] - m[2, 3] * m[3, 0]) + m[1, 3] * (m[2, 0] * m[3, 1] - m[2, 1] * m[3, 0])) - m[0, 3] * (m[1, 0] * (m[2, 1] * m[3, 2] - m[2, 2] * m[3, 1]) - m[1, 1] * (m[2, 0] * m[3, 2] - m[2, 2] * m[3, 0]) + m[1, 2] * (m[2, 0] * m[3, 1] - m[2, 1] * m[3, 0]));
}

template <typename T, std::size_t N>
[[nodiscard]]
constexpr auto determinant(internal::matrix<T, N, N> m) noexcept
{
	T determinant{1};

	for (const auto i : std::views::iota(std::size_t{}, N)) {
		auto pivot{i};

		for (const auto j : std::views::iota(i + 1, N)) {
			if (abs(m[pivot, i]) < abs(m[j, i])) {
				pivot = j;
			}
		}

		if (pivot != i) {
			for (const auto j : std::views::iota(i, N)) {
				std::swap(m[i, j], m[pivot, j]);
			}

			determinant *= -1;
		}

		if (!static_cast<bool>(m[i, i])) {
			return T{};
		}

		determinant *= m[i, i];

		for (const auto j : std::views::iota(i + 1, N)) {
			m[j, i] /= m[i, i];

			for (const auto k : std::views::iota(i + 1, N)) {
				m[j, k] -= m[j, i] * m[i, k];
			}
		}
	}

	return determinant;
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(sqrt, x, {
	return std::sqrt(x);
});

template <typename T>
[[nodiscard]]
constexpr auto length(const T x) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return abs(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto length(const internal::matrix<T, M, N> &x) noexcept
	requires (M == 1 || N == 1)
{
	if constexpr (M * N == 2) {
		return std::hypot(x.x(), x.y());
	}
	else if constexpr (M * N == 3) {
		return std::hypot(x.x(), x.y(), x.z());
	}
	else {
		return std::ranges::fold_left(x, 0, static_cast<T (*)(T, T)>(std::hypot));
	}
}

template <typename T, typename U>
[[nodiscard]]
constexpr auto distance(const T x, const U y) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
{
	return length(x - y);
}

template <typename T, std::size_t M, std::size_t N, typename U>
[[nodiscard]]
constexpr auto distance(const internal::matrix<T, M, N> &x, const U y) noexcept
	requires (std::is_arithmetic_v<U> && (M == 1 || N == 1))
{
	return length(x - y);
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto distance(const T x, const internal::matrix<U, M, N> &y) noexcept
	requires (std::is_arithmetic_v<T> && (M == 1 || N == 1))
{
	return length(x - y);
}

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto distance(const internal::matrix<T, M1, N1> &x, const internal::matrix<U, M2, N2> &y) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return length(x - y);
}

template <typename T, typename U>
[[nodiscard]]
constexpr auto dot(const T x, const U y) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
{
	return x * y;
}

template <typename T, std::size_t M, std::size_t N, typename U>
[[nodiscard]]
constexpr auto dot(const internal::matrix<T, M, N> &x, const U y) noexcept
	requires (std::is_arithmetic_v<U> && (M == 1 || N == 1))
{
	return std::ranges::fold_left(x | std::views::transform([y] [[nodiscard]] (const auto x) noexcept -> auto {
		return x * y;
	}), 0, std::plus{});
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto dot(const T x, const internal::matrix<U, M, N> &y) noexcept
	requires (std::is_arithmetic_v<T> && (M == 1 || N == 1))
{
	return std::ranges::fold_left(y | std::views::transform([x] [[nodiscard]] (const auto y) noexcept -> auto {
		return x * y;
	}), 0, std::plus{});
}

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto dot(const internal::matrix<T, M1, N1> &x, const internal::matrix<U, M2, N2> &y) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return std::ranges::fold_left(std::views::zip(x, y) | std::views::transform([] [[nodiscard]] (const auto xy) noexcept -> auto {
		return std::get<0>(xy) * std::get<1>(xy);
	}), 0, std::plus{});
}

template <typename T, typename U>
[[nodiscard]]
constexpr auto dst(const T src0, const U src1) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
{
	return internal::matrix<std::common_type_t<T, U>, 1, 4>{1, src0 * src1, src0, src1};
}

template <typename T, std::size_t M, std::size_t N, typename U>
[[nodiscard]]
constexpr auto dst(const internal::matrix<T, M, N> &src0, const U src1) noexcept
	requires (std::is_arithmetic_v<U> && (M == 1 || N == 1) && M * N == 4)
{
	return internal::matrix<std::common_type_t<T, U>, M, N>{1, src0.y() * src1, src0.z(), src1};
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto dst(const T src0, const internal::matrix<U, M, N> &src1) noexcept
	requires (std::is_arithmetic_v<T> && (M == 1 || N == 1) && M * N == 4)
{
	return internal::matrix<std::common_type_t<T, U>, M, N>{1, src0 * src1.y(), src0, src1.w()};
}

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto dst(const internal::matrix<T, M1, N1> &src0, const internal::matrix<U, M2, N2> &src1) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == 4 && M2 * N2 == 4)
{
	return internal::matrix<std::common_type_t<T, U>, M1, N1>{1, src0.y() * src1.y(), src0.z(), src1.w()};
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(exp, x, {
	return std::exp(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(exp2, x, {
	return std::exp2(x);
});

template <typename T>
[[nodiscard]]
constexpr auto faceforward(const T n, const T i, const T ng) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return dot(i, ng) < 0 ? n : -n;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto faceforward(const internal::matrix<T, M, N> &n, const T i, const T ng) noexcept
	requires (M == 1 || N == 1)
{
	return dot(i, ng) < 0 ? n : -n;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto faceforward(const T n, const internal::matrix<T, M, N> &i, const T ng) noexcept
	-> internal::matrix<T, M, N>
	requires (M == 1 || N == 1)
{
	internal::matrix<T, M, N> res;

	if (dot(i, ng) < 0) {
		for (auto &res : res) {
			res = n;
		}
	}
	else {
		for (auto &res : res) {
			res = -n;
		}
	}

	return res;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto faceforward(const T n, const T i, const internal::matrix<T, M, N> &ng) noexcept
	-> internal::matrix<T, M, N>
	requires (M == 1 || N == 1)
{
	internal::matrix<T, M, N> res;

	if (dot(i, ng) < 0) {
		for (auto &res : res) {
			res = n;
		}
	}
	else {
		for (auto &res : res) {
			res = -n;
		}
	}

	return res;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto faceforward(const internal::matrix<T, M1, N1> &n, const internal::matrix<T, M2, N2> &i, const T ng) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return dot(i, ng) < 0 ? n : -n;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto faceforward(const internal::matrix<T, M1, N1> &n, const T i, const internal::matrix<T, M2, N2> &ng) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return dot(i, ng) < 0 ? n : -n;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto faceforward(const T n, const internal::matrix<T, M1, N1> &i, const internal::matrix<T, M2, N2> &ng) noexcept
	-> internal::matrix<T, M1, N1>
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	internal::matrix<T, M1, N1> res;

	if (dot(i, ng) < 0) {
		for (auto &res : res) {
			res = n;
		}
	}
	else {
		for (auto &res : res) {
			res = -n;
		}
	}

	return res;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
[[nodiscard]]
constexpr auto faceforward(const internal::matrix<T, M1, N1> &n, const internal::matrix<T, M2, N2> &i, const internal::matrix<T, M3, N3> &ng) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) && M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3)
{
	return dot(i, ng) < 0 ? n : -n;
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(floor, x, {
	return std::floor(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(fma, a, b, c, {
	return std::fma(a, b, c);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(fmod, x, y, {
	return std::fmod(x, y);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(frac, x, {
	return x - floor(x);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(ldexp, x, exp, {
	return std::ldexp(x, static_cast<std::int32_t>(exp));
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(lerp, x, y, s, {
	return std::lerp(x, y, s);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(max, x, y, {
	return SDL_max(x, y);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(pow, x, y, {
	return std::pow(x, y);
});

template <typename T, typename U, typename V>
[[nodiscard]]
constexpr auto lit(const T n_dot_l, const U n_dot_h, const V m) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>)
{
	return internal::matrix<std::common_type_t<T, U, V>, 1, 4>{1, max(n_dot_l, std::common_type_t<T, U, V>{}), n_dot_l < 0 || n_dot_h < 0 ? 0 : pow(n_dot_h, m), 1};
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(log, x, {
	return std::log(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(log10, x, {
	return std::log10(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(log2, x, {
	return std::log2(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(mad, mvalue, avalue, bvalue, {
	if constexpr (std::is_constant_evaluated()) {
		return mvalue * avalue + bvalue;
	}
	else {
		return fma(mvalue, avalue, bvalue);
	}
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(min, x, y, {
	return SDL_min(x, y);
});

template <typename T>
constexpr auto modf(const T x, T &ip) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return std::modf(x, std::addressof(ip));
}

template <typename T, std::size_t M, std::size_t N>
constexpr auto modf(const T x, internal::matrix<T, M, N> &ip) noexcept
	-> internal::matrix<T, M, N>
	requires (std::is_arithmetic_v<T>)
{
	internal::matrix<T, M, N> res;
	res.front() = modf(x, ip.front());

	for (const auto i : std::views::iota(std::size_t{1}, M * N)) {
		res.std::template array<T, M * N>::operator[](i) = res.front();
		ip.std::template array<T, M * N>::operator[](i) = ip.front();
	}

	return res;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
constexpr auto modf(const internal::matrix<T, M1, N1> &x, internal::matrix<T, M2, N2> &ip) noexcept
	-> internal::matrix<T, M1, N1>
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)
{
	internal::matrix<T, M1, N1> res;

	for (const auto [res, x, ip] : std::views::zip(res, x, ip)) {
		res = modf(x, ip);
	}

	return res;
}

template <typename T, typename U>
[[nodiscard]]
constexpr auto mul(const T x, const U y) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U>)
{
	return x * y;
}

template <typename T, std::size_t M, std::size_t N, typename U>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M, N> &x, const U y) noexcept
	requires (std::is_arithmetic_v<U>)
{
	return x * y;
}

template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto mul(const T x, const internal::matrix<U, M, N> &y) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return x * y;
}

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M1, N1> &x, const internal::matrix<U, M2, N2> &y) noexcept
	-> internal::matrix<std::common_type_t<T, U>, M1 == 1 || N1 == 1 ? 1 : M1, M2 == 1 || N2 == 1 ? 1 : N2>
	requires ((M1 == 1 || N1 == 1) && M1 * N1 == M2 || (M2 == 1 || N2 == 1) && M2 * N2 == N1 || N1 == M2)
{
	constexpr auto M{M1 == 1 || N1 == 1 ? 1 : M1};
	constexpr auto N{(M1 == 1 || N1 == 1) && M1 * N1 == M2 ? M2 : N1};
	constexpr auto P{M2 == 1 || N2 == 1 ? 1 : N2};
	internal::matrix<std::common_type_t<T, U>, M, P> res{};

	for (const auto m : std::views::iota(std::size_t{}, M)) {
		for (const auto n : std::views::iota(std::size_t{}, N)) {
			for (const auto p : std::views::iota(std::size_t{}, P)) {
				res.std::template array<T, M * P>::operator[](mad(m, P, p)) += x.std::template array<T, M * N>::operator[](mad(m, N, n)) * y.std::template array<T, N * P>::operator[](mad(n, P, p));
			}
		}
	}

	return res;
}

template <typename T>
[[nodiscard]]
constexpr auto normalize(const T x) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return x / length(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto normalize(const internal::matrix<T, M, N> &x) noexcept
	requires (M == 1 || N == 1)
{
	return x / length(x);
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(radians, x, {
	return std::numbers::pi_v<T> / 180 * x;
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(rcp, x, {
	return 1 / x;
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(reflect, i, n, {
	return i - 2 * n * dot(i, n);
});

#define STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL(zero)\
{\
	const auto n_dot_i{dot(n, i)};\
	const auto k{1 - eta * eta * (1 - n_dot_i * n_dot_i)};\
\
	if (k < 0) {\
		return zero;\
	}\
\
	return eta * i - mad(eta, n_dot_i, sqrt(k)) * n;\
}

template <typename T, typename U, typename V>
[[nodiscard]]
constexpr auto refract(const T i, const U n, const V eta) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>)
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((std::common_type_t<T, U, V>{}));

template <typename T, std::size_t M, std::size_t N, typename U, typename V>
[[nodiscard]]
constexpr auto refract(const internal::matrix<T, M, N> &i, const U n, const V eta) noexcept
	requires (std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && (M == 1 || N == 1))
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<std::common_type_t<T, U, V>, M, N>{}));

template <typename T, typename U, std::size_t M, std::size_t N, typename V>
[[nodiscard]]
constexpr auto refract(const T i, const internal::matrix<U, M, N> &n, const V eta) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<V> && (M == 1 || N == 1))
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<std::common_type_t<T, U, V>, M, N>{}));

template <typename T, std::size_t M1, std::size_t N1, typename U, std::size_t M2, std::size_t N2, typename V>
[[nodiscard]]
constexpr auto refract(const internal::matrix<T, M1, N1> &i, const internal::matrix<U, M2, N2> &n, const V eta) noexcept
	requires (std::is_arithmetic_v<V> && (M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<std::common_type_t<T, U, V>, M1, N1>{}));

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(round, x, {
	return std::round(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(rsqrt, x, {
	return rcp(sqrt(x));
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(saturate, x, {
	return clamp(x, T{0}, T{1});
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(sign, x, {
	return static_cast<T>((0 < x) - (x < 0));
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(sin, x, {
	return std::sin(x);
});

template <typename T>
constexpr void sincos(const T x, T &s, T &c) noexcept
	requires (std::is_arithmetic_v<T>)
{
	s = sin(x);
	c = cos(x);
}

template <typename T, std::size_t M, std::size_t N>
constexpr void sincos(const T x, internal::matrix<T, M, N> &s, T &c) noexcept
{
	s.front() = sin(x);

	for (const auto i : std::views::iota(std::size_t{1}, M * N)) {
		s.std::template array<T, M * N>::operator[](i) = s.front();
	}

	c = cos(x);
}

template <typename T, std::size_t M, std::size_t N>
constexpr void sincos(const T x, T &s, internal::matrix<T, M, N> &c) noexcept
{
	s = sin(x);
	c.front() = cos(x);

	for (const auto i : std::views::iota(std::size_t{1}, M * N)) {
		c.std::template array<T, M * N>::operator[](i) = c.front();
	}
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
constexpr void sincos(const T x, internal::matrix<T, M1, N1> &s, internal::matrix<T, M2, N2> &c) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)
{
	s.front() = sin(x);
	c.front() = cos(x);

	for (const auto i : std::views::iota(std::size_t{1}, M1 * N1)) {
		s.std::template array<T, M1 * N1>::operator[](i) = s.front();
		c.std::template array<T, M2 * N2>::operator[](i) = c.front();
	}
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
constexpr void sincos(const internal::matrix<T, M1, N1> &x, internal::matrix<T, M2, N2> &s, internal::matrix<T, M3, N3> &c) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) && M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 || M1 == M2 && M2 == M3 && N1 == N2 && N2 == N3)
{
	for (const auto [x, s, c] : std::views::zip(x, s, c)) {
		s = sin(x);
		c = cos(x);
	}
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(sinh, x, {
	return std::sinh(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARG_OPERATION_IMPL(smoothstep, min, max, x, {
	const auto t{saturate((x - min) / (max - min))};
	return t * t * (3 - 2 * t);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARG_OPERATION_IMPL(step, y, x, {
	return x < y ? (std::common_type_t<T, U>{0}) : (std::common_type_t<T, U>{1});
})

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(tan, x, {
	return std::tan(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(tanh, x, {
	return std::tanh(x);
});

template <typename T>
[[nodiscard]]
constexpr auto transpose(const T x) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return x;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto transpose(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<T, N, M>
{
	internal::matrix<T, N, M> res;

	for (const auto m : std::views::iota(std::size_t{}, M)) {
		for (const auto n : std::views::iota(std::size_t{}, N)) {
			res.std::template array<T, N * M>::operator[](mad(n, M, m)) = x.std::template array<T, M * N>::operator[](mad(m, N, n));
		}
	}

	return res;
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARG_OPERATION_IMPL(trunc, x, {
	return std::trunc(x);
});
}

#endif
