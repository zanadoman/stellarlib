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

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::internal::abs(1) == 1);
static_assert(lin::internal::abs(-1) == 1);
static_assert(lin::internal::all(lin::internal::abs(lin::internal::matrix<std::int32_t, 1, 2>{1, -1}) == lin::internal::matrix<std::int32_t, 1, 2>{1, 1}));

TEST(stellarlib_lin_intrinsics, acos)
{
	[[maybe_unused]] const auto x = lin::internal::acos(0.0);
	[[maybe_unused]] const auto y = lin::internal::acos(0.0F);
	[[maybe_unused]] const auto z = lin::internal::acos(lin::internal::matrix<float, 1, 2>{0.0F, 0.0F});
	[[maybe_unused]] const auto w = lin::internal::acos(lin::internal::matrix<double, 1, 2>{0.0, 0.0});
}

static_assert(lin::internal::all(lin::internal::matrix<bool, 1, 2>{true, true}));
static_assert(!lin::internal::all(lin::internal::matrix<bool, 1, 2>{true, false}));

static_assert(lin::internal::any(lin::internal::matrix<bool, 1, 2>{true, false}));
static_assert(!lin::internal::any(lin::internal::matrix<bool, 1, 2>{false, false}));

static_assert(lin::internal::asdouble(0b01010001'11101011'10000101'00011111, 0b01000000'00001001'00011110'10111000) == 3.14);

#pragma clang diagnostic pop
