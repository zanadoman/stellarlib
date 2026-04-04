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

#include <stellarlib/ext/type_traits.hpp>
#include <stellarlib/lin/intrinsics.hpp>

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <format>
#include <memory>
#include <sstream>
#include <type_traits>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(std::is_standard_layout_v<lin::internal::matrix<float, 2, 2>>);
static_assert(sizeof(lin::internal::matrix<float, 2, 2>) == 4 * sizeof(float));
static_assert(ext::is_trivially_relocatable_v<lin::internal::matrix<float, 2, 2>>);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{} == 0);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{5} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{std::array<std::int16_t, 1>{5}} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{std::array<std::int8_t, 1>{5}} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{lin::internal::matrix<std::int8_t, 1, 1>{5}} == 5);
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{}) == std::array<std::int16_t, 4>{});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{1, 2, 3, 4}) == std::array<std::int16_t, 4>{1, 2, 3, 4});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{std::array<std::int16_t, 4>{1, 2, 3, 4}}) == std::array<std::int16_t, 4>{1, 2, 3, 4});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{std::array<std::int8_t, 4>{1, 2, 3, 4}}) == std::array<std::int16_t, 4>{1, 2, 3, 4});
static_assert(static_cast<std::array<std::int16_t, 2>>(lin::internal::matrix<std::int16_t, 1, 2>{lin::internal::matrix<std::int16_t, 2, 1>{1, 2}}) == std::array<std::int16_t, 2>{1, 2});
static_assert(static_cast<std::array<std::int16_t, 2>>(lin::internal::matrix<std::int16_t, 1, 2>{lin::internal::matrix<std::int8_t, 2, 1>{1, 2}}) == std::array<std::int16_t, 2>{1, 2});

TEST(stellarlib_lin_matrix, should_handle_assignment)
{
	lin::internal::matrix<std::int16_t, 2, 2> matrix1{};
	matrix1 = std::array<std::int16_t, 4>{1, 2, 3, 4};
	ASSERT_EQ((static_cast<const std::array<std::int16_t, 4> &>(matrix1)), (std::array<std::int16_t, 4>{1, 2, 3, 4}));
	matrix1 = std::array<std::int8_t, 4>{4, 3, 2, 1};
	ASSERT_EQ((static_cast<const std::array<std::int16_t, 4> &>(matrix1)), (std::array<std::int16_t, 4>{4, 3, 2, 1}));
	lin::internal::matrix<std::int16_t, 1, 2> matrix2{};
	matrix2 = lin::internal::matrix<std::int16_t, 2, 1>{1, 2};
	ASSERT_EQ((static_cast<const std::array<std::int16_t, 2> &>(matrix2)), (std::array<std::int16_t, 2>{1, 2}));
	matrix2 = lin::internal::matrix<std::int8_t, 2, 1>{2, 1};
	ASSERT_EQ((static_cast<const std::array<std::int16_t, 2> &>(matrix2)), (std::array<std::int16_t, 2>{2, 1}));
}

static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.x() == 1);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.r() == 1);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.y() == 2);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.g() == 2);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.z() == 3);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.b() == 3);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.w() == 4);
static_assert(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.a() == 4);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[0, 0] == 1);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[0, 1] == 2);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[1, 0] == 3);
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[1, 1] == 4);
static_assert(lin::all(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[0] == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}[1] == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));

TEST(stellarlib_lin_matrix, should_provide_accessors)
{
	lin::internal::matrix<std::int32_t, 1, 4> matrix1{};
	ASSERT_EQ(std::addressof(matrix1.x()), std::addressof(matrix1.r()));
	matrix1.x() = 1;
	ASSERT_EQ(std::addressof(matrix1.y()), std::addressof(matrix1.g()));
	matrix1.y() = 2;
	ASSERT_EQ(std::addressof(matrix1.z()), std::addressof(matrix1.b()));
	matrix1.z() = 3;
	ASSERT_EQ(std::addressof(matrix1.w()), std::addressof(matrix1.a()));
	matrix1.w() = 4;
	ASSERT_TRUE(lin::all(matrix1 == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}));
	lin::internal::matrix<std::int32_t, 2, 2> matrix2{};
	matrix2[0, 0] = 1;
	matrix2[0, 1] = 2;
	matrix2[1, 0] = 3;
	matrix2[1, 1] = 4;
	ASSERT_TRUE(lin::all(matrix2 == lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xx() == lin::internal::matrix<std::int32_t, 1, 2>{1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xy() == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xz() == lin::internal::matrix<std::int32_t, 1, 2>{1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xw() == lin::internal::matrix<std::int32_t, 1, 2>{1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yx() == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yy() == lin::internal::matrix<std::int32_t, 1, 2>{2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yz() == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yw() == lin::internal::matrix<std::int32_t, 1, 2>{2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zx() == lin::internal::matrix<std::int32_t, 1, 2>{3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zy() == lin::internal::matrix<std::int32_t, 1, 2>{3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zz() == lin::internal::matrix<std::int32_t, 1, 2>{3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zw() == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wx() == lin::internal::matrix<std::int32_t, 1, 2>{4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wy() == lin::internal::matrix<std::int32_t, 1, 2>{4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wz() == lin::internal::matrix<std::int32_t, 1, 2>{4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ww() == lin::internal::matrix<std::int32_t, 1, 2>{4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxx() == lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxy() == lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxz() == lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxw() == lin::internal::matrix<std::int32_t, 1, 3>{1, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyx() == lin::internal::matrix<std::int32_t, 1, 3>{1, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyy() == lin::internal::matrix<std::int32_t, 1, 3>{1, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyz() == lin::internal::matrix<std::int32_t, 1, 3>{1, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyw() == lin::internal::matrix<std::int32_t, 1, 3>{1, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzx() == lin::internal::matrix<std::int32_t, 1, 3>{1, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzy() == lin::internal::matrix<std::int32_t, 1, 3>{1, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzz() == lin::internal::matrix<std::int32_t, 1, 3>{1, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzw() == lin::internal::matrix<std::int32_t, 1, 3>{1, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwx() == lin::internal::matrix<std::int32_t, 1, 3>{1, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwy() == lin::internal::matrix<std::int32_t, 1, 3>{1, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwz() == lin::internal::matrix<std::int32_t, 1, 3>{1, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xww() == lin::internal::matrix<std::int32_t, 1, 3>{1, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxx() == lin::internal::matrix<std::int32_t, 1, 3>{2, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxy() == lin::internal::matrix<std::int32_t, 1, 3>{2, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxz() == lin::internal::matrix<std::int32_t, 1, 3>{2, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxw() == lin::internal::matrix<std::int32_t, 1, 3>{2, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyx() == lin::internal::matrix<std::int32_t, 1, 3>{2, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyy() == lin::internal::matrix<std::int32_t, 1, 3>{2, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyz() == lin::internal::matrix<std::int32_t, 1, 3>{2, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyw() == lin::internal::matrix<std::int32_t, 1, 3>{2, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzx() == lin::internal::matrix<std::int32_t, 1, 3>{2, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzy() == lin::internal::matrix<std::int32_t, 1, 3>{2, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzz() == lin::internal::matrix<std::int32_t, 1, 3>{2, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzw() == lin::internal::matrix<std::int32_t, 1, 3>{2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywx() == lin::internal::matrix<std::int32_t, 1, 3>{2, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywy() == lin::internal::matrix<std::int32_t, 1, 3>{2, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywz() == lin::internal::matrix<std::int32_t, 1, 3>{2, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yww() == lin::internal::matrix<std::int32_t, 1, 3>{2, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxx() == lin::internal::matrix<std::int32_t, 1, 3>{3, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxy() == lin::internal::matrix<std::int32_t, 1, 3>{3, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxz() == lin::internal::matrix<std::int32_t, 1, 3>{3, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxw() == lin::internal::matrix<std::int32_t, 1, 3>{3, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyx() == lin::internal::matrix<std::int32_t, 1, 3>{3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyy() == lin::internal::matrix<std::int32_t, 1, 3>{3, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyz() == lin::internal::matrix<std::int32_t, 1, 3>{3, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyw() == lin::internal::matrix<std::int32_t, 1, 3>{3, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzx() == lin::internal::matrix<std::int32_t, 1, 3>{3, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzy() == lin::internal::matrix<std::int32_t, 1, 3>{3, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzz() == lin::internal::matrix<std::int32_t, 1, 3>{3, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzw() == lin::internal::matrix<std::int32_t, 1, 3>{3, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwx() == lin::internal::matrix<std::int32_t, 1, 3>{3, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwy() == lin::internal::matrix<std::int32_t, 1, 3>{3, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwz() == lin::internal::matrix<std::int32_t, 1, 3>{3, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zww() == lin::internal::matrix<std::int32_t, 1, 3>{3, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxx() == lin::internal::matrix<std::int32_t, 1, 3>{4, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxy() == lin::internal::matrix<std::int32_t, 1, 3>{4, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxz() == lin::internal::matrix<std::int32_t, 1, 3>{4, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxw() == lin::internal::matrix<std::int32_t, 1, 3>{4, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyx() == lin::internal::matrix<std::int32_t, 1, 3>{4, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyy() == lin::internal::matrix<std::int32_t, 1, 3>{4, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyz() == lin::internal::matrix<std::int32_t, 1, 3>{4, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyw() == lin::internal::matrix<std::int32_t, 1, 3>{4, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzx() == lin::internal::matrix<std::int32_t, 1, 3>{4, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzy() == lin::internal::matrix<std::int32_t, 1, 3>{4, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzz() == lin::internal::matrix<std::int32_t, 1, 3>{4, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzw() == lin::internal::matrix<std::int32_t, 1, 3>{4, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwx() == lin::internal::matrix<std::int32_t, 1, 3>{4, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwy() == lin::internal::matrix<std::int32_t, 1, 3>{4, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwz() == lin::internal::matrix<std::int32_t, 1, 3>{4, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.www() == lin::internal::matrix<std::int32_t, 1, 3>{4, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxxx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxxy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxxz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxxw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxyx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxyy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxyz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxyw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxzx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxzy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxzz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxzw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxwx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxwy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxwz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xxww() == lin::internal::matrix<std::int32_t, 1, 4>{1, 1, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyxx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyxy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyxz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyxw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyyx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyyy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyyz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyyw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyzx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyzy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyzz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyzw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xywx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xywy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xywz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyww() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzxx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzxy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzxz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzxw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzyx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzyy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzyz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzyw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzzx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzzy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzzz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzzw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzwx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzwy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzwz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xzww() == lin::internal::matrix<std::int32_t, 1, 4>{1, 3, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwxx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwxy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwxz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwxw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwyx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwyy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwyz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwyw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwzx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwzy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwzz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwzw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwwx() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwwy() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwwz() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xwww() == lin::internal::matrix<std::int32_t, 1, 4>{1, 4, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxxx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxxy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxxz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxxw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxyx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxyy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxyz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxyw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxzx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxzy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxzz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxzw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxwx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxwy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxwz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yxww() == lin::internal::matrix<std::int32_t, 1, 4>{2, 1, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyxx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyxy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyxz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyxw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyyx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyyy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyyz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyyw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyzx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyzy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyzz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyzw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yywx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yywy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yywz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yyww() == lin::internal::matrix<std::int32_t, 1, 4>{2, 2, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzxx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzxy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzxz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzxw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzyx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzyy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzyz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzyw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzzx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzzy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzzz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzzw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzwx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzwy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzwz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.yzww() == lin::internal::matrix<std::int32_t, 1, 4>{2, 3, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywxx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywxy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywxz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywxw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywyx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywyy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywyz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywyw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywzx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywzy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywzz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywzw() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywwx() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywwy() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywwz() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.ywww() == lin::internal::matrix<std::int32_t, 1, 4>{2, 4, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxxx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxxy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxxz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxxw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxyx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxyy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxyz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxyw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxzx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxzy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxzz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxzw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxwx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxwy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxwz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zxww() == lin::internal::matrix<std::int32_t, 1, 4>{3, 1, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyxx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyxy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyxz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyxw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyyx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyyy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyyz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyyw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyzx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyzy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyzz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyzw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zywx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zywy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zywz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zyww() == lin::internal::matrix<std::int32_t, 1, 4>{3, 2, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzxx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzxy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzxz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzxw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzyx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzyy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzyz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzyw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzzx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzzy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzzz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzzw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzwx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzwy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzwz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zzww() == lin::internal::matrix<std::int32_t, 1, 4>{3, 3, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwxx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwxy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwxz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwxw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwyx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwyy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwyz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwyw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwzx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwzy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwzz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwzw() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwwx() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwwy() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwwz() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.zwww() == lin::internal::matrix<std::int32_t, 1, 4>{3, 4, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxxx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxxy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxxz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxxw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxyx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxyy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxyz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxyw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxzx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxzy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxzz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxzw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxwx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxwy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxwz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wxww() == lin::internal::matrix<std::int32_t, 1, 4>{4, 1, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyxx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyxy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyxz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyxw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyyx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyyy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyyz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyyw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyzx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyzy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyzz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyzw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wywx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wywy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wywz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wyww() == lin::internal::matrix<std::int32_t, 1, 4>{4, 2, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzxx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzxy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzxz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzxw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzyx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzyy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzyz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzyw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzzx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzzy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzzz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzzw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzwx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzwy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzwz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzww() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 4, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwxx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 1, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwxy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwxz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 1, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwxw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 1, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwyx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwyy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 2, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwyz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwyw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 2, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwzx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 3, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwzy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 3, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwzz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 3, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwzw() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwwx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 4, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwwy() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 4, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwwz() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 4, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wwww() == lin::internal::matrix<std::int32_t, 1, 4>{4, 4, 4, 4}));

TEST(stellarlib_lin_matrix, should_increment)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	ASSERT_TRUE(lin::all(++matrix == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix++ == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));
}

TEST(stellarlib_lin_matrix, should_decrement)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{3, 4};
	ASSERT_TRUE(lin::all(--matrix == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix-- == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(+lin::internal::matrix<std::int32_t, 1, 2>{1, 2} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(-lin::internal::matrix<std::int32_t, 1, 2>{1, 2} == lin::internal::matrix<std::int32_t, 1, 2>{-1, -2}));
static_assert(lin::all(!lin::internal::matrix<bool, 1, 2>{true, false} == lin::internal::matrix<bool, 1, 2>{false, true}));
static_assert(lin::all(~lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b0101'0101} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0101'0101, 0b1010'1010}));

TEST(stellarlib_lin_matrix, should_mul_assign)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	matrix *= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
	matrix *= lin::internal::matrix<std::int32_t, 2, 1>{7, 8};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{21, 48}));
	matrix *= lin::internal::matrix<std::int32_t, 1, 2>{49, 50};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1029, 2400}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * 3 == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
static_assert(lin::all(1 * lin::internal::matrix<std::int32_t, 1, 2>{2, 3} == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{3, 8}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{3, 8}));

TEST(stellarlib_lin_matrix, should_div_assign)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1029, 2400};
	matrix /= lin::internal::matrix<std::int32_t, 1, 2>{49, 50};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{21, 48}));
	matrix /= lin::internal::matrix<std::int32_t, 2, 1>{7, 8};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
	matrix /= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 6} / 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(6 / lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 8} / lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 8} / lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_rem_assign)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{83, 88};
	matrix %= lin::internal::matrix<std::int32_t, 1, 2>{29, 30};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{25, 28}));
	matrix %= lin::internal::matrix<std::int32_t, 2, 1>{9, 10};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{7, 8}));
	matrix %= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{7, 8} % 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(17 % lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{7, 10} % lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{7, 10} % lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_add_assign)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	matrix += 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
	matrix += lin::internal::matrix<std::int32_t, 2, 1>{6, 7};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{10, 12}));
	matrix += lin::internal::matrix<std::int32_t, 1, 2>{13, 14};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{23, 26}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + 3 == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
static_assert(lin::all(1 + lin::internal::matrix<std::int32_t, 1, 2>{2, 3} == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{4, 6}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{4, 6}));

TEST(stellarlib_lin_matrix, should_sub_assign)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{23, 26};
	matrix -= lin::internal::matrix<std::int32_t, 1, 2>{13, 14};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{10, 12}));
	matrix -= lin::internal::matrix<std::int32_t, 2, 1>{6, 7};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
	matrix -= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 5} - 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(5 - lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 6} - lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 6} - lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_shl_assign)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b0000'0001, 0b0000'0010};
	matrix <<= std::uint8_t{1};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0010, 0b0000'0100}));
	matrix <<= lin::internal::matrix<std::uint8_t, 2, 1>{2, 3};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));
	matrix <<= lin::internal::matrix<std::uint8_t, 1, 2>{3, 2};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0100'0000, 0b1000'0000}));
}

static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010} << std::uint8_t{3} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0001'0000}));
static_assert(lin::all(std::uint8_t{0b0000'0001} << lin::internal::matrix<std::uint8_t, 1, 2>{2, 3} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0100, 0b0000'1000}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010} << lin::internal::matrix<std::uint8_t, 2, 1>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010} << lin::internal::matrix<std::uint8_t, 1, 2>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));

TEST(stellarlib_lin_matrix, should_shr_assign)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b0100'0000, 0b1000'0000};
	matrix >>= lin::internal::matrix<std::uint8_t, 1, 2>{3, 2};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));
	matrix >>= lin::internal::matrix<std::uint8_t, 2, 1>{2, 3};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0010, 0b0000'0100}));
	matrix >>= std::uint8_t{1};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010}));
}

static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0001'0000} >> std::uint8_t{3} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010}));
static_assert(lin::all(std::uint8_t{0b0001'0000} >> lin::internal::matrix<std::uint8_t, 1, 2>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0010, 0b0000'0001}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000} >> lin::internal::matrix<std::uint8_t, 2, 1>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000} >> lin::internal::matrix<std::uint8_t, 1, 2>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} < 0) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((0 < lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} < lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} < lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} <= 0) == lin::internal::matrix<bool, 1, 3>{true, true, false}));
static_assert(lin::all((0 <= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} <= lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} <= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} > 0) == lin::internal::matrix<bool, 1, 3>{false, false, true}));
static_assert(lin::all((0 > lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} > lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} > lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, false, true}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} >= 0) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((0 >= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} >= lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} >= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} == 0) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((0 == lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} == lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} == lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} != 0) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((0 != lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} != lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} != lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));

TEST(stellarlib_lin_matrix, should_bit_and_assign)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix &= std::uint8_t{0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0101'0001}));
	matrix &= lin::internal::matrix<std::uint8_t, 2, 1>{0b1011'1100, 0b0011'1101};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1000, 0b0001'0001}));
	matrix &= lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1000, 0b0001'0001}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} & std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0000, 0b0101'0001}));
static_assert(lin::all((std::uint8_t{0b1010'1010} & lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0000'0000}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} & lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0101'0001}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} & lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0101'0001}));

TEST(stellarlib_lin_matrix, should_bit_xor_assign)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix ^= std::uint8_t{0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1000'1110}));
	matrix ^= lin::internal::matrix<std::uint8_t, 2, 1>{0b1011'1100, 0b0011'1101};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1100'1101, 0b1011'0011}));
	matrix ^= lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0001'0110, 0b0110'1000}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} ^ std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1000'1110}));
static_assert(lin::all((std::uint8_t{0b1010'1010} ^ lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1111'1111}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} ^ lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1000'1110}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} ^ lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1000'1110}));

TEST(stellarlib_lin_matrix, should_bit_or_assign)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix |= std::uint8_t{0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1101'1111}));
	matrix |= lin::internal::matrix<std::uint8_t, 2, 1>{0b1011'1100, 0b0011'1101};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1111'1111}));
	matrix |= lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b1101'1011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1111'1111}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} | std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1101'1111}));
static_assert(lin::all((std::uint8_t{0b1010'1010} | lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1111'1111}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} | lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1101'1111}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} | lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1101'1111}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{false, true, true} && true) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((true && lin::internal::matrix<std::int32_t, 1, 3>{false, true, true}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{true, true, false} && lin::internal::matrix<std::int32_t, 3, 1>{false, true, true}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{true, true, false} && lin::internal::matrix<std::int32_t, 1, 3>{false, true, true}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{true, false, false} || false) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((false || lin::internal::matrix<std::int32_t, 1, 3>{true, false, false}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{false, false, true} || lin::internal::matrix<std::int32_t, 3, 1>{true, false, false}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{false, false, true} || lin::internal::matrix<std::int32_t, 1, 3>{true, false, false}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));

TEST(stellarlib_lin_matrix, should_stream_and_format)
{
	std::ostringstream out1{};
	const lin::internal::matrix<std::int32_t, 1, 2> matrix1{1, 2};
	out1 << matrix1;
	ASSERT_EQ(out1.view(), "[1, 2]");
	ASSERT_EQ(std::format("{}", matrix1), out1.view());
	std::ostringstream out2{};
	const lin::internal::matrix<std::int32_t, 2, 2> matrix2{1, 2, 3, 4};
	out2 << matrix2;
	ASSERT_EQ(out2.view(), "[[1, 2], [3, 4]]");
	ASSERT_EQ(std::format("{}", matrix2), out2.view());
}

#pragma clang diagnostic pop
