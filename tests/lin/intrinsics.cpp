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

#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <numbers>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::abs(1.0F) == lin::abs(-1.0F));
static_assert(lin::all(lin::abs(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));

TEST(stellarlib_lin_intrinsics, acos)
{
	ASSERT_EQ(lin::acos(0.0F), std::acosf(0.0F));
	ASSERT_TRUE(lin::all(lin::acos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::acosf(0.5F), std::acosf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::acos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::acosf(0.5F), std::acosf(-0.5F), std::acosf(0.5F), std::acosf(-0.5F)}));
}

static_assert(lin::all(1.0F));
static_assert(!lin::all(0.0F));
static_assert(lin::all(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}));
static_assert(!lin::all(lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));

static_assert(lin::any(1.0F));
static_assert(!lin::any(0.0F));
static_assert(lin::any(lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(!lin::any(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));

TEST(stellarlib_lin_intrinsics, asin)
{
	ASSERT_EQ(lin::asin(0.0F), std::asinf(0.0F));
	ASSERT_TRUE(lin::all(lin::asin(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::asinf(0.5F), std::asinf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::asin(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::asinf(0.5F), std::asinf(-0.5F), std::asinf(0.5F), std::asinf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, atan)
{
	ASSERT_EQ(lin::atan(0.0F), std::atanf(0.0F));
	ASSERT_TRUE(lin::all(lin::atan(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::atanf(0.5F), std::atanf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::atan(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::atanf(0.5F), std::atanf(-0.5F), std::atanf(0.5F), std::atanf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, atan2)
{
	ASSERT_EQ(lin::atan2(0.5F, -0.5F), std::atan2f(0.5F, -0.5F));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.0F) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, 0.0F), std::atan2f(-0.5F, 0.0F)}));
	ASSERT_TRUE(lin::all(lin::atan2(0.0F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.0F, 0.5F), std::atan2f(0.0F, -0.5F)}));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F), std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
}

TEST(stellarlib_lin_intrinsics, ceil)
{
	ASSERT_EQ(lin::ceil(0.0F), std::ceilf(0.0F));
	ASSERT_TRUE(lin::all(lin::ceil(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::ceilf(0.5F), std::ceilf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::ceil(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::ceilf(0.5F), std::ceilf(-0.5F), std::ceilf(0.5F), std::ceilf(-0.5F)}));
}

static_assert(lin::clamp(-2.0F, -1.0F, 1.0F) == -1.0F);
static_assert(lin::clamp(0.0F, -1.0F, 1.0F) == 0.0F);
static_assert(lin::clamp(2.0F, -1.0F, 1.0F) == 1.0F);
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, -1.0F, 1.0F) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(-2.0F, lin::internal::matrix<float, 1, 3>{-2.0F, -1.0F, 0.0F}, 1.0F) == lin::internal::matrix<float, 1, 3>{-2.0F, -1.0F, 0.0F}));
static_assert(lin::all(lin::clamp(2.0F, -1.0F, lin::internal::matrix<float, 1, 3>{0.0F, 1.0F, 2.0F}) == lin::internal::matrix<float, 1, 3>{0.0, 1.0F, 2.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 3, 1>{-1.0F, -1.0F, -1.0F}, 1.0F) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, 1.0F) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, -1.0F, lin::internal::matrix<float, 3, 1>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, -1.0F, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(0.0F, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 3, 1>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));
static_assert(lin::all(lin::clamp(0.0F, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 3, 1>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 3, 1>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 3, 1>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 3, 3>{-2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 3, 3>{-1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 3, 3>{1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 3, 3>{-1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F}));

TEST(stellarlib_lin_intrinsics, cos)
{
	ASSERT_EQ(lin::cos(0.0F), std::cosf(0.0F));
	ASSERT_TRUE(lin::all(lin::cos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::cosf(0.5F), std::cosf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::cos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::cosf(0.5F), std::cosf(-0.5F), std::cosf(0.5F), std::cosf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, cosh)
{
	ASSERT_EQ(lin::cosh(0.0F), std::coshf(0.0F));
	ASSERT_TRUE(lin::all(lin::cosh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::coshf(0.5F), std::coshf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::cosh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::coshf(0.5F), std::coshf(-0.5F), std::coshf(0.5F), std::coshf(-0.5F)}));
}

static_assert(lin::all(lin::cross(1.0F, 2.0F) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));
static_assert(lin::all(lin::cross(lin::internal::matrix<float, 1, 3>{1.0F, 2.0F, 3.0F}, 4.0F) == lin::internal::matrix<float, 1, 3>{-4.0F, 8.0F, -4.0F}));
static_assert(lin::all(lin::cross(1.0F, lin::internal::matrix<float, 1, 3>{2.0F, 3.0F, 4.0F}) == lin::internal::matrix<float, 1, 3>{1.0F, -2.0F, 1.0F}));
static_assert(lin::all(lin::cross(lin::internal::matrix<float, 1, 3>{1.0F, 2.0F, 3.0F}, lin::internal::matrix<float, 3, 1>{4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 1, 3>{-3.0F, 6.0F, -3.0F}));
static_assert(lin::all(lin::cross(lin::internal::matrix<float, 1, 3>{1.0F, 2.0F, 3.0F}, lin::internal::matrix<float, 1, 3>{4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 1, 3>{-3.0F, 6.0F, -3.0F}));

static_assert(lin::degrees(std::numbers::pi_v<float>) == 180.0F);
static_assert(lin::all(lin::degrees(lin::internal::matrix<float, 1, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}) == lin::internal::matrix<float, 1, 2>{180.0F, 360.0F}));
static_assert(lin::all(lin::degrees(lin::internal::matrix<float, 2, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2, std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}) == lin::internal::matrix<float, 2, 2>{180.0F, 360.0F, 180.0F, 360.0F}));

static_assert(lin::determinant(lin::internal::matrix<float, 1, 1>{0.0F}) == 0.0F);
static_assert(lin::determinant(lin::internal::matrix<float, 2, 2>{1.0F, 1.0F, 2.0F, 2.0F}) == 0.0F);
static_assert(lin::determinant(lin::internal::matrix<float, 3, 3>{1.0F, 1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F, 3.0F}) == 0.0F);
static_assert(lin::determinant(lin::internal::matrix<float, 4, 4>{1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F, 4.0F, 4.0F, 4.0F, 4.0F}) == 0.0F);
static_assert(lin::determinant(lin::internal::matrix<float, 5, 5>{1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 2.0F, 2.0F, 2.0F, 2.0F, 2.0F, 3.0F, 3.0F, 3.0F, 3.0F, 3.0F, 4.0F, 4.0F, 4.0F, 4.0F, 4.0F, 5.0F, 5.0F, 5.0F, 5.0F, 5.0F}) == 0.0F);

static_assert(lin::distance(1.0F, -1.0F) == 2.0F);

TEST(stellarlib_lin_intrinsics, distance)
{
	ASSERT_EQ(lin::distance(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 5.0F), 5.0F);
	ASSERT_EQ(lin::distance(1.0F, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}), 5.0F);
	ASSERT_EQ(lin::distance(lin::internal::matrix<float, 1, 2>{-1.5F, -2.0F}, lin::internal::matrix<float, 2, 1>{1.5F, 2.0F}), 5.0F);
	ASSERT_EQ(lin::distance(lin::internal::matrix<float, 1, 2>{-1.5F, -2.0F}, lin::internal::matrix<float, 1, 2>{1.5F, 2.0F}), 5.0F);
}

static_assert(lin::dot(1.0F, 2.0F) == 2.0F);
static_assert(lin::all(lin::dot(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F) == 9.0F));
static_assert(lin::all(lin::dot(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == 5.0F));
static_assert(lin::dot(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}) == 11.0F);
static_assert(lin::dot(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == 11.0F);

TEST(stellarlib_lin_intrinsics, exp)
{
	ASSERT_EQ(lin::exp(0.0F), std::expf(0.0F));
	ASSERT_TRUE(lin::all(lin::exp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::expf(0.5F), std::expf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::exp(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::expf(0.5F), std::expf(-0.5F), std::expf(0.5F), std::expf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, exp2)
{
	ASSERT_EQ(lin::exp2(0.0F), std::exp2f(0.0F));
	ASSERT_TRUE(lin::all(lin::exp2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::exp2f(0.5F), std::exp2f(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::exp2(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::exp2f(0.5F), std::exp2f(-0.5F), std::exp2f(0.5F), std::exp2f(-0.5F)}));
}

static_assert(lin::sign(-2.0F) == -1.0F);
static_assert(lin::sign(0.0F) == 0.0F);
static_assert(lin::sign(2.0F) == 1.0F);
static_assert(lin::all(lin::sign(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::sign(lin::internal::matrix<float, 3, 3>{-2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F}) == lin::internal::matrix<float, 3, 3>{-1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F}));

static_assert(lin::faceforward(1.0F, 2.0F, 3.0F) == -1.0F);
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, 4.0F) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, 4.0F) == lin::internal::matrix<float, 1, 2>{-1.0F, -1.0F}));
static_assert(lin::all(lin::faceforward(1.0F, 2.0F, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -1.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, 5.0F) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, 5.0F) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, lin::internal::matrix<float, 2, 1>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 2, 1>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -1.0F}));
static_assert(lin::all(lin::faceforward(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -1.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 2, 1>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));

TEST(stellarlib_lin_intrinsics, floor)
{
	ASSERT_EQ(lin::floor(0.0F), std::floorf(0.0F));
	ASSERT_TRUE(lin::all(lin::floor(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::floorf(0.5F), std::floorf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::floor(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::floorf(0.5F), std::floorf(-0.5F), std::floorf(0.5F), std::floorf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, fmod)
{
	ASSERT_EQ(lin::fmod(3.14F, 1.5F), std::fmodf(3.14F, 1.5F));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, 1.5F) == lin::internal::matrix<float, 1, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 1.5F)}));
	ASSERT_TRUE(lin::all(lin::fmod(6.28F, lin::internal::matrix<float, 1, 2>{1.5F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::fmodf(6.28F, 1.5F), std::fmodf(6.28F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, lin::internal::matrix<float, 2, 1>{1.5F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, lin::internal::matrix<float, 1, 2>{1.5F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}, lin::internal::matrix<float, 2, 2>{1.5F, 3.0F, 1.5F, 3.0F}) == lin::internal::matrix<float, 2, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F), std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
}

TEST(stellarlib_lin_intrinsics, frac)
{
	ASSERT_EQ(lin::frac(0.0F), 0.0F);
	ASSERT_TRUE(lin::all(lin::frac(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
	ASSERT_TRUE(lin::all(lin::frac(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));
}

TEST(stellarlib_lin_intrinsics, ldexp)
{
	ASSERT_EQ(lin::ldexp(0.5F, 2.0F), std::ldexpf(0.5F, 2.0F));
	ASSERT_TRUE(lin::all(lin::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 2.0F) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, 2.0F), std::ldexpf(-0.5F, 2.0F)}));
	ASSERT_TRUE(lin::all(lin::ldexp(0.5F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, 2.0F), std::ldexpf(0.5F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{2.0F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, 2.0F), std::ldexpf(-0.5F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, 2.0F), std::ldexpf(-0.5F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::ldexp(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{2.0F, 3.0F, 2.0F, 3.0F}) == lin::internal::matrix<float, 2, 2>{std::ldexpf(0.5F, 2.0F), std::ldexpf(-0.5F, 3.0F), std::ldexpf(0.5F, 2.0F), std::ldexpf(-0.5F, 3.0F)}));
}

static_assert(lin::lerp(0.0F, 2.0F, 0.25F) == 0.5F);
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, 2.0F, 0.25F) == lin::internal::matrix<float, 1, 2>{0.5F, 1.25F}));
static_assert(lin::all(lin::lerp(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 0.25F) == lin::internal::matrix<float, 1, 2>{0.25F, 0.5F}));
static_assert(lin::all(lin::lerp(0.0F, 2.0F, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{2.0F, 3.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 2.0F, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 2.0F, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::all(lin::lerp(0.0F, lin::internal::matrix<float, 1, 2>{2.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::all(lin::lerp(0.0F, lin::internal::matrix<float, 1, 2>{2.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 2.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{2.0F, 3.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 2.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 2, 1>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 2.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 2.5F}));
static_assert(lin::all(lin::lerp(lin::internal::matrix<float, 2, 2>{0.0F, 1.0F, 0.0F, 1.0F}, lin::internal::matrix<float, 2, 2>{2.0F, 3.0F, 2.0F, 3.0F}, lin::internal::matrix<float, 2, 2>{0.25F, 0.75F, 0.25F, 0.75F}) == lin::internal::matrix<float, 2, 2>{0.5F, 2.5F, 0.5F, 2.5F}));

static_assert(lin::length(1.0F) == lin::length(-1.0F));

TEST(stellarlib_lin_intrinsics, length)
{
	ASSERT_EQ(lin::length(lin::internal::matrix<float, 1, 2>{-3.0F, -4.0F}), 5.0F);
	ASSERT_EQ(lin::length(lin::internal::matrix<float, 1, 3>{-1.0F, -2.0F, -2.0F}), 3.0F);
}

TEST(stellarlib_lin_intrinsics, log)
{
	ASSERT_EQ(lin::log(0.0F), std::logf(0.0F));
	ASSERT_TRUE(lin::all(lin::log(lin::internal::matrix<float, 1, 2>{5.0F, 0.0F}) == lin::internal::matrix<float, 1, 2>{std::logf(5.0F), std::logf(0.0F)}));
	ASSERT_TRUE(lin::all(lin::log(lin::internal::matrix<float, 2, 2>{0.5F, 0.0F, 0.5F, 0.0F}) == lin::internal::matrix<float, 2, 2>{std::logf(0.5F), std::logf(0.0F), std::logf(0.5F), std::logf(0.0F)}));
}

TEST(stellarlib_lin_intrinsics, log10)
{
	ASSERT_EQ(lin::log10(0.0F), std::log10f(0.0F));
	ASSERT_TRUE(lin::all(lin::log10(lin::internal::matrix<float, 1, 2>{5.0F, 0.0F}) == lin::internal::matrix<float, 1, 2>{std::log10f(5.0F), std::log10f(0.0F)}));
	ASSERT_TRUE(lin::all(lin::log10(lin::internal::matrix<float, 2, 2>{0.5F, 0.0F, 0.5F, 0.0F}) == lin::internal::matrix<float, 2, 2>{std::log10f(0.5F), std::log10f(0.0F), std::log10f(0.5F), std::log10f(0.0F)}));
}

TEST(stellarlib_lin_intrinsics, log2)
{
	ASSERT_EQ(lin::log2(0.0F), std::log2f(0.0F));
	ASSERT_TRUE(lin::all(lin::log2(lin::internal::matrix<float, 1, 2>{5.0F, 0.0F}) == lin::internal::matrix<float, 1, 2>{std::log2f(5.0F), std::log2f(0.0F)}));
	ASSERT_TRUE(lin::all(lin::log2(lin::internal::matrix<float, 2, 2>{0.5F, 0.0F, 0.5F, 0.0F}) == lin::internal::matrix<float, 2, 2>{std::log2f(0.5F), std::log2f(0.0F), std::log2f(0.5F), std::log2f(0.0F)}));
}

static_assert(lin::mad(1.0F, 2.0F, 3.0F) == 5.0F);
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, 4.0F) == lin::internal::matrix<float, 1, 2>{7.0F, 10.0F}));
static_assert(lin::all(lin::mad(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, 4.0F) == lin::internal::matrix<float, 1, 2>{6.0F, 7.0F}));
static_assert(lin::all(lin::mad(1.0F, 2.0F, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, 5.0F) == lin::internal::matrix<float, 1, 2>{8.0F, 13.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, 5.0F) == lin::internal::matrix<float, 1, 2>{8.0F, 13.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, lin::internal::matrix<float, 2, 1>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{7.0F, 11.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{7.0F, 11.0F}));
static_assert(lin::all(lin::mad(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 2, 1>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{6.0F, 8.0F}));
static_assert(lin::all(lin::mad(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}) == lin::internal::matrix<float, 1, 2>{6.0F, 8.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 2, 1>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{3.0F, 4.0F, 3.0F, 4.0F}, lin::internal::matrix<float, 2, 2>{5.0F, 6.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 2, 2>{8.0F, 14.0F, 8.0F, 14.0F}));

static_assert(lin::max(0.5F, -0.5F) == 0.5F);
static_assert(lin::all(lin::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.0F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.0F}));
static_assert(lin::all(lin::max(0.0F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.0F}));
static_assert(lin::all(lin::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::max(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));

static_assert(lin::min(0.5F, -0.5F) == -0.5F);
static_assert(lin::all(lin::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.0F) == lin::internal::matrix<float, 1, 2>{0.0F, -0.5F}));
static_assert(lin::all(lin::min(0.0F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, -0.5F}));
static_assert(lin::all(lin::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::all(lin::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::all(lin::min(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{-0.5F, -0.5F, -0.5F, -0.5F}));

TEST(stellarlib_lin_intrinsics, modf)
{
	float ip1{};
	float ip2{};
	ASSERT_EQ(lin::modf(3.14F, ip1), std::modff(3.14F, std::addressof(ip2)));
	ASSERT_EQ(ip1, ip2);
	lin::internal::matrix<float, 1, 2> ip3{};
	lin::internal::matrix<float, 1, 2> ip4{};
	ASSERT_TRUE(lin::all(lin::modf(3.14F, ip3) == lin::internal::matrix<float, 1, 2>{std::modff(3.14F, std::addressof(ip4.x())), std::modff(3.14F, std::addressof(ip4.y()))}));
	ASSERT_TRUE(lin::all(ip3 == ip4));
	ASSERT_TRUE(lin::all(lin::modf(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, ip3) == lin::internal::matrix<float, 1, 2>{std::modff(3.14F, std::addressof(ip4.x())), std::modff(6.28F, std::addressof(ip4.y()))}));
	ASSERT_TRUE(lin::all(ip3 == ip4));
	lin::internal::matrix<float, 2, 2> ip5{};
	lin::internal::matrix<float, 2, 2> ip6{};
	ASSERT_TRUE(lin::all(lin::modf(3.14F, ip5) == lin::internal::matrix<float, 2, 2>{std::modff(3.14F, std::addressof(ip6[0, 0])), std::modff(3.14F, std::addressof(ip6[0, 1])), std::modff(3.14F, std::addressof(ip6[1, 0])), std::modff(3.14F, std::addressof(ip6[1, 1]))}));
	ASSERT_TRUE(lin::all(ip3 == ip4));
	ASSERT_TRUE(lin::all(lin::modf(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}, ip5) == lin::internal::matrix<float, 2, 2>{std::modff(3.14F, std::addressof(ip6[0, 0])), std::modff(6.28F, std::addressof(ip6[0, 1])), std::modff(3.14F, std::addressof(ip6[1, 0])), std::modff(6.28F, std::addressof(ip6[1, 1]))}));
	ASSERT_TRUE(lin::all(ip3 == ip4));
}

static_assert(lin::mul(1.0F, 2.0F) == 2.0F);
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F) == lin::internal::matrix<float, 1, 2>{3.0F, 6.0F}));
static_assert(lin::all(lin::mul(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}) == 11.0F));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == 11.0F));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 3.0F, 4.0F}, 5.0F) == lin::internal::matrix<float, 2, 2>{5.0F, 10.0F, 15.0F, 20.0F}));
static_assert(lin::all(lin::mul(1.0F, lin::internal::matrix<float, 2, 2>{2.0F, 3.0F, 4.0F, 5.0F}) == lin::internal::matrix<float, 2, 2>{2.0F, 3.0F, 4.0F, 5.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 2, 3>{1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F}, lin::internal::matrix<float, 3, 4>{7.0F, 8.0F, 9.0F, 10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F, 17.0F, 18.0F}) == lin::internal::matrix<float, 2, 4>{74.0F, 80.0F, 86.0F, 92.0F, 173.0F, 188.0F, 203.0F, 218.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 3.0F, 4.0F}, lin::internal::matrix<float, 2, 1>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{17.0F, 39.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{17.0F, 39.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{3.0F, 4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{13.0F, 16.0F}));
static_assert(lin::all(lin::mul(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{3.0F, 4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{13.0F, 16.0F}));

static_assert(lin::normalize(2.0F) == 1.0F);
static_assert(lin::normalize(-2.0F) == -1.0F);

TEST(stellarlib_lin_intrinsics, normalize)
{
	ASSERT_TRUE(lin::all(lin::normalize(lin::internal::matrix<float, 1, 2>{0.0, 2.0F}) == lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}));
	ASSERT_TRUE(lin::all(lin::normalize(lin::internal::matrix<float, 1, 3>{0.0, 0.0F, 2.0F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 1.0F}));
	ASSERT_TRUE(lin::all(lin::normalize(lin::internal::matrix<float, 1, 4>{0.0, 0.0F, 0.0F, 2.0F}) == lin::internal::matrix<float, 1, 4>{0.0F, 0.0F, 0.0F, 1.0F}));
}

TEST(stellarlib_lin_intrinsics, pow)
{
	ASSERT_EQ(lin::pow(0.5F, -0.5F), std::powf(0.5F, -0.5F));
	ASSERT_TRUE(lin::all(lin::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.0F) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, 0.0F), std::powf(-0.5F, 0.0F)}));
	ASSERT_TRUE(lin::all(lin::pow(0.5F, lin::internal::matrix<float, 1, 2>{-0.5F, 0.0F}) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, -0.5F), std::powf(0.5F, 0.0F)}));
	ASSERT_TRUE(lin::all(lin::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::all(lin::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::all(lin::pow(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F), std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
}

TEST(stellarlib_lin_intrinsics, lit)
{
	ASSERT_TRUE(lin::all(lin::lit(-1.0F, 2.0F, 3.0F) == lin::internal::matrix<float, 1, 4>{1.0F, 0.0F, 0.0F, 1.0F}));
	ASSERT_TRUE(lin::all(lin::lit(1.0F, -1.0F, 3.0F) == lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, 0.0F, 1.0F}));
	ASSERT_TRUE(lin::all(lin::lit(1.0F, 2.0F, 3.0F) == lin::internal::matrix<float, 1, 4>{1.0F, 1.0F, 8.0F, 1.0F}));
}

static_assert(lin::radians(180.0F) == std::numbers::pi_v<float>);
static_assert(lin::all(lin::radians(lin::internal::matrix<float, 1, 2>{180.0F, 360.0F}) == lin::internal::matrix<float, 1, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}));
static_assert(lin::all(lin::radians(lin::internal::matrix<float, 2, 2>{180.0F, 360.0F, 180.0F, 360.0F}) == lin::internal::matrix<float, 2, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2, std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}));

static_assert(lin::rcp(2.0F) == 0.5F);
static_assert(lin::all(lin::rcp(lin::internal::matrix<float, 1, 2>{2.0F, 4.0F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.25F}));
static_assert(lin::all(lin::rcp(lin::internal::matrix<float, 2, 2>{2.0F, 4.0F, 2.0F, 4.0F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.25F, 0.5F, 0.25F}));

static_assert(lin::reflect(1.0F, 2.0F) == -7.0F);
static_assert(lin::all(lin::reflect(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 3.0F) == lin::internal::matrix<float, 1, 2>{-17.0F, -34.0F}));
static_assert(lin::all(lin::reflect(1.0F, lin::internal::matrix<float, 1, 2>{2.0F, 3.0F}) == lin::internal::matrix<float, 1, 2>{-7.0F, -17.0F}));
static_assert(lin::all(lin::reflect(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}) == lin::internal::matrix<float, 1, 2>{-17.0F, -62.0F}));
static_assert(lin::all(lin::reflect(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == lin::internal::matrix<float, 1, 2>{-17.0F, -62.0F}));
static_assert(lin::all(lin::reflect(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{3.0F, 4.0F, 3.0F, 4.0F}) == lin::internal::matrix<float, 2, 2>{-17.0F, -62.0F, -17.0F, -62.0F}));

TEST(stellarlib_lin_intrinsics, sqrt)
{
	ASSERT_EQ(lin::sqrt(0.0F), std::sqrtf(0.0F));
	ASSERT_TRUE(lin::all(lin::sqrt(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sqrtf(0.5F), std::sqrtf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::sqrt(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::sqrtf(0.5F), std::sqrtf(-0.5F), std::sqrtf(0.5F), std::sqrtf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, refract)
{
	ASSERT_TRUE(lin::all(lin::refract(-0.5F, 0.5F, 1.0F) == lin::internal::matrix<float, 1, 1>{-0.5F}));
	ASSERT_TRUE(lin::all(lin::refract(lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, 0.5F, 1.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
	ASSERT_TRUE(lin::all(lin::refract(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}, 1.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
	ASSERT_TRUE(lin::all(lin::refract(lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}, 1.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
}

TEST(stellarlib_lin_intrinsics, round)
{
	ASSERT_EQ(lin::round(0.0F), std::roundf(0.0F));
	ASSERT_TRUE(lin::all(lin::round(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::roundf(0.5F), std::roundf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::round(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::roundf(0.5F), std::roundf(-0.5F), std::roundf(0.5F), std::roundf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, rsqrt)
{
	ASSERT_EQ(lin::rsqrt(0.0F), 1.0F / std::sqrtf(0.0F));
	ASSERT_TRUE(lin::all(lin::rsqrt(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F / std::sqrtf(0.5F), 1.0F / std::sqrtf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::rsqrt(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F / std::sqrtf(0.5F), 1.0F / std::sqrtf(-0.5F), 1.0F / std::sqrtf(0.5F), 1.0F / std::sqrtf(-0.5F)}));
}

static_assert(lin::saturate(-1.0F) == 0.0F);
static_assert(lin::saturate(0.5F) == 0.5F);
static_assert(lin::saturate(2.0F) == 1.0F);
static_assert(lin::all(lin::saturate(lin::internal::matrix<float, 1, 3>{-1.0F, 0.5F, 2.0F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.5F, 1.0F}));
static_assert(lin::all(lin::saturate(lin::internal::matrix<float, 3, 3>{-1.0F, 0.5F, 2.0F, -1.0F, 0.5F, 2.0F, -1.0F, 0.5F, 2.0F}) == lin::internal::matrix<float, 3, 3>{0.0F, 0.5F, 1.0F, 0.0F, 0.5F, 1.0F, 0.0F, 0.5F, 1.0F}));

TEST(stellarlib_lin_intrinsics, sin)
{
	ASSERT_EQ(lin::sin(0.0F), std::sinf(0.0F));
	ASSERT_TRUE(lin::all(lin::sin(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::sin(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::sinf(0.5F), std::sinf(-0.5F), std::sinf(0.5F), std::sinf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, sincos)
{
	float s1{};
	float c1{};
	lin::sincos(0.0F, s1, c1);
	ASSERT_EQ(s1, std::sinf(0.0F));
	ASSERT_EQ(c1, std::cosf(0.0F));
	lin::internal::matrix<float, 1, 2> s2{};
	lin::sincos(0.0F, s2, c1);
	ASSERT_TRUE(lin::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.0F), std::sinf(0.0F)}));
	ASSERT_EQ(c1, std::cosf(0.0F));
	lin::internal::matrix<float, 1, 2> c2{};
	lin::sincos(0.0F, s1, c2);
	ASSERT_EQ(s1, std::sinf(0.0F));
	ASSERT_TRUE(lin::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.0F), std::cosf(0.0F)}));
	lin::sincos(0.0F, s2, c2);
	ASSERT_TRUE(lin::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.0F), std::sinf(0.0F)}));
	ASSERT_TRUE(lin::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.0F), std::cosf(0.0F)}));
	lin::sincos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, s2, c2);
	ASSERT_TRUE(lin::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.5F), std::cosf(-0.5F)}));
	lin::internal::matrix<float, 2, 2> s3{};
	lin::internal::matrix<float, 2, 2> c3{};
	lin::sincos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, s3, c3);
	ASSERT_TRUE(lin::all(s3 == lin::internal::matrix<float, 2, 2>{std::sinf(0.5F), std::sinf(-0.5F), std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::all(c3 == lin::internal::matrix<float, 2, 2>{std::cosf(0.5F), std::cosf(-0.5F), std::cosf(0.5F), std::cosf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, sinh)
{
	ASSERT_EQ(lin::sinh(0.0F), std::sinhf(0.0F));
	ASSERT_TRUE(lin::all(lin::sinh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sinhf(0.5F), std::sinhf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::sinh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::sinhf(0.5F), std::sinhf(-0.5F), std::sinhf(0.5F), std::sinhf(-0.5F)}));
}

static_assert(lin::smoothstep(0.0F, 1.0F, 0.25F) < 0.175F);
static_assert(lin::smoothstep(0.0F, 1.0F, 0.5F) == 0.5F);
static_assert(0.825F < lin::smoothstep(0.0F, 1.0F, 0.75F));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(0.0F, 1.0F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 2, 1>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::all(lin::smoothstep(lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}, lin::internal::matrix<float, 2, 2>{1.0F, 1.0F, 1.0F, 1.0F}, lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));

static_assert(lin::step(0.0F, 2.0F) == 0.0F);
static_assert(lin::step(0.0F, 0.0F) == 1.0F);
static_assert(lin::all(lin::step(lin::internal::matrix<float, 1, 2>{0.0F, 2.0F}, 2.0F) == lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}));
static_assert(lin::all(lin::step(0.0F, lin::internal::matrix<float, 1, 2>{0.0F, 2.0F}) == lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(lin::all(lin::step(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 2, 1>{0.0F, 2.0F}) == lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(lin::all(lin::step(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{0.0F, 2.0F}) == lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(lin::all(lin::step(lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}, lin::internal::matrix<float, 2, 2>{0.0F, 2.0F, 0.0F, 2.0F}) == lin::internal::matrix<float, 2, 2>{1.0F, 0.0F, 1.0F, 0.0F}));

TEST(stellarlib_lin_intrinsics, tan)
{
	ASSERT_EQ(lin::tan(0.0F), std::tanf(0.0F));
	ASSERT_TRUE(lin::all(lin::tan(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::tanf(0.5F), std::tanf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::tan(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::tanf(0.5F), std::tanf(-0.5F), std::tanf(0.5F), std::tanf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, tanh)
{
	ASSERT_EQ(lin::tanh(0.0F), std::tanhf(0.0F));
	ASSERT_TRUE(lin::all(lin::tanh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::tanhf(0.5F), std::tanhf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::tanh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::tanhf(0.5F), std::tanhf(-0.5F), std::tanhf(0.5F), std::tanhf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, trunc)
{
	ASSERT_EQ(lin::trunc(0.0F), std::truncf(0.0F));
	ASSERT_TRUE(lin::all(lin::trunc(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::truncf(0.5F), std::truncf(-0.5F)}));
	ASSERT_TRUE(lin::all(lin::trunc(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::truncf(0.5F), std::truncf(-0.5F), std::truncf(0.5F), std::truncf(-0.5F)}));
}

static_assert(lin::transpose(0.0F) == 0.0F);
static_assert(lin::all(lin::transpose(lin::internal::matrix<float, 2, 3>{1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 3, 2>{1.0F, 4.0F, 2.0F, 5.0F, 3.0F, 6.0F}));

#pragma clang diagnostic pop
