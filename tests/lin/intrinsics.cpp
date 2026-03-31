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

#include <cstdint>
#include <cmath>
#include <numbers>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::abs(1.0F) == lin::abs(-1.0F));
static_assert(lin::abs(1) == lin::abs(-1));
static_assert(lin::all(lin::abs(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));
static_assert(lin::all(lin::abs(lin::internal::matrix<std::int32_t, 1, 2>{1, -1}) == lin::internal::matrix<std::int32_t, 1, 2>{1, 1}));
static_assert(lin::all(lin::abs(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{1.0F, 1.0F, 1.0F, 1.0F}));
static_assert(lin::all(lin::abs(lin::internal::matrix<std::int32_t, 2, 2>{1, -1, 1, -1}) == lin::internal::matrix<std::int32_t, 2, 2>{1, 1, 1, 1}));

TEST(stellarlib_lin_intrinsics, acos)
{
	ASSERT_EQ(lin::acos(0.0F), std::acosf(0.0F));
	ASSERT_TRUE(lin::all(lin::acos(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::acosf(1.0F), std::acosf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::acos(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::acosf(1.0F), std::acosf(-1.0F), std::acosf(1.0F), std::acosf(-1.0F)}));
}

static_assert(lin::all(1.0F));
static_assert(lin::all(1));
static_assert(lin::all(true));
static_assert(lin::all(lin::internal::matrix<float, 1, 2>{1.0F, 1.0F}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 1}));
static_assert(lin::all(lin::internal::matrix<bool, 1, 2>{true, true}));
static_assert(lin::all(lin::internal::matrix<float, 2, 2>{1.0F, 1.0F, 1.0F, 1.0F}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 2, 2>{1, 1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<bool, 2, 2>{true, true, true, true}));
static_assert(!lin::all(0.0F));
static_assert(!lin::all(0));
static_assert(!lin::all(false));
static_assert(!lin::all(lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(!lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 0}));
static_assert(!lin::all(lin::internal::matrix<bool, 1, 2>{true, false}));
static_assert(!lin::all(lin::internal::matrix<float, 2, 2>{1.0F, 0.0F, 1.0F, 0.0F}));
static_assert(!lin::all(lin::internal::matrix<std::int32_t, 2, 2>{1, 0, 1, 0}));
static_assert(!lin::all(lin::internal::matrix<bool, 2, 2>{true, false, true, false}));

static_assert(lin::any(1.0F));
static_assert(lin::any(1));
static_assert(lin::any(true));
static_assert(lin::any(lin::internal::matrix<float, 1, 2>{1.0F, 0.0F}));
static_assert(lin::any(lin::internal::matrix<std::int32_t, 1, 2>{1, 0}));
static_assert(lin::any(lin::internal::matrix<bool, 1, 2>{true, false}));
static_assert(lin::any(lin::internal::matrix<float, 2, 2>{1.0F, 0.0F, 1.0F, 0.0F}));
static_assert(lin::any(lin::internal::matrix<std::int32_t, 2, 2>{1, 0, 1, 0}));
static_assert(lin::any(lin::internal::matrix<bool, 2, 2>{true, false, false, true}));
static_assert(!lin::any(0.0F));
static_assert(!lin::any(0));
static_assert(!lin::any(false));
static_assert(!lin::any(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F}));
static_assert(!lin::any(lin::internal::matrix<std::int32_t, 1, 2>{0, 0}));
static_assert(!lin::any(lin::internal::matrix<bool, 1, 2>{false, false}));
static_assert(!lin::any(lin::internal::matrix<float, 2, 2>{0.0F, 0.0F, 0.0F, 0.0F}));
static_assert(!lin::any(lin::internal::matrix<std::int32_t, 2, 2>{0, 0, 0, 0}));
static_assert(!lin::any(lin::internal::matrix<bool, 2, 2>{false, false, false, false}));

static_assert(lin::asdouble(0x51EB851F, 0x40091EB8) == 3.14);
static_assert(lin::all(lin::asdouble(lin::internal::matrix<std::uint32_t, 1, 2>{0x51EB851F, 0x51EB851F}, lin::internal::matrix<std::uint32_t, 1, 2>{0x40091EB8, 0x40191EB8}) == lin::internal::matrix<double, 1, 2>{3.14, 6.28}));
static_assert(lin::all(lin::asdouble(lin::internal::matrix<std::uint32_t, 1, 2>{0x51EB851F, 0x51EB851F}, lin::internal::matrix<std::uint32_t, 2, 1>{0x40091EB8, 0x40191EB8}) == lin::internal::matrix<double, 1, 2>{3.14, 6.28}));

static_assert(lin::asfloat(3.14F) == 3.14F);
static_assert(lin::asfloat<std::int32_t>(0x4048F5C3) == 3.14F);
static_assert(lin::asfloat<std::uint32_t>(0x4048F5C3) == 3.14F);
static_assert(lin::all(lin::asfloat(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}) == lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}));
static_assert(lin::all(lin::asfloat(lin::internal::matrix<std::int32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}));
static_assert(lin::all(lin::asfloat(lin::internal::matrix<std::uint32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}));
static_assert(lin::all(lin::asfloat(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}) == lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}));
static_assert(lin::all(lin::asfloat(lin::internal::matrix<std::int32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}));
static_assert(lin::all(lin::asfloat(lin::internal::matrix<std::uint32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}));

TEST(stellarlib_lin_intrinsics, asin)
{
	ASSERT_EQ(lin::asin(0.0F), std::asinf(0.0F));
	ASSERT_TRUE(lin::all(lin::asin(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::asinf(1.0F), std::asinf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::asin(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::asinf(1.0F), std::asinf(-1.0F), std::asinf(1.0F), std::asinf(-1.0F)}));
}

static_assert(lin::asint(3.14F) == 0x4048F5C3);
static_assert(lin::asint<std::uint32_t>(0x4048F5C3) == 0x4048F5C3);
static_assert(lin::all(lin::asint(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}) == lin::internal::matrix<std::int32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asint(lin::internal::matrix<std::uint32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<std::int32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asint(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}) == lin::internal::matrix<std::int32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asint(lin::internal::matrix<std::uint32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<std::int32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}));

TEST(stellarlib_lin_intrinsics, asuint)
{
	std::uint32_t lowbits{};
	std::uint32_t highbits{};
	lin::asuint(3.14, lowbits, highbits);
	ASSERT_EQ(lowbits, 0x51EB851F);
	ASSERT_EQ(highbits, 0x40091EB8);
}

static_assert(lin::asuint(3.14F) == 0x4048F5C3);
static_assert(lin::asuint<std::int32_t>(0x4048F5C3) == 0x4048F5C3);
static_assert(lin::all(lin::asuint(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}) == lin::internal::matrix<std::uint32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asuint(lin::internal::matrix<std::int32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<std::uint32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asuint(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}) == lin::internal::matrix<std::uint32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}));
static_assert(lin::all(lin::asuint(lin::internal::matrix<std::int32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<std::uint32_t, 2, 2>{0x4048F5C3, 0x40C8F5C3, 0x4048F5C3, 0x40C8F5C3}));

TEST(stellarlib_lin_intrinsics, atan)
{
	ASSERT_EQ(lin::atan(0.0F), std::atanf(0.0F));
	ASSERT_TRUE(lin::all(lin::atan(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::atanf(1.0F), std::atanf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::atan(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::atanf(1.0F), std::atanf(-1.0F), std::atanf(1.0F), std::atanf(-1.0F)}));
}

TEST(stellarlib_lin_intrinsics, atan2)
{
	ASSERT_EQ(lin::atan2(1.0F, -1.0F), std::atan2f(1.0F, -1.0F));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 1, 2>{-1.0F, 1.0F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(1.0F, -1.0F), std::atan2f(-1.0F, 1.0F)}));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}, lin::internal::matrix<float, 2, 1>{-1.0F, 1.0F}) == lin::internal::matrix<float, 1, 2>{std::atan2f(1.0F, -1.0F), std::atan2f(-1.0F, 1.0F)}));
	ASSERT_TRUE(lin::all(lin::atan2(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}, lin::internal::matrix<float, 2, 2>{-1.0F, 1.0F, -1.0F, 1.0F}) == lin::internal::matrix<float, 2, 2>{std::atan2f(1.0F, -1.0F), std::atan2f(-1.0F, 1.0F), std::atan2f(1.0F, -1.0F), std::atan2f(-1.0F, 1.0F)}));
}

TEST(stellarlib_lin_intrinsics, ceil)
{
	ASSERT_EQ(lin::ceil(3.14F), std::ceilf(3.14F));
	ASSERT_TRUE(lin::all(lin::ceil(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}) == lin::internal::matrix<float, 1, 2>{std::ceilf(3.14F), std::ceilf(6.28F)}));
	ASSERT_TRUE(lin::all(lin::ceil(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}) == lin::internal::matrix<float, 2, 2>{std::ceilf(3.14F), std::ceilf(6.28F), std::ceilf(3.14F), std::ceilf(6.28F)}));
}

static_assert(lin::clamp(-2.0F, -1.0F, 1.0F) == -1.0F);
static_assert(lin::clamp(0.0F, -1.0F, 1.0F) == 0.0F);
static_assert(lin::clamp(2.0F, -1.0F, 1.0F) == 1.0F);
static_assert(lin::clamp(-2, -1, 1) == -1);
static_assert(lin::clamp(0, -1, 1) == 0);
static_assert(lin::clamp(2, -1, 1) == 1);
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 3, 1>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 3, 1>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 1, 3>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 1, 3>{-2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 1, 3>{-1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 3, 1>{1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<std::int32_t, 1, 3>{-2, 0, 2}, lin::internal::matrix<std::int32_t, 1, 3>{-1, -1, -1}, lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 1}) == lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<std::int32_t, 3, 1>{-2, 0, 2}, lin::internal::matrix<std::int32_t, 1, 3>{-1, -1, -1}, lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 1}) == lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<std::int32_t, 1, 3>{-2, 0, 2}, lin::internal::matrix<std::int32_t, 3, 1>{-1, -1, -1}, lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 1}) == lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<std::int32_t, 1, 3>{-2, 0, 2}, lin::internal::matrix<std::int32_t, 1, 3>{-1, -1, -1}, lin::internal::matrix<std::int32_t, 3, 1>{1, 1, 1}) == lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<float, 3, 3>{-2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F, -2.0F, 0.0F, 2.0F}, lin::internal::matrix<float, 3, 3>{-1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F, -1.0F}, lin::internal::matrix<float, 3, 3>{1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F}) == lin::internal::matrix<float, 3, 3>{-1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::clamp(lin::internal::matrix<std::int32_t, 3, 3>{-2, 0, 2, -2, 0, 2, -2, 0, 2}, lin::internal::matrix<std::int32_t, 3, 3>{-1, -1, -1, -1, -1, -1, -1, -1, -1}, lin::internal::matrix<std::int32_t, 3, 3>{1, 1, 1, 1, 1, 1, 1, 1, 1}) == lin::internal::matrix<std::int32_t, 3, 3>{-1, 0, 1, -1, 0, 1, -1, 0, 1}));

TEST(stellarlib_lin_intrinsics, cos)
{
	ASSERT_EQ(lin::cos(0.0F), std::cosf(0.0F));
	ASSERT_TRUE(lin::all(lin::cos(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::cosf(1.0F), std::cosf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::cos(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::cosf(1.0F), std::cosf(-1.0F), std::cosf(1.0F), std::cosf(-1.0F)}));
}

TEST(stellarlib_lin_intrinsics, cosh)
{
	ASSERT_EQ(lin::cosh(0.0F), std::coshf(0.0F));
	ASSERT_TRUE(lin::all(lin::cosh(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::coshf(1.0F), std::coshf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::cosh(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::coshf(1.0F), std::coshf(-1.0F), std::coshf(1.0F), std::coshf(-1.0F)}));
}

static_assert(lin::countbits(0x4048F5C3) == 13);
static_assert(lin::all(lin::countbits(lin::internal::matrix<std::uint32_t, 1, 2>{0x4048F5C3, 0x40C8F5C3}) == lin::internal::matrix<std::uint32_t, 1, 2>{13, 14}));

static_assert(lin::all(lin::cross(lin::internal::matrix<float, 1, 3>{1, 2, 3}, lin::internal::matrix<float, 1, 3>{4, 5, 6}) == lin::internal::matrix<float, 1, 3>{-3, 6, -3}));
static_assert(lin::all(lin::cross(lin::internal::matrix<float, 1, 3>{1, 2, 3}, lin::internal::matrix<float, 3, 1>{4, 5, 6}) == lin::internal::matrix<float, 1, 3>{-3, 6, -3}));

static_assert(lin::degrees(std::numbers::pi_v<float>) == 180.0F);
static_assert(lin::all(lin::degrees(lin::internal::matrix<float, 1, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}) == lin::internal::matrix<float, 1, 2>{180.0F, 360.0F}));
static_assert(lin::all(lin::degrees(lin::internal::matrix<float, 2, 2>{std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2, std::numbers::pi_v<float>, std::numbers::pi_v<float> * 2}) == lin::internal::matrix<float, 2, 2>{180.0F, 360.0F, 180.0F, 360.0F}));

TEST(stellarlib_lin_intrinsics, length)
{
	ASSERT_EQ(lin::length(lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}), 5.0F);
}

TEST(stellarlib_lin_intrinsics, distance)
{
	ASSERT_EQ(lin::distance(lin::internal::matrix<float, 1, 2>{-1.5F, -2.0F}, lin::internal::matrix<float, 1, 2>{1.5F, 2.0F}), 5.0F);
	ASSERT_EQ(lin::distance(lin::internal::matrix<float, 1, 2>{-1.5F, -2.0F}, lin::internal::matrix<float, 2, 1>{1.5F, 2.0F}), 5.0F);
}

static_assert(lin::dot(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}) == 11.0F);
static_assert(lin::dot(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}) == 11.0F);
static_assert(lin::dot(lin::internal::matrix<std::int32_t, 1, 2>{1, 2}, lin::internal::matrix<std::int32_t, 1, 2>{3, 4}) == 11);
static_assert(lin::dot(lin::internal::matrix<std::int32_t, 1, 2>{1, 2}, lin::internal::matrix<std::int32_t, 2, 1>{3, 4}) == 11);

TEST(stellarlib_lin_intrinsics, exp)
{
	ASSERT_EQ(lin::exp(0.0F), std::expf(0.0F));
	ASSERT_TRUE(lin::all(lin::exp(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::expf(1.0F), std::expf(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::exp(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::expf(1.0F), std::expf(-1.0F), std::expf(1.0F), std::expf(-1.0F)}));
}

TEST(stellarlib_lin_intrinsics, exp2)
{
	ASSERT_EQ(lin::exp2(0.0F), std::exp2f(0.0F));
	ASSERT_TRUE(lin::all(lin::exp2(lin::internal::matrix<float, 1, 2>{1.0F, -1.0F}) == lin::internal::matrix<float, 1, 2>{std::exp2f(1.0F), std::exp2f(-1.0F)}));
	ASSERT_TRUE(lin::all(lin::exp2(lin::internal::matrix<float, 2, 2>{1.0F, -1.0F, 1.0F, -1.0F}) == lin::internal::matrix<float, 2, 2>{std::exp2f(1.0F), std::exp2f(-1.0F), std::exp2f(1.0F), std::exp2f(-1.0F)}));
}

static_assert(lin::sign(-5.0F) == -1.0F);
static_assert(lin::sign(0.0F) == 0.0F);
static_assert(lin::sign(5.0F) == 1.0F);
static_assert(lin::sign(-5) == -1);
static_assert(lin::sign(0) == 0);
static_assert(lin::sign(5) == 1);
static_assert(lin::all(lin::sign(lin::internal::matrix<float, 1, 3>{-5.0F, 0.0F, 5.0F}) == lin::internal::matrix<float, 1, 3>{-1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::sign(lin::internal::matrix<std::int32_t, 1, 3>{-5, 0, 5}) == lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1}));
static_assert(lin::all(lin::sign(lin::internal::matrix<float, 3, 3>{-5.0F, 0.0F, 5.0F, -5.0F, 0.0F, 5.0F, -5.0F, 0.0F, 5.0F}) == lin::internal::matrix<float, 3, 3>{-1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F, -1.0F, 0.0F, 1.0F}));
static_assert(lin::all(lin::sign(lin::internal::matrix<std::int32_t, 3, 3>{-5, 0, 5, -5, 0, 5, -5, 0, 5}) == lin::internal::matrix<std::int32_t, 3, 3>{-1, 0, 1, -1, 0, 1, -1, 0, 1}));

static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));
static_assert(lin::all(lin::faceforward(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 2, 1>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{-1.0F, -2.0F}));

TEST(stellarlib_lin_intrinsics, floor)
{
	ASSERT_EQ(lin::floor(3.14F), std::floorf(3.14F));
	ASSERT_TRUE(lin::all(lin::floor(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}) == lin::internal::matrix<float, 1, 2>{std::floorf(3.14F), std::floorf(6.28F)}));
	ASSERT_TRUE(lin::all(lin::floor(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}) == lin::internal::matrix<float, 2, 2>{std::floorf(3.14F), std::floorf(6.28F), std::floorf(3.14F), std::floorf(6.28F)}));
}

static_assert(lin::mad(1.0F, 2.0F, 3.0F) == 5.0F);
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 2, 1>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 2, 1>{3.0F, 4.0F}, lin::internal::matrix<float, 1, 2>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 1, 2>{1.0F, 2.0F}, lin::internal::matrix<float, 1, 2>{3.0F, 4.0F}, lin::internal::matrix<float, 2, 1>{5.0F, 6.0F}) == lin::internal::matrix<float, 1, 2>{8.0F, 14.0F}));
static_assert(lin::all(lin::mad(lin::internal::matrix<float, 2, 2>{1.0F, 2.0F, 1.0F, 2.0F}, lin::internal::matrix<float, 2, 2>{3.0F, 4.0F, 3.0F, 4.0F}, lin::internal::matrix<float, 2, 2>{5.0F, 6.0F, 5.0F, 6.0F}) == lin::internal::matrix<float, 2, 2>{8.0F, 14.0F, 8.0F, 14.0F}));

static_assert(lin::fma(1.0, 2.0, 3.0) == 5.0);
static_assert(lin::all(lin::fma(lin::internal::matrix<double, 1, 2>{1.0, 2.0}, lin::internal::matrix<double, 1, 2>{3.0, 4.0}, lin::internal::matrix<double, 1, 2>{5.0, 6.0}) == lin::internal::matrix<double, 1, 2>{8.0, 14.0}));
static_assert(lin::all(lin::fma(lin::internal::matrix<double, 2, 1>{1.0, 2.0}, lin::internal::matrix<double, 1, 2>{3.0, 4.0}, lin::internal::matrix<double, 1, 2>{5.0, 6.0}) == lin::internal::matrix<double, 1, 2>{8.0, 14.0}));
static_assert(lin::all(lin::fma(lin::internal::matrix<double, 1, 2>{1.0, 2.0}, lin::internal::matrix<double, 2, 1>{3.0, 4.0}, lin::internal::matrix<double, 1, 2>{5.0, 6.0}) == lin::internal::matrix<double, 1, 2>{8.0, 14.0}));
static_assert(lin::all(lin::fma(lin::internal::matrix<double, 1, 2>{1.0, 2.0}, lin::internal::matrix<double, 1, 2>{3.0, 4.0}, lin::internal::matrix<double, 2, 1>{5.0, 6.0}) == lin::internal::matrix<double, 1, 2>{8.0, 14.0}));
static_assert(lin::all(lin::fma(lin::internal::matrix<double, 2, 2>{1.0, 2.0, 1.0, 2.0}, lin::internal::matrix<double, 2, 2>{3.0, 4.0, 3.0, 4.0}, lin::internal::matrix<double, 2, 2>{5.0, 6.0, 5.0, 6.0}) == lin::internal::matrix<double, 2, 2>{8.0, 14.0, 8.0, 14.0}));

TEST(stellarlib_lin_intrinsics, fmod)
{
	ASSERT_EQ(lin::fmod(3.14F, 1.5F), std::fmodf(3.14F, 1.5F));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, lin::internal::matrix<float, 1, 2>{1.5F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 1, 2>{3.14F, 6.28F}, lin::internal::matrix<float, 2, 1>{1.5F, 3.0F}) == lin::internal::matrix<float, 1, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
	ASSERT_TRUE(lin::all(lin::fmod(lin::internal::matrix<float, 2, 2>{3.14F, 6.28F, 3.14F, 6.28F}, lin::internal::matrix<float, 2, 2>{1.5F, 3.0F, 1.5F, 3.0F}) == lin::internal::matrix<float, 2, 2>{std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F), std::fmodf(3.14F, 1.5F), std::fmodf(6.28F, 3.0F)}));
}

#pragma clang diagnostic pop
