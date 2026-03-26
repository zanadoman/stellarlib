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

#include <stellarlib/lin/matrix.hpp>

#include <stellarlib/lin/intrinsics.hpp>

#include <cstdint>

#include <gtest/gtest.h>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[0] == 1);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[1] == 3);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[2] == 2);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[3] == 4);
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} == lin::internal::matrix<std::int32_t, 1, 2>{lin::internal::matrix<std::int32_t, 2, 1>{1, 2}}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 2, 1>{1, 2} == lin::internal::matrix<std::int32_t, 2, 1>{lin::internal::matrix<std::int32_t, 1, 2>{1, 2}}));

static_assert(lin::all(+lin::internal::matrix<std::int32_t, 1, 2>{1, 2} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(-lin::internal::matrix<std::int32_t, 1, 2>{1, 2} == lin::internal::matrix<std::int32_t, 1, 2>{-1, -2}));
static_assert(lin::all(!lin::internal::matrix<bool, 1, 2>{true, false} == lin::internal::matrix<bool, 1, 2>{false, true}));
static_assert(lin::all(~lin::internal::matrix<std::uint8_t, 1, 2>{std::uint8_t{1}, std::uint8_t{2}} == lin::internal::matrix<std::uint8_t, 1, 2>{std::uint8_t{254}, std::uint8_t{253}}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{3, 8}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 8} / lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 5} % lin::internal::matrix<std::int32_t, 1, 2>{2, 3} == lin::internal::matrix<std::int32_t, 1, 2>{0, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{4, 6}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 6} - lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_increment_and_decrement)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	ASSERT_TRUE(lin::all(++matrix == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix++ == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));
	ASSERT_TRUE(lin::all(--matrix == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix-- == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

#pragma clang diagnostic pop
