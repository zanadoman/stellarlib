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
#include <cstddef>
#include <cstdint>
#include <functional>

namespace stellarlib::lin
{
template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const internal::matrix<T, M, N> &matrix) noexcept
{
	return !std::ranges::contains(matrix, false);
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

template <typename T, typename U, std::size_t M, std::size_t N, std::size_t P>
[[nodiscard]]
constexpr auto mul(const internal::matrix<T, M, N> &lhs, const internal::matrix<U, N, P> &rhs) noexcept
	-> internal::matrix<std::common_type_t<T, U>, M, P>
{
	internal::matrix<std::common_type_t<T, U>, M, P> res{};

	for (const auto m : std::views::iota(std::size_t{}, M)) {
		for (const auto n : std::views::iota(std::size_t{}, N)) {
			for (const auto p : std::views::iota(std::size_t{}, P)) {
				res[mad(m, P, p)] += lhs[mad(m, N, n)] * rhs[mad(n, P, p)];
			}
		}
	}

	return res;
}

static_assert(all(mul(internal::matrix<std::int32_t, 1, 2>{1, 2}, internal::matrix<std::int32_t, 1, 2>{3, 4}) == 11));
static_assert(all(mul(internal::matrix<std::int32_t, 2, 1>{1, 2}, internal::matrix<std::int32_t, 2, 1>{3, 4}) == 11));
static_assert(all(mul(internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}, internal::matrix<std::int32_t, 2, 2>{5, 6, 7, 8}) == internal::matrix<std::int32_t, 2, 2>{19, 22, 43, 50}));
static_assert(all(mul(internal::matrix<std::int32_t, 1, 3>{0, 0, 1}, internal::matrix<std::int32_t, 3, 3>{1, 0, 0, 0, 1, 0, 5, 0, 1}) == internal::matrix<std::int32_t, 1, 3>{5, 0, 1}));
static_assert(all(mul(internal::matrix<std::int32_t, 3, 3>{1, 0, 5, 0, 1, 0, 0, 0, 1}, internal::matrix<std::int32_t, 3, 1>{0, 0, 1}) == internal::matrix<std::int32_t, 3, 1>{5, 0, 1}));
}

#endif
