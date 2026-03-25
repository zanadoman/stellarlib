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

#include <algorithm>
#include <cstddef>

#include <stellarlib/lin/matrix.hpp>

#include <cmath>

namespace stellarlib::lin::internal
{
/* [[nodiscard]] */
/* constexpr auto perspective(const float fovy, const float aspect, const float near, const float far) noexcept */
/* { */
/* 	const float f = 1.0F / std::tanf(fovy * 0.5F); */
/* 	return matrix<float, 4, 4>{ */
/* 		f / aspect, 0.0F, 0.0F, 0.0F, */
/* 		0.0F, f, 0.0F, 0.0F, */
/* 		0.0F, 0.0F, (far + near) / (near - far), -1.0F, */
/* 		0.0F, 0.0F, 2.0F * far * near / (near - far), 0.0F, */
/* 	}; */
/* } */

template <typename T, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto all(const matrix<T, M, N> &matrix) noexcept
{
	return std::ranges::find(matrix, false) == matrix.end();
}

template <typename T, typename U, std::size_t M, std::size_t N, std::size_t P>
[[nodiscard]]
constexpr auto mul(const matrix<T, M, N> &lhs, const matrix<U, P, N> &rhs) noexcept
	-> matrix<std::common_type_t<T, U>, M, P>
{
	matrix<std::common_type_t<T, U>, M, P> res{};

	for (const auto m : std::views::iota(std::size_t{}, M)) {
		for (const auto p : std::views::iota(std::size_t{}, P)) {
			for (const auto n : std::views::iota(std::size_t{}, N)) {
				res[m * P + p] += lhs[m * N + n] * rhs[n * P + p];
			}
		}
	}

	return res;
}

/* static_assert(all(mul(matrix<int, 1, 2>{1, 2}, matrix<int, 2, 2>{4, 6, 5, 7}) == matrix<int, 1, 2>{16, 19})); */
/* static_assert(all(mul(matrix<int, 1, 3>{1, 2, 3}, matrix<int, 2, 3>{4, 6, 8, 5, 7, 9}) == matrix<int, 1, 2>{40, 46})); */
/* static_assert(all(mul(matrix<int, 2, 3>{1, 2, 3, 4, 5, 6}, matrix<int, 2, 3>{1, 3, 5, 2, 4, 6}) == matrix<int, 2, 2>{22, 49, 28, 64})); */


static_assert(all(mul(matrix<int, 1, 2>{1, 2}, matrix<int, 1, 2>{3, 4}) == matrix<int, 1, 1>{11}));
static_assert(all(mul(matrix<int, 1, 3>{0, 0, 1}, mul(matrix<int, 3, 3>{1, 0, 0, 0, 1, 0, 2, 3, 1}, matrix<int, 3, 3>{2, 0, 0, 0, 1, 0, 0, 0, 1})) == matrix<int, 1, 3>{4, 3, 1}));
}

#endif
