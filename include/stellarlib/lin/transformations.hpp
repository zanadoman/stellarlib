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

#ifndef STELLARLIB_LIN_TRANSFORMATIONS_HPP
#define STELLARLIB_LIN_TRANSFORMATIONS_HPP

#include <stellarlib/lin/intrinsics.hpp>
#include <stellarlib/lin/matrix.hpp>

#include <cstddef>
#include <ranges>
#include <type_traits>

namespace stellarlib::lin
{
template <typename T, std::size_t N>
[[nodiscard]]
constexpr auto identity() noexcept
	-> internal::matrix<T, N, N>
{
	internal::matrix<T, N, N> res;

	for (const auto i : std::views::iota(std::size_t{}, N * N)) {
		res.std::template array<T, N * N>::operator[](i) = i % (N + 1) ? 0 : 1;
	}

	return res;
}

template <typename T>
[[nodiscard]]
constexpr auto perspective(const T fovy, const T aspect, const T near, const T far) noexcept
	-> internal::matrix<T, 4, 4>
{
	const auto f{internal::rcp(internal::tan(fovy / 2))};

	return internal::matrix<T, 4, 4>{
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), -1,
		0, 0, 2 * far * near / (near - far), 0
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto translate(const internal::matrix<T, M, N> &v) noexcept
	requires (M * N == 2)
{
	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		1, 0, 0,
		0, 1, 0,
		v.x(), v.y(), 1
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto translate(const internal::matrix<T, M, N> &v) noexcept
	requires (M * N == 3)
{
	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		v.x(), v.y(), v.z(), 1
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto rotate(const T angle, const T axis) noexcept
	requires (std::is_arithmetic_v<T> && M * N == 2)
{
	const auto cos{internal::cos(angle)};
	const auto sin{internal::sin(angle) * internal::normalize(axis)};

	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		cos, sin, 0,
		-sin, cos, 0,
		0, 0, 1
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto rotate(const T angle, const internal::matrix<T, M, N> &axis) noexcept
	requires (std::is_arithmetic_v<T> && M * N == 3)
{
	const auto cos{internal::cos(angle)};
	const auto sin{internal::sin(angle)};
	const auto normal{internal::normalize(axis)};

	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto scale(const internal::matrix<T, M, N> &v) noexcept
	requires (M * N == 2)
{
	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		v.x(), 0, 0,
		0, v.y(), 0,
		0, 0, 1
	};
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto scale(const internal::matrix<T, M, N> &v) noexcept
	requires (M * N == 3)
{
	return internal::matrix<T, internal::max(M, N) + 1, internal::max(M, N) + 1>{
		v.x(), 0, 0, 0,
		0, v.y(), 0, 0,
		0, 0, v.z(), 0,
		0, 0, 0, 1
	};
}
}

#endif
