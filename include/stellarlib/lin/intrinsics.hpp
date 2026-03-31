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
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numbers>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin
{
template <typename T>
[[nodiscard]]
constexpr auto abs(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>)
{
	return x < 0 ? -x : x;
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto abs(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = abs(x);
	}

	return ret;
}

[[nodiscard]]
auto acos(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto acos(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = acos(x);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto all(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t> || std::is_same_v<T, bool>)
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
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t> || std::is_same_v<T, bool>)
{
	return static_cast<bool>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto any(const internal::matrix<T, M, N> &x) noexcept
{
	return std::ranges::any_of(x, static_cast<bool (*)(T)>(any));
}

[[nodiscard]]
constexpr auto asdouble(const std::uint32_t lowbits, const std::uint32_t highbits) noexcept
{
	return std::bit_cast<double>(static_cast<std::uint64_t>(highbits) << std::uint8_t{32} | static_cast<std::uint64_t>(lowbits));
}

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto asdouble(const internal::matrix<std::uint32_t, M1, N1> &lowbits, const internal::matrix<std::uint32_t, M2, N2> &highbits)
	-> internal::matrix<double, M1, N1>
	requires (M1 * N1 == M2 * N2 && (M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1))
{
	internal::matrix<double, M1, N1> ret;

	for (const auto [ret, lowbits, highbits] : std::views::zip(ret, lowbits, highbits)) {
		ret = asdouble(lowbits, highbits);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto asfloat(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t> || std::is_same_v<T, std::uint32_t>)
{
	return std::bit_cast<float>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto asfloat(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = asfloat(x);
	}

	return ret;
}

[[nodiscard]]
auto asin(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto asin(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = asin(x);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto asint(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::uint32_t>)
{
	return std::bit_cast<std::int32_t>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto asint(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<std::int32_t, M, N>
{
	internal::matrix<std::int32_t, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = asint(x);
	}

	return ret;
}

void asuint(double value, std::uint32_t &lowbits, std::uint32_t &highbits) noexcept;

template <typename T>
[[nodiscard]]
constexpr auto asuint(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>)
{
	return std::bit_cast<std::uint32_t>(x);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto asuint(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<std::uint32_t, M, N>
{
	internal::matrix<std::uint32_t, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = asuint(x);
	}

	return ret;
}

[[nodiscard]]
auto atan(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto atan(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = atan(x);
	}

	return ret;
}

[[nodiscard]]
auto atan2(float y, float x) noexcept
	-> float;

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto atan2(const internal::matrix<float, M1, N1> &y, const internal::matrix<float, M2, N2> &x) noexcept
	-> internal::matrix<float, M1, N1>
	requires (M1 * N1 == M2 * N2 && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) || M1 == M2 && N1 == N2))
{
	internal::matrix<float, M1, N1> ret;

	for (const auto [ret, y, x] : std::views::zip(ret, y, x)) {
		ret = atan2(y, x);
	}

	return ret;
}

[[nodiscard]]
auto ceil(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto ceil(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = ceil(x);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto clamp(const T x, const T min, const T max) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>)
{
	return SDL_clamp(x, min, max);
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
[[nodiscard]]
constexpr auto clamp(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &min, const internal::matrix<T, M3, N3> &max) noexcept
	-> internal::matrix<T, M1, N1>
	requires (M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) || M1 == M2 && M2 == M3 && N1 == N2 && N2 == N3))
{
	internal::matrix<T, M1, N1> ret;

	for (const auto [ret, x, min, max] : std::views::zip(ret, x, min, max)) {
		ret = clamp(x, min, max);
	}

	return ret;
}

[[nodiscard]]
auto cos(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto cos(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = cos(x);
	}

	return ret;
}

[[nodiscard]]
auto cosh(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto cosh(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = cosh(x);
	}

	return ret;
}

[[nodiscard]]
constexpr auto countbits(const std::uint32_t value) noexcept
{
	return static_cast<std::uint32_t>(std::popcount(value));
}

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto countbits(const internal::matrix<std::uint32_t, M, N> &value) noexcept
	-> internal::matrix<std::uint32_t, M, N>
	requires (M == 1 || N == 1)
{
	internal::matrix<std::uint32_t, M, N> ret;

	for (const auto [ret, value] : std::views::zip(ret, value)) {
		ret = countbits(value);
	}

	return ret;
}

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto cross(const internal::matrix<float, M1, N1> &x, const internal::matrix<float, M2, N2> &y)
	requires (M1 * N1 == 3 && M2 * N2 == 3)
{
	return internal::matrix<float, M1, N1>{x.y() * y.z() - x.z() * y.y(), x.z() * y.x() - x.x() * y.z(), x.x() * y.y() - x.y() * y.x()};
}

[[nodiscard]]
constexpr auto degrees(const float x) noexcept
{
	return 180 / std::numbers::pi_v<float> * x;
}

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto degrees(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = degrees(x);
	}

	return ret;
}

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto length(const internal::matrix<float, M, N> &x) noexcept
	requires (M == 1 || N == 1)
{
	return std::sqrtf(std::ranges::fold_left(x | std::views::transform([] [[nodiscard]] (const auto x) noexcept -> auto {
		return x * x;
	}), 0.0F, std::plus<float>{}));
}

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto distance(const internal::matrix<float, M1, N1> &x, const internal::matrix<float, M2, N2> &y) noexcept
	requires (M1 * N1 == M2 * N2 && (M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1))
{
	return length(x - y);
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto dot(const internal::matrix<T, M1, N1> &x, const internal::matrix<T, M2, N2> &y) noexcept
	requires ((std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>) && M1 * N1 == M2 * N2 && (M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1))
{
	return std::ranges::fold_left(std::views::zip(x, y) | std::views::transform([] [[nodiscard]] (const auto xy) noexcept -> auto {
		return std::get<0>(xy) * std::get<1>(xy);
	}), 0.0F, std::plus<float>{});
}

[[nodiscard]]
auto exp(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto exp(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = exp(x);
	}

	return ret;
}

[[nodiscard]]
auto exp2(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto exp2(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = exp2(x);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto sign(const T x) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, std::int32_t>)
{
	return static_cast<T>((0 < x) - (x < 0));
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto sign(const internal::matrix<T, M, N> &x) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = sign(x);
	}

	return ret;
}

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
[[nodiscard]]
constexpr auto faceforward(const internal::matrix<float, M1, N1> &n, const internal::matrix<float, M2, N2> &i, const internal::matrix<float, M3, N3> &ng) noexcept
	-> internal::matrix<float, M1, N1>
	requires (M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1)))
{
	return -n * sign(dot(i, ng));
}

[[nodiscard]]
auto floor(float x) noexcept
	-> float;

template <std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto floor(const internal::matrix<float, M, N> &x) noexcept
	-> internal::matrix<float, M, N>
{
	internal::matrix<float, M, N> ret;

	for (const auto [ret, x] : std::views::zip(ret, x)) {
		ret = floor(x);
	}

	return ret;
}

template <typename T>
[[nodiscard]]
constexpr auto mad(const T mvalue, const T avalue, const T bvalue) noexcept
	requires (std::is_same_v<T, float> || std::is_same_v<T, double> || std::is_same_v<T, std::int32_t> || std::is_same_v<T, std::uint32_t>)
{
	return mvalue * avalue + bvalue;
}

template <typename T, std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
[[nodiscard]]
constexpr auto mad(const internal::matrix<T, M1, N1> &mvalue, const internal::matrix<T, M2, N2> &avalue, const internal::matrix<T, M3, N3> &bvalue) noexcept
	-> internal::matrix<T, M1, N1>
	requires (M1 * N1 == M2 * N2 && M2 * N2 == M3 * N3 && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) && (M3 == 1 || N3 == 1) || M1 == M2 && M2 == M3 && N1 == N2 && N2 == N3))
{
	internal::matrix<T, M1, N1> ret;

	for (const auto [ret, mvalue, avalue, bvalue] : std::views::zip(ret, mvalue, avalue, bvalue)) {
		ret = mad(mvalue, avalue, bvalue);
	}

	return ret;
}

[[nodiscard]]
constexpr auto fma(const double a, const double b, const double c)
{
	return mad(a, b, c);
}

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2, std::size_t M3, std::size_t N3>
[[nodiscard]]
constexpr auto fma(const internal::matrix<double, M1, N1> &a, const internal::matrix<double, M2, N2> &b, const internal::matrix<double, M3, N3> &c) noexcept
{
	return mad(a, b, c);
}

[[nodiscard]]
auto fmod(float x, float y) noexcept
	-> float;

template <std::size_t M1, std::size_t N1, std::size_t M2, std::size_t N2>
[[nodiscard]]
constexpr auto fmod(const internal::matrix<float, M1, N1> &x, const internal::matrix<float, M2, N2> &y) noexcept
	-> internal::matrix<float, M1, N1>
	requires (M1 * N1 == M2 * N2 && ((M1 == 1 || N1 == 1) && (M2 == 1 || N2 == 1) || M1 == M2 && N1 == N2))
{
	internal::matrix<float, M1, N1> ret;

	for (const auto [ret, x, y] : std::views::zip(ret, x, y)) {
		ret = fmod(x, y);
	}

	return ret;
}
}

#endif
