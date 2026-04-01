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
#define STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(op, expr)\
template <typename T>\
[[nodiscard]]\
constexpr auto op(const T x) noexcept\
	requires (std::is_arithmetic_v<T>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &x) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, x] : std::views::zip(res, x)) {\
		res = op(x);\
	}\
\
	return res;\
}

#define STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(op, expr)\
template <typename T>\
[[nodiscard]]\
constexpr auto op(const T x, const T y) noexcept\
	requires (std::is_arithmetic_v<T>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &x, const T y) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, x] : std::views::zip(res, x)) {\
		res = op(x, y);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const T &x, const internal::matrix<T, M, N> y) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, y] : std::views::zip(res, y)) {\
		res = op(x, y);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept\
	-> internal::matrix<T, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)\
{\
	internal::matrix<T, M1, N1> res;\
\
	for (const auto [res, x, y] : std::views::zip(res, x, y)) {\
		res = op(x, y);\
	}\
\
	return res;\
}

#define STELLARLIB_LIN_INTRINSICS_TRIPLE_ARGUMENT_OPERATION_IMPL(op, expr)\
template <typename T>\
[[nodiscard]]\
constexpr auto op(const T x, const T y, const T z) noexcept\
	requires (std::is_arithmetic_v<T>)\
{\
	expr\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M, N> &x, const T y, const T z) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, x] : std::views::zip(res, x)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const T x, const internal::matrix<T, M, N> &y, const T z) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, y] : std::views::zip(res, y)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto op(const T x, const T y, const internal::matrix<T, M, N> &z) noexcept\
	-> internal::matrix<T, M, N>\
{\
	internal::matrix<T, M, N> res;\
\
	for (const auto [res, z] : std::views::zip(res, z)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y, const T z) noexcept\
	-> internal::matrix<T, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)\
{\
	internal::matrix<T, M1, N1> res;\
\
	for (const auto [res, x, y] : std::views::zip(res, x, y)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &x, const T y, const internal::matrix<T, M2, N2> &z) noexcept\
	-> internal::matrix<T, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)\
{\
	internal::matrix<T, M1, N1> res;\
\
	for (const auto [res, x, z] : std::views::zip(res, x, z)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>\
[[nodiscard]]\
constexpr auto op(const T x, const internal::matrix<T, M1, N1> &y, const internal::matrix<T, M2, N2> &z) noexcept\
	-> internal::matrix<T, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2 || M1 == M2 && N1 == N2)\
{\
	internal::matrix<T, M1, N1> res;\
\
	for (const auto [res, y, z] : std::views::zip(res, y, z)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}\
\
template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>\
[[nodiscard]]\
constexpr auto op(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y, const internal::matrix<T, M3, N3> &z) noexcept\
	-> internal::matrix<T, M1, N1>\
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) && M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 || M1 == M2 && M2 == M3 && N1 == N2 && N2 == N3)\
{\
	internal::matrix<T, M1, N1> res;\
\
	for (const auto [res, x, y, z] : std::views::zip(res, x, y, z)) {\
		res = op(x, y, z);\
	}\
\
	return res;\
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(abs, {
	return x < 0 ? -x : x;
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(acos, {
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

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(asin, {
	return std::asin(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(atan, {
	return std::atan(x);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(atan2, {
	return std::atan2(x, y);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(ceil, {
	return std::ceil(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARGUMENT_OPERATION_IMPL(clamp, {
	return SDL_clamp(x, y, z);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(cos, {
	return std::cos(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(cosh, {
	return std::cosh(x);
});

template <typename T>
[[nodiscard]]
constexpr auto cross([[maybe_unused]] const T x, [[maybe_unused]] const T y) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return lin::internal::matrix<T, 1, 3>{};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto cross(const internal::matrix<T, M, N> &x, const T y) noexcept
	requires (M * N == 3)
{
	return internal::matrix<float, M, N>{x.y() * y - x.z() * y, x.z() * y - x.x() * y, x.x() * y - x.y() * y};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto cross(const T x, const internal::matrix<T, M, N> &y) noexcept
	requires (M * N == 3)
{
	return internal::matrix<float, M, N>{x * y.z() - x * y.y(), x * y.x() - x * y.z(), x * y.y() - x * y.x()};
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto cross(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept
	requires (M1 * N1 == 3 && M2 * N2 == 3)
{
	return internal::matrix<float, M1, N1>{x.y() * y.z() - x.z() * y.y(), x.z() * y.x() - x.x() * y.z(), x.x() * y.y() - x.y() * y.x()};
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(degrees, {
	return 180 / std::numbers::pi_v<T> * x;
});

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 1, 1> &x) noexcept
{
	return x;
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 2, 2> &x) noexcept
{
	return x[0, 0] * x[1, 1] - x[0, 1] * x[1, 0];
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 3, 3> &x) noexcept
{
	return x[0, 0] * (x[1, 1] * x[2, 2] - x[1, 2] * x[2, 1]) - x[0, 1] * (x[1, 0] * x[2, 2] - x[1, 2] * x[2, 0]) + x[0, 2] * (x[1, 0] * x[2, 1] - x[1, 1] * x[2, 0]);
}

template <typename T>
[[nodiscard]]
constexpr auto determinant(const internal::matrix<T, 4, 4> &x) noexcept
{
	return x[0, 0] * (x[1, 1] * (x[2, 2] * x[3, 3] - x[2, 3] * x[3, 2]) - x[1, 2] * (x[2, 1] * x[3, 3] - x[2, 3] * x[3, 1]) + x[1, 3] * (x[2, 1] * x[3, 2] - x[2, 2] * x[3, 1])) - x[0, 1] * (x[1, 0] * (x[2, 2] * x[3, 3] - x[2, 3] * x[3, 2]) - x[1, 2] * (x[2, 0] * x[3, 3] - x[2, 3] * x[3, 0]) + x[1, 3] * (x[2, 0] * x[3, 2] - x[2, 2] * x[3, 0])) + x[0, 2] * (x[1, 0] * (x[2, 1] * x[3, 3] - x[2, 3] * x[3, 1]) - x[1, 1] * (x[2, 0] * x[3, 3] - x[2, 3] * x[3, 0]) + x[1, 3] * (x[2, 0] * x[3, 1] - x[2, 1] * x[3, 0])) - x[0, 3] * (x[1, 0] * (x[2, 1] * x[3, 2] - x[2, 2] * x[3, 1]) - x[1, 1] * (x[2, 0] * x[3, 2] - x[2, 2] * x[3, 0]) + x[1, 2] * (x[2, 0] * x[3, 1] - x[2, 1] * x[3, 0]));
}

template <typename T, std::size_t N>
[[nodiscard]]
constexpr auto determinant(internal::matrix<T, N, N> x) noexcept
{
	T determinant{1};

	for (const auto i : std::views::iota(std::size_t{}, N)) {
		auto pivot{i};

		for (const auto j : std::views::iota(i + 1, N)) {
			if (abs(x[pivot, i]) < abs(x[j, i])) {
				pivot = j;
			}
		}

		if (pivot != i) {
			for (const auto j : std::views::iota(i, N)) {
				std::swap(x[i, j], x[pivot, j]);
			}

			determinant *= -1;
		}

		if (!static_cast<bool>(x[i, i])) {
			return T{};
		}

		determinant *= x[i, i];

		for (const auto j : std::views::iota(i + 1, N)) {
			x[j, i] /= x[i, i];

			for (const auto k : std::views::iota(i + 1, N)) {
				x[j, k] -= x[j, i] * x[i, k];
			}
		}
	}

	return determinant;
}

template <typename T>
[[nodiscard]]
constexpr auto distance(const T x, const T y) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return abs(y - x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto distance(const internal::matrix<T, M, N> &x, const T y) noexcept
	requires (M == 1 || N == 1)
{
	return std::sqrt(std::ranges::fold_left(x | std::views::transform([y] [[nodiscard]] (const auto x) noexcept -> auto {
		return y - x;
	}) | std::views::transform([] [[nodiscard]] (const auto d) noexcept -> auto {
		return d * d;
	}), 0, std::plus{}));
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto distance(const T x, const internal::matrix<T, M, N> &y) noexcept
	requires (M == 1 || N == 1)
{
	return std::sqrt(std::ranges::fold_left(y | std::views::transform([x] [[nodiscard]] (const auto y) noexcept -> auto {
		return y - x;
	}) | std::views::transform([] [[nodiscard]] (const auto d) noexcept -> auto {
		return d * d;
	}), 0, std::plus{}));
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto distance(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return std::sqrt(std::ranges::fold_left(std::views::zip(x, y) | std::views::transform([] [[nodiscard]] (const auto xy) noexcept -> auto {
		return std::get<1>(xy) - std::get<0>(xy);
	}) | std::views::transform([] [[nodiscard]] (const auto d) noexcept -> auto {
		return d * d;
	}), 0, std::plus{}));
}

template <typename T>
[[nodiscard]]
constexpr auto dot(const T x, const T y) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return x * y;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto dot(const internal::matrix<T, M, N> &x, const T y) noexcept
	requires (M == 1 || N == 1)
{
	return std::ranges::fold_left(x | std::views::transform([y] [[nodiscard]] (const auto x) noexcept -> auto {
		return x * y;
	}), 0, std::plus{});
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto dot(const T x, const internal::matrix<T, M, N> &y) noexcept
	requires (M == 1 || N == 1)
{
	return std::ranges::fold_left(y | std::views::transform([x] [[nodiscard]] (const auto y) noexcept -> auto {
		return x * y;
	}), 0, std::plus{});
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto dot(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	return std::ranges::fold_left(std::views::zip(x, y) | std::views::transform([] [[nodiscard]] (const auto xy) noexcept -> auto {
		return std::get<0>(xy) * std::get<1>(xy);
	}), 0, std::plus{});
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(exp, {
	return std::exp(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(exp2, {
	return std::exp2(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(sign, {
	return static_cast<T>((0 < x) - (x < 0));
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
	-> lin::internal::matrix<T, M, N>
	requires (M == 1 || N == 1)
{
	lin::internal::matrix<T, M, N> res;

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
	-> lin::internal::matrix<T, M, N>
	requires (M == 1 || N == 1)
{
	lin::internal::matrix<T, M, N> res;

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
	-> lin::internal::matrix<T, M1, N1>
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
{
	lin::internal::matrix<T, M1, N1> res;

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

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(floor, {
	return std::floor(x);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(fmod, {
	return std::fmod(x, y);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(frac, {
	return x - floor(x);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(ldexp, {
	return std::ldexp(x, static_cast<std::int32_t>(y));
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

	if constexpr (M * N == 3) {
		return std::hypot(x.x(), x.y(), x.z());
	}

	return std::sqrt(std::ranges::fold_left(x | std::views::transform([] [[nodiscard]] (const auto x) noexcept -> auto {
		return x * x;
	}), 0, std::plus{}));
}

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARGUMENT_OPERATION_IMPL(lerp, {
	return std::lerp(x, y, z);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(log, {
	return std::log(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(log10, {
	return std::log10(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(log2, {
	return std::log2(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARGUMENT_OPERATION_IMPL(mad, {
	return x * y + z;
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(max, {
	return SDL_max(x, y);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(min, {
	return SDL_min(x, y);
});

template <typename T>
constexpr auto modf(const T x, T &ip) noexcept
{
	return std::modf(x, std::addressof(ip));
}

template <typename T, std::size_t M, std::size_t N>
constexpr auto modf(const T x, internal::matrix<T, M, N> &ip) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> res;
	res.front() = modf(x, ip.front());

	for (const auto i : std::views::iota(std::size_t{1}, M * N)) {
		res.std::template array<T, M * N>::operator[](i) = res.front();
		ip.std::template array<T, M * N>::operator[](i) = ip.front();
	}

	return res;
}

template <typename T, std::size_t M, std::size_t N>
constexpr auto modf(const internal::matrix<T, M, N> &x, internal::matrix<T, M, N> &ip) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> res;

	for (const auto [res, x, ip] : std::views::zip(res, x, ip)) {
		res = modf(x, ip);
	}

	return res;
}

template <typename T>
[[nodiscard]]
constexpr auto mul(const T x, const T y) noexcept
	requires (std::is_arithmetic_v<T>)
{
	return x * y;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M, N> &x, const T y) noexcept
{
	return x * y;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto mul(const T x, const internal::matrix<T, M, N> &y) noexcept
{
	return x * y;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept
	-> internal::matrix<T, M1 == 1 || N1 == 1 ? 1 : M1, M2 == 1 || N2 == 1 ? 1 : N2>
	requires ((M1 == 1 || N1 == 1) && M1 * N1 == M2 || (M2 == 1 || N2 == 1) && M2 * N2 == N1 || N1 == M2)
{
	constexpr auto M{M1 == 1 || N1 == 1 ? 1 : M1};
	constexpr auto N{(M1 == 1 || N1 == 1) && M1 * N1 == M2 ? M2 : N1};
	constexpr auto P{M2 == 1 || N2 == 1 ? 1 : N2};
	internal::matrix<T, M, P> res{};

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

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(pow, {
	return std::pow(x, y);
});

template <typename T>
[[nodiscard]]
constexpr auto lit(const T n_dot_l, const T n_dot_h, const T m) noexcept
{
	return internal::matrix<T, 1, 4>{1, max(n_dot_l, T{}), n_dot_l < 0 || n_dot_h < 0 ? 0 : pow(n_dot_h, m), 1};
}

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(radians, {
	return std::numbers::pi_v<T> / 180 * x;
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(rcp, {
	return 1 / x;
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(reflect, {
	return x - 2 * y * dot(x, y);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(sqrt, {
	return std::sqrt(x);
});

#define STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL(zero)\
{\
	const T n_dot_i{dot(n, i)};\
	const T k{1 - eta * eta * (1 - n_dot_i * n_dot_i)};\
\
	if (k < 0) {\
		return zero;\
	}\
\
	return eta * i - (eta * n_dot_i + sqrt(k)) * n;\
}

template <typename T>
[[nodiscard]]
constexpr auto refract(const T i, const T n, const T eta) noexcept
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL(T{});

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto refract(const internal::matrix<T, M, N> &i, const T n, const T eta) noexcept
	requires (M == 1 || N == 1)
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<T, M, N>{}));

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto refract(const T i, const internal::matrix<T, M, N> &n, const T eta) noexcept
	requires (M == 1 || N == 1)
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<T, M, N>{}));

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto refract(const internal::matrix<T, M1, N1> &i, const internal::matrix<T, M2, N2> &n, const T eta) noexcept
	requires ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && M1 * N1 == M2 * N2)
STELLARLIB_LIN_INTRINSICS_REFRACT_IMPL((internal::matrix<T, M1, N1>{}));

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(round, {
	return std::round(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(rsqrt, {
	return rcp(sqrt(x));
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(saturate, {
	return clamp(x, T{0}, T{1});
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(sin, {
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

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(sinh, {
	return std::sinh(x);
});

STELLARLIB_LIN_INTRINSICS_TRIPLE_ARGUMENT_OPERATION_IMPL(smoothstep, {
	const auto t{saturate((z - x) / (y - x))};
	return t * t * (3 - 2 * t);
});

STELLARLIB_LIN_INTRINSICS_DOUBLE_ARGUMENT_OPERATION_IMPL(step, {
	return x < y ? T{0} : T{1};
})

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(tan, {
	return std::tan(x);
});

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(tanh, {
	return std::tanh(x);
});

template <typename T>
[[nodiscard]]
constexpr auto transpose(const T x) noexcept
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

STELLARLIB_LIN_INTRINSICS_SINGLE_ARGUMENT_OPERATION_IMPL(trunc, {
	return std::trunc(x);
});
}

#endif
