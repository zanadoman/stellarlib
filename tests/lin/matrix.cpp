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

#include <array>
#include <cstdint>
#include <type_traits>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(sizeof(lin::internal::matrix<float, 1, 1>) == sizeof(float));
static_assert(std::is_standard_layout_v<lin::internal::matrix<float, 1, 1>>);
static_assert(ext::is_trivially_relocatable_v<lin::internal::matrix<float, 2, 2>>);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{5} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{std::array<std::int8_t, 1>{5}} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{lin::internal::matrix<std::int8_t, 1, 1>{5}} == 5);
static_assert(lin::internal::matrix<std::int16_t, 1, 1>{lin::internal::matrix<std::int16_t, 1, 1>{5}} == 5);

static_assert(sizeof(lin::internal::matrix<float, 2, 2>) == 4 * sizeof(float));
static_assert(std::is_standard_layout_v<lin::internal::matrix<float, 2, 2>>);
static_assert(ext::is_trivially_relocatable_v<lin::internal::matrix<float, 1, 1>>);
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{1, 2, 3, 4}) == std::array<std::int16_t, 4>{1, 3, 2, 4});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{std::array<std::int8_t, 4>{1, 2, 3, 4}}) == std::array<std::int16_t, 4>{1, 2, 3, 4});
static_assert(static_cast<std::array<std::int16_t, 2>>(lin::internal::matrix<std::int16_t, 1, 2>{lin::internal::matrix<std::int8_t, 2, 1>{1, 2}}) == std::array<std::int16_t, 2>{1, 2});
static_assert(static_cast<std::array<std::int16_t, 2>>(lin::internal::matrix<std::int16_t, 2, 1>{lin::internal::matrix<std::int8_t, 1, 2>{1, 2}}) == std::array<std::int16_t, 2>{1, 2});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{lin::internal::matrix<std::int8_t, 2, 2>{1, 2, 3, 4}}) == std::array<std::int16_t, 4>{1, 3, 2, 4});
static_assert(static_cast<std::array<std::int16_t, 4>>(lin::internal::matrix<std::int16_t, 2, 2>{lin::internal::matrix<std::int16_t, 2, 2>{1, 2, 3, 4}}) == std::array<std::int16_t, 4>{1, 3, 2, 4});

#pragma clang diagnostic pop
