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
#include <memory>
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
	ASSERT_EQ((static_cast<std::array<std::int16_t, 4>>(matrix1)), (std::array<std::int16_t, 4>{1, 2, 3, 4}));
	matrix1 = std::array<std::int8_t, 4>{4, 3, 2, 1};
	ASSERT_EQ((static_cast<std::array<std::int16_t, 4>>(matrix1)), (std::array<std::int16_t, 4>{4, 3, 2, 1}));
	lin::internal::matrix<std::int16_t, 1, 2> matrix2{};
	matrix2 = lin::internal::matrix<std::int16_t, 2, 1>{1, 2};
	ASSERT_EQ((static_cast<std::array<std::int16_t, 2>>(matrix2)), (std::array<std::int16_t, 2>{1, 2}));
	matrix2 = lin::internal::matrix<std::int8_t, 2, 1>{2, 1};
	ASSERT_EQ((static_cast<std::array<std::int16_t, 2>>(matrix2)), (std::array<std::int16_t, 2>{2, 1}));
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
	ASSERT_EQ(std::addressof(matrix1.y()), std::addressof(matrix1.g()));
	ASSERT_EQ(std::addressof(matrix1.z()), std::addressof(matrix1.b()));
	ASSERT_EQ(std::addressof(matrix1.w()), std::addressof(matrix1.a()));
	lin::internal::matrix<std::int32_t, 2, 2> matrix2{};
	matrix2[0, 0] = 1;
	matrix2[0, 1] = 2;
	matrix2[1, 0] = 3;
	matrix2[1, 1] = 4;
	ASSERT_TRUE(lin::all(matrix2 == lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.xyzw() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.rgba() == lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.wzyx() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 4>{1, 2, 3, 4}.abgr() == lin::internal::matrix<std::int32_t, 1, 4>{4, 3, 2, 1}));

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

TEST(stellarlib_lin_matrix, should_multiply)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	matrix *= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
	matrix *= lin::internal::matrix<std::int32_t, 2, 1>{7, 8};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{21, 48}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * 3 == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
static_assert(lin::all(1 * lin::internal::matrix<std::int32_t, 1, 2>{2, 3} == lin::internal::matrix<std::int32_t, 1, 2>{2, 3}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} * lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{3, 8}));

TEST(stellarlib_lin_matrix, should_divide)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{21, 48};
	matrix /= lin::internal::matrix<std::int32_t, 2, 1>{7, 8};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{3, 6}));
	matrix /= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 6} / 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(6 / lin::internal::matrix<std::int32_t, 1, 2>{3, 6} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{3, 8} / lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_mod)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{10, 12};
	matrix %= lin::internal::matrix<std::int32_t, 2, 1>{6, 7};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
	matrix %= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 5} % 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(5 % lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 6} % lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_add)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{1, 2};
	matrix += 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
	matrix += lin::internal::matrix<std::int32_t, 2, 1>{6, 7};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{10, 12}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + 3 == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
static_assert(lin::all(1 + lin::internal::matrix<std::int32_t, 1, 2>{2, 3} == lin::internal::matrix<std::int32_t, 1, 2>{3, 4}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{4, 6}));

TEST(stellarlib_lin_matrix, should_substract)
{
	lin::internal::matrix<std::int32_t, 1, 2> matrix{10, 12};
	matrix -= lin::internal::matrix<std::int32_t, 2, 1>{6, 7};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{4, 5}));
	matrix -= 3;
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
}

static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 5} - 3 == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));
static_assert(lin::all(5 - lin::internal::matrix<std::int32_t, 1, 2>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{2, 1}));
static_assert(lin::all(lin::internal::matrix<std::int32_t, 1, 2>{4, 6} - lin::internal::matrix<std::int32_t, 2, 1>{3, 4} == lin::internal::matrix<std::int32_t, 1, 2>{1, 2}));

TEST(stellarlib_lin_matrix, should_shift_left)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b0000'0001, 0b0000'0010};
	matrix <<= std::uint8_t{1};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0010, 0b0000'0100}));
	matrix <<= lin::internal::matrix<std::uint8_t, 2, 1>{2, 3};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));
}

static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010} << std::uint8_t{3} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0001'0000}));
static_assert(lin::all(std::uint8_t{0b0000'0001} << lin::internal::matrix<std::uint8_t, 1, 2>{2, 3} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0100, 0b0000'1000}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010} << lin::internal::matrix<std::uint8_t, 2, 1>{3, 4} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'1000, 0b0010'0000}));

TEST(stellarlib_lin_matrix, should_shift_right)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b0000'1000, 0b0010'0000};
	matrix >>= lin::internal::matrix<std::uint8_t, 2, 1>{2, 3};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0010, 0b0000'0100}));
	matrix >>= std::uint8_t{1};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0010}));
}

static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0011, 0b0000'0010} >> std::uint8_t{1} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0001}));
static_assert(lin::all(std::uint8_t{0b0000'0011} >> lin::internal::matrix<std::uint8_t, 1, 2>{2, 1} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0000, 0b0000'0001}));
static_assert(lin::all(lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0100, 0b0000'0011} >> lin::internal::matrix<std::uint8_t, 2, 1>{2, 1} == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0001, 0b0000'0001}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} < 0) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((0 < lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} < lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} <= 0) == lin::internal::matrix<bool, 1, 3>{true, true, false}));
static_assert(lin::all((0 <= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} <= lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} > 0) == lin::internal::matrix<bool, 1, 3>{false, false, true}));
static_assert(lin::all((0 > lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} > lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, false, true}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} >= 0) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((0 >= lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} >= lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, true}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} == 0) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((0 == lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} == lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} != 0) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((0 != lin::internal::matrix<std::int32_t, 1, 3>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{-1, 0, 1} != lin::internal::matrix<std::int32_t, 3, 1>{1, 0, -1}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));

TEST(stellarlib_lin_matrix, should_assign_and)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix &= std::uint8_t{0b1100'0011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'0010, 0b0100'0001}));
	matrix &= lin::internal::matrix<std::uint8_t, 2, 1>{0b0011'1100, 0b0011'1100};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0000, 0b0000'0000}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} & std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0000'0000, 0b0101'0001}));
static_assert(lin::all((std::uint8_t{0b1010'1010} & lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0000'0000}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} & lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1000'1010, 0b0101'0001}));

TEST(stellarlib_lin_matrix, should_assign_xor)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix ^= std::uint8_t{0b1100'0011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0110'1001, 0b1001'0110}));
	matrix ^= lin::internal::matrix<std::uint8_t, 2, 1>{0b0011'1100, 0b0011'1100};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b0101'0101, 0b1010'1010}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} ^ std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1000'1110}));
static_assert(lin::all((std::uint8_t{0b1010'1010} ^ lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1111'1111}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} ^ lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b0111'0001, 0b1000'1110}));

TEST(stellarlib_lin_matrix, should_assign_or)
{
	lin::internal::matrix<std::uint8_t, 1, 2> matrix{0b1010'1010, 0b0101'0101};
	matrix |= std::uint8_t{0b1100'0011};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1110'1011, 0b1101'0111}));
	matrix |= lin::internal::matrix<std::uint8_t, 2, 1>{0b0011'1100, 0b0011'1100};
	ASSERT_TRUE(lin::all(matrix == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1111'1111}));
}

static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} | std::uint8_t{0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1111, 0b1101'1111}));
static_assert(lin::all((std::uint8_t{0b1010'1010} | lin::internal::matrix<std::uint8_t, 1, 2>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1111'1111}));
static_assert(lin::all((lin::internal::matrix<std::uint8_t, 1, 2>{0b1010'1010, 0b1101'1011} | lin::internal::matrix<std::uint8_t, 2, 1>{0b1101'1011, 0b0101'0101}) == lin::internal::matrix<std::uint8_t, 1, 2>{0b1111'1011, 0b1101'1111}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{true, false} && true) == lin::internal::matrix<bool, 1, 3>{true, false}));
static_assert(lin::all((true && lin::internal::matrix<std::int32_t, 1, 3>{false, true}) == lin::internal::matrix<bool, 1, 3>{false, true}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{true, true, false} && lin::internal::matrix<std::int32_t, 3, 1>{false, true, true}) == lin::internal::matrix<bool, 1, 3>{false, true, false}));

static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{false, true} || false) == lin::internal::matrix<bool, 1, 3>{false, true}));
static_assert(lin::all((false || lin::internal::matrix<std::int32_t, 1, 3>{true, false}) == lin::internal::matrix<bool, 1, 3>{true, false}));
static_assert(lin::all((lin::internal::matrix<std::int32_t, 1, 3>{false, false, true} || lin::internal::matrix<std::int32_t, 3, 1>{true, false, false}) == lin::internal::matrix<bool, 1, 3>{true, false, true}));

#pragma clang diagnostic pop
