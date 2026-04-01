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
#include <ranges>

namespace stellarlib::lin
{
template <typename T>
[[nodiscard]]
constexpr auto acos(const T arg) noexcept
{
	return std::acos(arg);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto acos(const internal::matrix<T, M, N> &matrix) noexcept
	-> internal::matrix<T, M, N>
{
	internal::matrix<T, M, N> res;

	for (const auto [lhs, rhs] : std::views::zip(res, matrix)) {
		lhs = acos(rhs);
	}

	return res;
}

template <typename T>
[[nodiscard]]
constexpr auto all(const T arg) noexcept
{
	return static_cast<bool>(arg);
}

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const internal::matrix<T, M, N> &matrix) noexcept
{
	return std::ranges::all_of(matrix, static_cast<bool (*)(T)>(all));
}
}

#endif
