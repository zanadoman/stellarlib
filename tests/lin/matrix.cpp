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

#include <cstdint>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4} == lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4});
static_assert(lin::internal::matrix<std::int32_t, 2, 2>{1, 2, 3, 4} != lin::internal::matrix<std::int32_t, 2, 2>{5, 6, 7, 8});

static_assert(lin::internal::matrix<std::int32_t, 1, 2>{1, 2} + lin::internal::matrix<std::int32_t, 1, 2>{5, 6} == lin::internal::matrix<std::int32_t, 1, 2>{6, 8});

#pragma clang diagnostic pop
