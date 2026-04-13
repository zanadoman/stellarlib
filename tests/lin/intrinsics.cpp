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
#include <cstdint>
#include <numbers>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::internal::abs(0.0F) == 0.0F);
static_assert(lin::internal::all(lin::internal::abs(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::abs(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));

TEST(stellarlib_lin_intrinsics, acos)
{
	ASSERT_EQ(lin::internal::acos(0.0F), std::acosf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::acos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::acosf(0.5F), std::acosf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::acos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::acosf(0.5F), std::acosf(-0.5F), std::acosf(0.5F), std::acosf(-0.5F)}));
}

static_assert(lin::internal::all(0.5F));
static_assert(!lin::internal::all(0.0F));
static_assert(lin::internal::all(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(!lin::internal::all(lin::internal::matrix<float, 1, 2>{0.5F, 0.0F}));
static_assert(lin::internal::all(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}));
static_assert(!lin::internal::all(lin::internal::matrix<float, 2, 2>{0.5F, 0.0F, 0.5F, 0.0F}));

static_assert(lin::internal::any(0.5F));
static_assert(!lin::internal::any(0.0F));
static_assert(lin::internal::any(lin::internal::matrix<float, 1, 2>{0.5F, 0.0F}));
static_assert(!lin::internal::any(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::any(lin::internal::matrix<float, 2, 2>{0.5F, 0.0F, 0.5F, 0.0F}));
static_assert(!lin::internal::any(lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}));

TEST(stellarlib_lin_intrinsics, asin)
{
	ASSERT_EQ(lin::internal::asin(0.0F), std::asinf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::asin(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::asinf(0.5F), std::asinf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::asin(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::asinf(0.5F), std::asinf(-0.5F), std::asinf(0.5F), std::asinf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, atan)
{
	ASSERT_EQ(lin::internal::atan(0.0F), std::atanf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::atanf(0.5F), std::atanf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::atanf(0.5F), std::atanf(-0.5F), std::atanf(0.5F), std::atanf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, atan2)
{
	ASSERT_EQ(lin::internal::atan2(0.5F, -0.5F), std::atan2f(0.5F, -0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, 0.5F), std::atan2f(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan2(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(-0.5F, 0.5F), std::atan2f(-0.5F, -0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::atan2(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F), std::atan2f(0.5F, -0.5F), std::atan2f(-0.5F, 0.5F)}));
}

TEST(stellarlib_lin_intrinsics, ceil)
{
	ASSERT_EQ(lin::internal::ceil(0.0F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::ceil(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::ceil(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F, 0.0F, 1.0F, 0.0F}));
}

static_assert(lin::internal::clamp(0.0F, -0.5F, 0.5F) == 0.0F);
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, -0.5F, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(0.0F, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::clamp(0.0F, -0.5F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 2, 1>{-0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, -0.5F, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, -0.5F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(0.0F, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::clamp(0.0F, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 2, 1>{-0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 2, 1>{1.0F, -1.0F}, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::clamp(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}, lin::internal::matrix<float, 2, 2>{-0.5F, -0.5F, -0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}));

TEST(stellarlib_lin_intrinsics, cos)
{
	ASSERT_EQ(lin::internal::cos(0.0F), std::cosf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::cos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::cosf(0.5F), std::cosf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::cos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::cosf(0.5F), std::cosf(-0.5F), std::cosf(0.5F), std::cosf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, cosh)
{
	ASSERT_EQ(lin::internal::cosh(0.0F), std::coshf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::cosh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::coshf(0.5F), std::coshf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::cosh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::coshf(0.5F), std::coshf(-0.5F), std::coshf(0.5F), std::coshf(-0.5F)}));
}

static_assert(lin::internal::all(lin::internal::cross(0.5F, -0.5F) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::cross(lin::internal::matrix<float, 1, 3>{0.5F, -0.5F, 0.5F}, -0.5F) == lin::internal::matrix<float, 1, 3>{0.5F, 0.0F, -0.5F}));
static_assert(lin::internal::all(lin::internal::cross(-0.5F, lin::internal::matrix<float, 1, 3>{0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 1, 3>{-0.5F, 0.0F, 0.5F}));
static_assert(lin::internal::all(lin::internal::cross(lin::internal::matrix<float, 1, 3>{0.5F, -0.5F, 0.5F}, lin::internal::matrix<float, 3, 1>{-0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::cross(lin::internal::matrix<float, 1, 3>{0.5F, -0.5F, 0.5F}, lin::internal::matrix<float, 1, 3>{-0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 1, 3>{0.0F, 0.0F, 0.0F}));

static_assert(lin::internal::degrees(std::numbers::pi_v<float>) == 180.0F);
static_assert(lin::internal::all(lin::internal::degrees(lin::internal::matrix<float, 1, 2>{std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>}) == lin::internal::matrix<float, 1, 2>{180.0F, 360.0F}));
static_assert(lin::internal::all(lin::internal::degrees(lin::internal::matrix<float, 2, 2>{std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>, std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>}) == lin::internal::matrix<float, 2, 2>{180.0F, 360.0F, 180.0F, 360.0F}));

static_assert(lin::internal::determinant(lin::internal::matrix<float, 1, 1>{0.0F}) == 0.0F);
static_assert(lin::internal::determinant(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == 0.0F);
static_assert(lin::internal::determinant(lin::internal::matrix<float, 3, 3>{0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F}) == 0.0F);
static_assert(lin::internal::determinant(lin::internal::matrix<float, 4, 4>{0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F}) == 0.0F);
static_assert(lin::internal::determinant(lin::internal::matrix<float, 5, 5>{0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F}) == 0.0F);

TEST(stellarlib_lin_intrinsics, sqrt)
{
	ASSERT_EQ(lin::internal::sqrt(0.5F), std::sqrtf(0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::sqrt(lin::internal::matrix<float, 1, 2>{1.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::sqrtf(1.5F), std::sqrtf(0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::sqrt(lin::internal::matrix<float, 2, 2>{1.5F, 0.5F, 1.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::sqrtf(1.5F), std::sqrtf(0.5F), std::sqrtf(1.5F), std::sqrtf(0.5F)}));
}

TEST(stellarlib_lin_intrinsics, length)
{
	static_assert(lin::internal::length(-1.0F) == 1.0F);
	ASSERT_EQ(lin::internal::length(lin::internal::matrix<float, 1, 2>{-3.0F, -4.0F}), 5.0F);
	ASSERT_EQ(lin::internal::length(lin::internal::matrix<float, 1, 3>{-1.0F, -2.0F, -2.0F}), 3.0F);
	ASSERT_EQ(lin::internal::length(lin::internal::matrix<float, 1, 4>{-1.0F, -2.0F, -2.0F, -4.0F}), 5.0F);
}

TEST(stellarlib_lin_intrinsics, distance)
{
	static_assert(lin::internal::distance(0.5F, -0.5F) == 1.0F);
	ASSERT_EQ(lin::internal::distance(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 5.0F), 5.0F);
	ASSERT_EQ(lin::internal::distance(1.0F, lin::internal::matrix<float, 1, 2>{4.0F, 5.0F}), 5.0F);
	ASSERT_EQ(lin::internal::distance(lin::internal::matrix<float, 1, 2>{1.5F, -2.0F}, lin::internal::matrix<float, 2, 1>{-1.5F, 2.0F}), 5.0F);
	ASSERT_EQ(lin::internal::distance(lin::internal::matrix<float, 1, 2>{1.5F, -2.0F}, lin::internal::matrix<float, 1, 2>{-1.5F, 2.0F}), 5.0F);
}

static_assert(lin::internal::dot(0.5F, -0.5F) == -0.25F);
static_assert(lin::internal::all(lin::internal::dot(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == 0.0F));
static_assert(lin::internal::all(lin::internal::dot(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == 0.0F));
static_assert(lin::internal::dot(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == -0.5F);
static_assert(lin::internal::dot(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == -0.5F);

static_assert(lin::internal::all(lin::internal::dst(0.5F, -0.5F) == lin::internal::matrix<float, 1, 4>{1.0F, -0.25F, 0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::dst(lin::internal::matrix<float, 1, 4>{0.5F, -0.5F, 0.5F, -0.5F}, -0.5F) == lin::internal::matrix<float, 1, 4>{1.0F, 0.25F, 0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::dst(-0.5F, lin::internal::matrix<float, 1, 4>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 1, 4>{1.0F, 0.25F, -0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::dst(lin::internal::matrix<float, 1, 4>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 4, 1>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 1, 4>{1.0F, -0.25F, 0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::dst(lin::internal::matrix<float, 1, 4>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 1, 4>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 1, 4>{1.0F, -0.25F, 0.5F, 0.5F}));

TEST(stellarlib_lin_intrinsics, exp)
{
	ASSERT_EQ(lin::internal::exp(0.0F), std::expf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::exp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::expf(0.5F), std::expf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::exp(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::expf(0.5F), std::expf(-0.5F), std::expf(0.5F), std::expf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, exp2)
{
	ASSERT_EQ(lin::internal::exp2(0.0F), std::exp2f(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::exp2(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::exp2f(0.5F), std::exp2f(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::exp2(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::exp2f(0.5F), std::exp2f(-0.5F), std::exp2f(0.5F), std::exp2f(-0.5F)}));
}

static_assert(lin::internal::faceforward(0.5F, -0.5F, 0.5F) == 0.5F);
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, -0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(0.5F, -0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, -5.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, -5.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::faceforward(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));

TEST(stellarlib_lin_intrinsics, floor)
{
	ASSERT_EQ(lin::internal::floor(0.0F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::floor(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, -1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::floor(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.0F, -1.0F, 0.0F, -1.0F}));
}

TEST(stellarlib_lin_intrinsics, fma)
{
	ASSERT_EQ(lin::internal::fma(0.5F, -0.5F, 0.5F), 0.25F);
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(0.5F, -0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.75F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.75F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.25F, 0.25F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.25F, 0.25F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.75F, 0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.75F, 0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fma(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}, lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.25F, -0.75F, 0.25F, -0.75F}));
}

TEST(stellarlib_lin_intrinsics, fmod)
{
	ASSERT_EQ(lin::internal::fmod(0.5F, -0.5F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::fmod(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fmod(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fmod(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fmod(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::fmod(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}));
}

TEST(stellarlib_lin_intrinsics, frac)
{
	ASSERT_EQ(lin::internal::frac(0.0F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::frac(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::frac(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));
}

TEST(stellarlib_lin_intrinsics, ldexp)
{
	ASSERT_EQ(lin::internal::ldexp(0.5F, -0.5F), std::ldexpf(0.5F, static_cast<std::int32_t>(-0.5F)));
	ASSERT_TRUE(lin::internal::all(lin::internal::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, static_cast<std::int32_t>(0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F))}));
	ASSERT_TRUE(lin::internal::all(lin::internal::ldexp(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(-0.5F))}));
	ASSERT_TRUE(lin::internal::all(lin::internal::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, static_cast<std::int32_t>(-0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F))}));
	ASSERT_TRUE(lin::internal::all(lin::internal::ldexp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::ldexpf(0.5F, static_cast<std::int32_t>(-0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F))}));
	ASSERT_TRUE(lin::internal::all(lin::internal::ldexp(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::ldexpf(0.5F, static_cast<std::int32_t>(-0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F)), std::ldexpf(0.5F, static_cast<std::int32_t>(-0.5F)), std::ldexpf(-0.5F, static_cast<std::int32_t>(0.5F))}));
}

static_assert(lin::internal::lerp(0.0F, 1.0F, 0.5F) == 0.5F);
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, 2.0F, 0.5F) == lin::internal::matrix<float, 1, 2>{1.0F, 1.5F}));
static_assert(lin::internal::all(lin::internal::lerp(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 1.0F}));
static_assert(lin::internal::all(lin::internal::lerp(0.0F, 1.0F, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 1.5F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, 2.0F, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, 2.0F, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.5F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.5F}));
static_assert(lin::internal::all(lin::internal::lerp(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.5F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 2, 1>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}) == lin::internal::matrix<float, 1, 2>{0.25F, 1.75F}));
static_assert(lin::internal::all(lin::internal::lerp(lin::internal::matrix<float, 2, 2>{0.0F, 1.0F, 0.0F, 1.0F}, lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{0.25F, 0.75F, 0.25F, 0.75F}) == lin::internal::matrix<float, 2, 2>{0.25F, 1.75F, 0.25F, 1.75F}));

static_assert(lin::internal::max(0.5F, -0.5F) == 0.5F);
static_assert(lin::internal::all(lin::internal::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::max(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::max(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::max(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));

TEST(stellarlib_lin_intrinsics, pow)
{
	ASSERT_EQ(lin::internal::pow(0.5F, -0.5F), std::powf(0.5F, -0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, 0.5F), std::powf(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::pow(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::powf(-0.5F, 0.5F), std::powf(-0.5F, -0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::pow(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::pow(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F), std::powf(0.5F, -0.5F), std::powf(-0.5F, 0.5F)}));
}

TEST(stellarlib_lin_intrinsics, lit)
{
	ASSERT_TRUE(lin::internal::all(lin::internal::lit(0.5F, 0.5F, -0.5F) == lin::internal::matrix<float, 1, 4>{1.0F, 0.5F, std::powf(0.5F, -0.5F), 1.0F}));
	static_assert(lin::internal::all(lin::internal::lit(-0.5F, -0.5F, 0.5F) == lin::internal::matrix<float, 1, 4>{1.0F, 0.0F, 0.0F, 1.0F}));
}

TEST(stellarlib_lin_intrinsics, log)
{
	ASSERT_EQ(lin::internal::log(0.5F), std::logf(0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::log(lin::internal::matrix<float, 1, 2>{1.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::logf(1.5F), std::logf(0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::log(lin::internal::matrix<float, 2, 2>{1.5F, 0.5F, 1.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::logf(1.5F), std::logf(0.5F), std::logf(1.5F), std::logf(0.5F)}));
}

TEST(stellarlib_lin_intrinsics, log10)
{
	ASSERT_EQ(lin::internal::log10(0.5F), std::log10f(0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::log10(lin::internal::matrix<float, 1, 2>{1.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::log10f(1.5F), std::log10f(0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::log10(lin::internal::matrix<float, 2, 2>{1.5F, 0.5F, 1.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::log10f(1.5F), std::log10f(0.5F), std::log10f(1.5F), std::log10f(0.5F)}));
}

TEST(stellarlib_lin_intrinsics, log2)
{
	ASSERT_EQ(lin::internal::log2(0.5F), std::log2f(0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::log2(lin::internal::matrix<float, 1, 2>{1.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{std::log2f(1.5F), std::log2f(0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::log2(lin::internal::matrix<float, 2, 2>{1.5F, 0.5F, 1.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{std::log2f(1.5F), std::log2f(0.5F), std::log2f(1.5F), std::log2f(0.5F)}));
}

static_assert(lin::internal::mad(0.5F, -0.5F, 0.5F) == 0.25F);
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.25F, 0.75F}));
static_assert(lin::internal::all(lin::internal::mad(0.5F, -0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.75F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, -0.5F) == lin::internal::matrix<float, 1, 2>{-0.75F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.25F, 0.25F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.25F, 0.25F}));
static_assert(lin::internal::all(lin::internal::mad(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.75F, 0.75F}));
static_assert(lin::internal::all(lin::internal::mad(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.75F, 0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.25F, -0.75F}));
static_assert(lin::internal::all(lin::internal::mad(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}, lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.25F, -0.75F, 0.25F, -0.75F}));

static_assert(lin::internal::min(0.5F, -0.5F) == -0.5F);
static_assert(lin::internal::all(lin::internal::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::min(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::min(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::min(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{-0.5F, -0.5F, -0.5F, -0.5F}));

TEST(stellarlib_lin_intrinsics, modf)
{
	float ip1{};
	ASSERT_EQ(lin::internal::modf(1.5F, ip1), 0.5F);
	ASSERT_EQ(ip1, 1.0F);
	lin::internal::matrix<float, 1, 2> ip2{};
	ASSERT_TRUE(lin::internal::all(lin::internal::modf(1.5F, ip2) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
	ASSERT_TRUE(lin::internal::all(ip2 == lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::modf(lin::internal::matrix<float, 2, 1>{1.5F, -1.5F}, ip2) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(ip2 == lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::modf(lin::internal::matrix<float, 1, 2>{1.5F, -1.5F}, ip2) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(ip2 == lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}));
	lin::internal::matrix<float, 2, 2> ip2x2{};
	ASSERT_TRUE(lin::internal::all(lin::internal::modf(lin::internal::matrix<float, 2, 2>{1.5F, -1.5F, 1.5F, -1.5F}, ip2x2) == lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(ip2x2 == lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}));
}

static_assert(lin::internal::mul(0.5F, -0.5F) == -0.25F);
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.25F, -0.25F}));
static_assert(lin::internal::all(lin::internal::mul(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{-0.25F, 0.25F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 1>{-0.5F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 1>{-0.5F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 4, 3>{0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 3, 2>{-0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 4, 2>{-0.25F, 0.25F, 0.25F, -0.25F, -0.25F, 0.25F, 0.25F, -0.25F}));
static_assert(lin::internal::all(lin::internal::mul(lin::internal::matrix<float, 2, 3>{0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 3, 4>{-0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 4>{-0.25F, 0.25F, -0.25F, 0.25F, 0.25F, -0.25F, 0.25F, -0.25F}));

TEST(stellarlib_lin_intrinsics, normalize)
{
	static_assert(lin::internal::normalize(0.5F) == 1.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::normalize(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sqrtf(0.5F), -std::sqrtf(0.5F)}));
}

static_assert(lin::internal::radians(180.0F) == std::numbers::pi_v<float>);
static_assert(lin::internal::all(lin::internal::radians(lin::internal::matrix<float, 1, 2>{180.0F, 360.0F}) == lin::internal::matrix<float, 1, 2>{std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>}));
static_assert(lin::internal::all(lin::internal::radians(lin::internal::matrix<float, 2, 2>{180.0F, 360.0F, 180.0F, 360.0F}) == lin::internal::matrix<float, 2, 2>{std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>, std::numbers::pi_v<float>, 2.0F * std::numbers::pi_v<float>}));

static_assert(lin::internal::rcp(0.5F) == 2.0F);
static_assert(lin::internal::all(lin::internal::rcp(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{2.0F, -2.0F}));
static_assert(lin::internal::all(lin::internal::rcp(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{2.0F, -2.0F, 2.0F, -2.0F}));

static_assert(lin::internal::reflect(0.5F, -0.5F) == 0.25F);
static_assert(lin::internal::all(lin::internal::reflect(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::reflect(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
static_assert(lin::internal::all(lin::internal::reflect(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::reflect(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));

TEST(stellarlib_lin_intrinsics, refract)
{
	ASSERT_EQ(lin::internal::refract(0.5F, -0.5F, 1.0F), 0.5F);
	ASSERT_TRUE(lin::internal::all(lin::internal::refract(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F, 1.0F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::refract(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 1.0F) == lin::internal::matrix<float, 1, 2>{-0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::refract(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}, 1.0F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::refract(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}, 1.0F) == lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}));
}

TEST(stellarlib_lin_intrinsics, round)
{
	ASSERT_EQ(lin::internal::round(0.0F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::round(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::round(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}));
}

TEST(stellarlib_lin_intrinsics, rsqrt)
{
	ASSERT_EQ(lin::internal::rsqrt(0.5F), 1.0F / std::sqrtf(0.5F));
	ASSERT_TRUE(lin::internal::all(lin::internal::rsqrt(lin::internal::matrix<float, 1, 2>{1.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F / std::sqrtf(1.5F), 1.0F / std::sqrtf(0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::rsqrt(lin::internal::matrix<float, 2, 2>{1.5F, 0.5F, 1.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F / std::sqrtf(1.5F), 1.0F / std::sqrtf(0.5F), 1.0F / std::sqrtf(1.5F), 1.0F / std::sqrtf(0.5F)}));
}

static_assert(lin::internal::saturate(0.0F) == 0.0F);
static_assert(lin::internal::all(lin::internal::saturate(lin::internal::matrix<float, 1, 2>{1.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(lin::internal::all(lin::internal::saturate(lin::internal::matrix<float, 2, 2>{1.5F, -0.5F, 1.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F, 0.0F, 1.0F, 0.0F}));

static_assert(lin::internal::sign(0.0F) == 0.0F);
static_assert(lin::internal::all(lin::internal::sign(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}));
static_assert(lin::internal::all(lin::internal::sign(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}));

TEST(stellarlib_lin_intrinsics, sin)
{
	ASSERT_EQ(lin::internal::sin(0.0F), std::sinf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::sin(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::sin(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::sinf(0.5F), std::sinf(-0.5F), std::sinf(0.5F), std::sinf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, sincos)
{
	float s1{};
	float c1{};
	lin::internal::sincos(0.0F, s1, c1);
	ASSERT_EQ(s1, std::sinf(0.0F));
	ASSERT_EQ(c1, std::cosf(0.0F));
	lin::internal::matrix<float, 1, 2> s2{};
	lin::internal::matrix<float, 1, 2> c2{};
	lin::internal::sincos(0.0F, s2, c1);
	ASSERT_TRUE(lin::internal::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.0F), std::sinf(0.0F)}));
	ASSERT_EQ(c1, std::cosf(0.0F));
	lin::internal::sincos(0.0F, s1, c2);
	ASSERT_EQ(s1, std::sinf(0.0F));
	ASSERT_TRUE(lin::internal::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.0F), std::cosf(0.0F)}));
	lin::internal::sincos(0.0F, s2, c2);
	ASSERT_TRUE(lin::internal::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.0F), std::sinf(0.0F)}));
	ASSERT_TRUE(lin::internal::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.0F), std::cosf(0.0F)}));
	lin::internal::sincos(lin::internal::matrix<float, 2, 1>{0.5F, -0.5F}, s2, c2);
	ASSERT_TRUE(lin::internal::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.5F), std::cosf(-0.5F)}));
	lin::internal::sincos(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, s2, c2);
	ASSERT_TRUE(lin::internal::all(s2 == lin::internal::matrix<float, 1, 2>{std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(c2 == lin::internal::matrix<float, 1, 2>{std::cosf(0.5F), std::cosf(-0.5F)}));
	lin::internal::matrix<float, 2, 2> s2x2{};
	lin::internal::matrix<float, 2, 2> c2x2{};
	lin::internal::sincos(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, s2x2, c2x2);
	ASSERT_TRUE(lin::internal::all(s2x2 == lin::internal::matrix<float, 2, 2>{std::sinf(0.5F), std::sinf(-0.5F), std::sinf(0.5F), std::sinf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(c2x2 == lin::internal::matrix<float, 2, 2>{std::cosf(0.5F), std::cosf(-0.5F), std::cosf(0.5F), std::cosf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, sinh)
{
	ASSERT_EQ(lin::internal::sinh(0.0F), std::sinhf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::sinh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::sinhf(0.5F), std::sinhf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::sinh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::sinhf(0.5F), std::sinhf(-0.5F), std::sinhf(0.5F), std::sinhf(-0.5F)}));
}

static_assert(lin::internal::smoothstep(0.0F, 1.0F, 0.5F) == 0.5F);
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(0.0F, 1.0F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, 0.5F) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, 1.0F, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(0.0F, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 2, 1>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 2, 1>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 2, 1>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}, lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}, lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.5F, 0.5F}));
static_assert(lin::internal::all(lin::internal::smoothstep(lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}, lin::internal::matrix<float, 2, 2>{1.0F, 1.0F, 1.0F, 1.0F}, lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.5F, 0.5F, 0.5F, 0.5F}));

static_assert(lin::internal::step(0.5F, -0.5F) == 0.0F);
static_assert(lin::internal::all(lin::internal::step(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, 0.5F) == lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));
static_assert(lin::internal::all(lin::internal::step(-0.5F, lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));
static_assert(lin::internal::all(lin::internal::step(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 2, 1>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}));
static_assert(lin::internal::all(lin::internal::step(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}, lin::internal::matrix<float, 1, 2>{-0.5F, 0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 1.0F}));
static_assert(lin::internal::all(lin::internal::step(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}, lin::internal::matrix<float, 2, 2>{-0.5F, 0.5F, -0.5F, 0.5F}) == lin::internal::matrix<float, 2, 2>{0.0F, 1.0F, 0.0F, 1.0F}));

TEST(stellarlib_lin_intrinsics, tan)
{
	ASSERT_EQ(lin::internal::tan(0.0F), std::tanf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::tan(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::tanf(0.5F), std::tanf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::tan(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::tanf(0.5F), std::tanf(-0.5F), std::tanf(0.5F), std::tanf(-0.5F)}));
}

TEST(stellarlib_lin_intrinsics, tanh)
{
	ASSERT_EQ(lin::internal::tanh(0.0F), std::tanhf(0.0F));
	ASSERT_TRUE(lin::internal::all(lin::internal::tanh(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{std::tanhf(0.5F), std::tanhf(-0.5F)}));
	ASSERT_TRUE(lin::internal::all(lin::internal::tanh(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{std::tanhf(0.5F), std::tanhf(-0.5F), std::tanhf(0.5F), std::tanhf(-0.5F)}));
}

static_assert(lin::internal::transpose(0.0F) == 0.0F);
static_assert(lin::internal::all(lin::internal::transpose(lin::internal::matrix<float, 1, 3>{1.0F, 2.0F, 3.0F}) == lin::internal::matrix<float, 3, 1>{1.0F, 2.0F, 3.0F}));
static_assert(lin::internal::all(lin::internal::transpose(lin::internal::matrix<float, 2, 3>{1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 3, 2>{1.0F, 4.0F, 2.0F, 5.0F, 3.0F, 6.0F}));

TEST(stellarlib_lin_intrinsics, trunc)
{
	ASSERT_EQ(lin::internal::trunc(0.0F), 0.0F);
	ASSERT_TRUE(lin::internal::all(lin::internal::trunc(lin::internal::matrix<float, 1, 2>{0.5F, -0.5F}) == lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
	ASSERT_TRUE(lin::internal::all(lin::internal::trunc(lin::internal::matrix<float, 2, 2>{0.5F, -0.5F, 0.5F, -0.5F}) == lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}));
}

#pragma clang diagnostic pop
