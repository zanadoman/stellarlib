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

#include <stellarlib/lin/transformations.hpp>

#include <stellarlib/lin/intrinsics.hpp>
#include <stellarlib/lin/matrix.hpp>

#include <gtest/gtest.h>

#include <limits>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

TEST(stellarlib_lib_transformations, translate)
{
	ASSERT_TRUE(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 1.0F}, lin::translate(lin::internal::matrix<float, 3, 3>{1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F})) == lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 4>{0.0F, 0.0F, 0.0F, 1.0F}, lin::translate(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F})) == lin::internal::matrix<float, 1, 4>{2.0F, 3.0F, 4.0F, 1.0F}));
}

TEST(stellarlib_lib_transformations, rotate)
{
	const auto vector1{lin::internal::mul(lin::internal::matrix<float, 1, 3>{1.0F, 0.0F, 1.0F}, lin::rotate(lin::internal::matrix<float, 3, 3>{1.0F}, lin::internal::radians(90.0F)))};
	ASSERT_NEAR(vector1.x(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector1.y(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector1.z(), 1.0F, std::numeric_limits<float>::epsilon());
	auto vector2{lin::internal::mul(lin::internal::matrix<float, 1, 4>{0.0F, 1.0F, 0.0F, 1.0F}, lin::rotate(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::radians(90.0F), lin::internal::matrix<float, 1, 3>{1.0F, 0.0F, 0.0F}))};
	ASSERT_NEAR(vector2.x(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.y(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.z(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.w(), 1.0F, std::numeric_limits<float>::epsilon());
	vector2 = lin::internal::mul(vector2, lin::rotate(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::radians(90.0F), lin::internal::matrix<float, 1, 3>{0.0F, 1.0F, 0.0F}));
	ASSERT_NEAR(vector2.x(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.y(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.z(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.w(), 1.0F, std::numeric_limits<float>::epsilon());
	vector2 = lin::internal::mul(vector2, lin::rotate(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::radians(90.0F), lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 1.0F}));
	ASSERT_NEAR(vector2.x(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.y(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.z(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.w(), 1.0F, std::numeric_limits<float>::epsilon());
}

TEST(stellarlib_lib_transformations, scale)
{
	ASSERT_TRUE(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}, lin::scale(lin::internal::matrix<float, 3, 3>{1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F})) == lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, 1.0F, 1.0F}, lin::scale(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F})) == lin::internal::matrix<float, 1, 4>{2.0F, 3.0F, 4.0F, 1.0F}));
}

TEST(stellarlib_lib_transformations, should_transform)
{
	const auto vector1{lin::internal::mul(lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}, lin::scale(lin::rotate(lin::translate(lin::internal::matrix<float, 3, 3>{1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}), lin::internal::radians(90.0F)), lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}))};
	ASSERT_NEAR(vector1.x(), -1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector1.y(), 5.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector1.z(), 1.0F, std::numeric_limits<float>::epsilon());
	const auto vector2{lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, 1.0F, 1.0F}, lin::scale(lin::rotate(lin::translate(lin::internal::matrix<float, 4, 4>{1.0F}, lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F}), lin::internal::radians(90.0F), lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 1.0F}), lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F}))};
	ASSERT_NEAR(vector2.x(), -1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.y(), 5.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.z(), 8.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector2.w(), 1.0F, std::numeric_limits<float>::epsilon());
}

TEST(stellarlib_lib_transformations, perspective)
{
	const auto matrix{lin::perspective(lin::internal::degrees(60.0F), 16.0F / 9.0F, 0.1F)};
	const auto vector{lin::internal::mul(lin::internal::matrix<float, 1, 4>{0.0F, 0.0F, -1.0F, 1.0F}, matrix)};
	ASSERT_NEAR(vector.x(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_NEAR(vector.y(), 0.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, vector.z());
	ASSERT_NEAR(vector.w(), 1.0F, std::numeric_limits<float>::epsilon());
	auto far_tl{lin::internal::mul(lin::internal::matrix<float, 1, 4>{-1.0F, 1.0F, -2.0F, 1.0F}, matrix)};
	far_tl /= far_tl.w();
	auto far_bl{lin::internal::mul(lin::internal::matrix<float, 1, 4>{-1.0F, -1.0F, -2.0F, 1.0F}, matrix)};
	far_bl /= far_bl.w();
	auto far_br{lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, -1.0F, -2.0F, 1.0F}, matrix)};
	far_br /= far_br.w();
	auto far_tr{lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, -2.0F, 1.0F}, matrix)};
	far_tr /= far_tr.w();
	ASSERT_LT(far_tl.x(), 0.0F);
	ASSERT_NEAR(far_tl.x(), far_bl.x(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_tl.z());
	ASSERT_NEAR(far_tl.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(far_bl.y(), 0.0F);
	ASSERT_NEAR(far_bl.y(), far_br.y(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_bl.z());
	ASSERT_NEAR(far_bl.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_br.x());
	ASSERT_NEAR(far_br.x(), far_tr.x(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_br.z());
	ASSERT_NEAR(far_br.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_tr.y());
	ASSERT_NEAR(far_tr.y(), far_tl.y(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, far_tr.z());
	ASSERT_NEAR(far_tr.w(), 1.0F, std::numeric_limits<float>::epsilon());
	auto near_tl{lin::internal::mul(lin::internal::matrix<float, 1, 4>{-1.0F, 1.0F, -1.0F, 1.0F}, matrix)};
	near_tl /= near_tl.w();
	auto near_bl{lin::internal::mul(lin::internal::matrix<float, 1, 4>{-1.0F, -1.0F, -1.0F, 1.0F}, matrix)};
	near_bl /= near_bl.w();
	auto near_br{lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, -1.0F, -1.0F, 1.0F}, matrix)};
	near_br /= near_br.w();
	auto near_tr{lin::internal::mul(lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, -1.0F, 1.0F}, matrix)};
	near_tr /= near_tr.w();
	ASSERT_LT(near_tl.x(), far_tl.x());
	ASSERT_NEAR(near_tl.x(), near_bl.x(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, near_tl.z());
	ASSERT_NEAR(near_tl.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(near_bl.y(), far_bl.y());
	ASSERT_NEAR(near_bl.y(), near_br.y(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, near_bl.z());
	ASSERT_NEAR(near_bl.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(far_br.x(), near_br.x());
	ASSERT_NEAR(near_br.x(), near_tr.x(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, near_br.z());
	ASSERT_NEAR(near_br.w(), 1.0F, std::numeric_limits<float>::epsilon());
	ASSERT_LT(far_tr.y(), near_tr.y());
	ASSERT_NEAR(near_tr.y(), near_tl.y(), std::numeric_limits<float>::epsilon());
	ASSERT_LT(0.0F, near_tr.z());
	ASSERT_NEAR(near_tr.w(), 1.0F, std::numeric_limits<float>::epsilon());
}

#pragma clang diagnostic pop
