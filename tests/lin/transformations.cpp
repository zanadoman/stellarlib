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

#include <stellarlib/lin/intrinsics.hpp>
#include <stellarlib/lin/matrix.hpp>
#include <stellarlib/lin/transformations.hpp>

#include <gtest/gtest.h>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::internal::all(lin::identity<float, 4>() == lin::internal::matrix<float, 4, 4>{1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F}));

TEST(stellarlib_lib_transformations, perspective)
{
	const auto matrix{lin::perspective(1.0F, 2.0F, 1.0F, 2.0F)};
	ASSERT_EQ((matrix[0, 0]), lin::internal::rcp(lin::internal::tan(0.5F)) / 2.0F);
	ASSERT_EQ((matrix[1, 0]), 0.0F);
	ASSERT_EQ((matrix[2, 0]), 0.0F);
	ASSERT_EQ((matrix[3, 0]), 0.0F);
	ASSERT_EQ((matrix[0, 1]), 0.0F);
	ASSERT_EQ((matrix[1, 1]), lin::internal::rcp(lin::internal::tan(0.5F)));
	ASSERT_EQ((matrix[2, 1]), 0.0F);
	ASSERT_EQ((matrix[3, 1]), 0.0F);
	ASSERT_EQ((matrix[0, 2]), 0.0F);
	ASSERT_EQ((matrix[1, 2]), 0.0F);
	ASSERT_EQ((matrix[2, 2]), -3.0F);
	ASSERT_EQ((matrix[3, 2]), -4.0F);
	ASSERT_EQ((matrix[0, 3]), 0.0F);
	ASSERT_EQ((matrix[1, 3]), 0.0F);
	ASSERT_EQ((matrix[2, 3]), -1.0F);
	ASSERT_EQ((matrix[3, 3]), 0.0F);
}

static_assert(lin::internal::all(lin::translate(lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 3, 3>{1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 2.0F, 3.0F, 1.0F}));
static_assert(lin::internal::all(lin::translate(lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F}) == lin::internal::matrix<float, 4, 4>{1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 2.0F, 3.0F, 4.0F, 1.0F}));

static_assert(lin::internal::all(lin::scale(lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 3, 3>{2.0F, 0.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 0.0F, 1.0F}));
static_assert(lin::internal::all(lin::scale(lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F}) == lin::internal::matrix<float, 4, 4>{2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 0.0F, 0.0F, 4.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F}));

#pragma clang diagnostic pop
