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

#ifndef STELLARLIB_LIN_LIN_HPP
#define STELLARLIB_LIN_LIN_HPP

/* IWYU pragma: begin_exports */
#include <stellarlib/lin/matrix.hpp>
/* IWYU pragma: end_exports */

#include <cstdint>

namespace stellarlib::lin
{
using bool1x1 = internal::matrix<bool, 1, 1>;
using bool2x1 = internal::matrix<bool, 2, 1>;
using bool3x1 = internal::matrix<bool, 3, 1>;
using bool4x1 = internal::matrix<bool, 4, 1>;
using bool1x2 = internal::matrix<bool, 1, 2>;
using bool2x2 = internal::matrix<bool, 2, 2>;
using bool3x2 = internal::matrix<bool, 3, 2>;
using bool4x2 = internal::matrix<bool, 4, 2>;
using bool1x3 = internal::matrix<bool, 1, 3>;
using bool2x3 = internal::matrix<bool, 2, 3>;
using bool3x3 = internal::matrix<bool, 3, 3>;
using bool4x3 = internal::matrix<bool, 4, 3>;
using bool1x4 = internal::matrix<bool, 1, 4>;
using bool2x4 = internal::matrix<bool, 2, 4>;
using bool3x4 = internal::matrix<bool, 3, 4>;
using bool4x4 = internal::matrix<bool, 4, 4>;

using int1x1 = internal::matrix<std::int32_t, 1, 1>;
using int2x1 = internal::matrix<std::int32_t, 2, 1>;
using int3x1 = internal::matrix<std::int32_t, 3, 1>;
using int4x1 = internal::matrix<std::int32_t, 4, 1>;
using int1x2 = internal::matrix<std::int32_t, 1, 2>;
using int2x2 = internal::matrix<std::int32_t, 2, 2>;
using int3x2 = internal::matrix<std::int32_t, 3, 2>;
using int4x2 = internal::matrix<std::int32_t, 4, 2>;
using int1x3 = internal::matrix<std::int32_t, 1, 3>;
using int2x3 = internal::matrix<std::int32_t, 2, 3>;
using int3x3 = internal::matrix<std::int32_t, 3, 3>;
using int4x3 = internal::matrix<std::int32_t, 4, 3>;
using int1x4 = internal::matrix<std::int32_t, 1, 4>;
using int2x4 = internal::matrix<std::int32_t, 2, 4>;
using int3x4 = internal::matrix<std::int32_t, 3, 4>;
using int4x4 = internal::matrix<std::int32_t, 4, 4>;

using uint1x1 = internal::matrix<std::uint32_t, 1, 1>;
using uint2x1 = internal::matrix<std::uint32_t, 2, 1>;
using uint3x1 = internal::matrix<std::uint32_t, 3, 1>;
using uint4x1 = internal::matrix<std::uint32_t, 4, 1>;
using uint1x2 = internal::matrix<std::uint32_t, 1, 2>;
using uint2x2 = internal::matrix<std::uint32_t, 2, 2>;
using uint3x2 = internal::matrix<std::uint32_t, 3, 2>;
using uint4x2 = internal::matrix<std::uint32_t, 4, 2>;
using uint1x3 = internal::matrix<std::uint32_t, 1, 3>;
using uint2x3 = internal::matrix<std::uint32_t, 2, 3>;
using uint3x3 = internal::matrix<std::uint32_t, 3, 3>;
using uint4x3 = internal::matrix<std::uint32_t, 4, 3>;
using uint1x4 = internal::matrix<std::uint32_t, 1, 4>;
using uint2x4 = internal::matrix<std::uint32_t, 2, 4>;
using uint3x4 = internal::matrix<std::uint32_t, 3, 4>;
using uint4x4 = internal::matrix<std::uint32_t, 4, 4>;

using float1x1 = internal::matrix<float, 1, 1>;
using float2x1 = internal::matrix<float, 2, 1>;
using float3x1 = internal::matrix<float, 3, 1>;
using float4x1 = internal::matrix<float, 4, 1>;
using float1x2 = internal::matrix<float, 1, 2>;
using float2x2 = internal::matrix<float, 2, 2>;
using float3x2 = internal::matrix<float, 3, 2>;
using float4x2 = internal::matrix<float, 4, 2>;
using float1x3 = internal::matrix<float, 1, 3>;
using float2x3 = internal::matrix<float, 2, 3>;
using float3x3 = internal::matrix<float, 3, 3>;
using float4x3 = internal::matrix<float, 4, 3>;
using float1x4 = internal::matrix<float, 1, 4>;
using float2x4 = internal::matrix<float, 2, 4>;
using float3x4 = internal::matrix<float, 3, 4>;
using float4x4 = internal::matrix<float, 4, 4>;

using double1x1 = internal::matrix<double, 1, 1>;
using double2x1 = internal::matrix<double, 2, 1>;
using double3x1 = internal::matrix<double, 3, 1>;
using double4x1 = internal::matrix<double, 4, 1>;
using double1x2 = internal::matrix<double, 1, 2>;
using double2x2 = internal::matrix<double, 2, 2>;
using double3x2 = internal::matrix<double, 3, 2>;
using double4x2 = internal::matrix<double, 4, 2>;
using double1x3 = internal::matrix<double, 1, 3>;
using double2x3 = internal::matrix<double, 2, 3>;
using double3x3 = internal::matrix<double, 3, 3>;
using double4x3 = internal::matrix<double, 4, 3>;
using double1x4 = internal::matrix<double, 1, 4>;
using double2x4 = internal::matrix<double, 2, 4>;
using double3x4 = internal::matrix<double, 3, 4>;
using double4x4 = internal::matrix<double, 4, 4>;
}

#endif
