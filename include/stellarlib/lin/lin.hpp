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
#include <stellarlib/lin/intrinsics.hpp>
#include <stellarlib/lin/matrix.hpp>
/* IWYU pragma: end_exports */

#include <cstddef>
#include <cstdint>

/**
 * @brief Linear algebra module
 */
namespace stellarlib::lin
{
/**
 * @brief Generic M*N matrix with per-component operations
 * @tparam T Arithmetic type of the components
 * @tparam M Number of rows in the matrix
 * @tparam N Number of columns in the matrix
 */
template <typename T, std::size_t M = 4, std::size_t N = 4>
using matrix = internal::matrix<T, M, N>;

/**
 * @brief Generic ND vector with per-component operations
 * @tparam T Arithmetic type of the components
 * @tparam N Dimension of the vector
 */
template <typename T, std::size_t N = 4>
using vector = matrix<T, 1, N>;

/// 1D boolean vector
using bool1 = vector<bool, 1>;
/// 2D boolean vector
using bool2 = vector<bool, 2>;
/// 3D boolean vector
using bool3 = vector<bool, 3>;
/// 4D boolean vector
using bool4 = vector<bool>;

/// 1D char vector
using char1 = vector<std::int8_t, 1>;
/// 2D char vector
using char2 = vector<std::int8_t, 2>;
/// 3D char vector
using char3 = vector<std::int8_t, 3>;
/// 4D char vector
using char4 = vector<std::int8_t>;

/// 1D unsigned char vector
using uchar1 = vector<std::uint8_t, 1>;
/// 2D unsigned char vector
using uchar2 = vector<std::uint8_t, 2>;
/// 3D unsigned char vector
using uchar3 = vector<std::uint8_t, 3>;
/// 4D unsigned char vector
using uchar4 = vector<std::uint8_t>;

/// 1D short vector
using short1 = vector<std::int16_t, 1>;
/// 2D short vector
using short2 = vector<std::int16_t, 2>;
/// 3D short vector
using short3 = vector<std::int16_t, 3>;
/// 4D short vector
using short4 = vector<std::int16_t>;

/// 1D unsigned short vector
using ushort1 = vector<std::uint16_t, 1>;
/// 2D unsigned short vector
using ushort2 = vector<std::uint16_t, 2>;
/// 3D unsigned short vector
using ushort3 = vector<std::uint16_t, 3>;
/// 4D unsigned short vector
using ushort4 = vector<std::uint16_t>;

/// 1D integer vector
using int1 = vector<std::int32_t, 1>;
/// 2D integer vector
using int2 = vector<std::int32_t, 2>;
/// 3D integer vector
using int3 = vector<std::int32_t, 3>;
/// 4D integer vector
using int4 = vector<std::int32_t>;

/// 1D unsigned integer vector
using uint1 = vector<std::uint32_t, 1>;
/// 2D unsigned integer vector
using uint2 = vector<std::uint32_t, 2>;
/// 3D unsigned integer vector
using uint3 = vector<std::uint32_t, 3>;
/// 4D unsigned integer vector
using uint4 = vector<std::uint32_t>;

/// 1D long vector
using long1 = vector<std::int64_t, 1>;
/// 2D long vector
using long2 = vector<std::int64_t, 2>;
/// 3D long vector
using long3 = vector<std::int64_t, 3>;
/// 4D long vector
using long4 = vector<std::int64_t>;

/// 1D unsigned long vector
using ulong1 = vector<std::uint64_t, 1>;
/// 2D unsigned long vector
using ulong2 = vector<std::uint64_t, 2>;
/// 3D unsigned long vector
using ulong3 = vector<std::uint64_t, 3>;
/// 4D unsigned long vector
using ulong4 = vector<std::uint64_t>;

/// 1D float vector
using float1 = vector<float, 1>;
/// 2D float vector
using float2 = vector<float, 2>;
/// 3D float vector
using float3 = vector<float, 3>;
/// 4D float vector
using float4 = vector<float>;

/// 1D double vector
using double1 = vector<double, 1>;
/// 2D double vector
using double2 = vector<double, 2>;
/// 3D double vector
using double3 = vector<double, 3>;
/// 4D double vector
using double4 = vector<double>;

/// 1x1 boolean matrix
using bool1x1 = bool1;
/// 2x1 boolean matrix
using bool2x1 = matrix<bool, 2, 1>;
/// 3x1 boolean matrix
using bool3x1 = matrix<bool, 3, 1>;
/// 4x1 boolean matrix
using bool4x1 = matrix<bool, 4, 1>;
/// 1x2 boolean matrix
using bool1x2 = bool2;
/// 2x2 boolean matrix
using bool2x2 = matrix<bool, 2, 2>;
/// 3x2 boolean matrix
using bool3x2 = matrix<bool, 3, 2>;
/// 4x2 boolean matrix
using bool4x2 = matrix<bool, 4, 2>;
/// 1x3 boolean matrix
using bool1x3 = bool3;
/// 2x3 boolean matrix
using bool2x3 = matrix<bool, 2, 3>;
/// 3x3 boolean matrix
using bool3x3 = matrix<bool, 3, 3>;
/// 4x3 boolean matrix
using bool4x3 = matrix<bool, 4, 3>;
/// 1x4 boolean matrix
using bool1x4 = bool4;
/// 2x4 boolean matrix
using bool2x4 = matrix<bool, 2>;
/// 3x4 boolean matrix
using bool3x4 = matrix<bool, 3>;
/// 4x4 boolean matrix
using bool4x4 = matrix<bool>;

/// 1x1 char matrix
using char1x1 = char1;
/// 2x1 char matrix
using char2x1 = matrix<std::int8_t, 2, 1>;
/// 3x1 char matrix
using char3x1 = matrix<std::int8_t, 3, 1>;
/// 4x1 char matrix
using char4x1 = matrix<std::int8_t, 4, 1>;
/// 1x2 char matrix
using char1x2 = char2;
/// 2x2 char matrix
using char2x2 = matrix<std::int8_t, 2, 2>;
/// 3x2 char matrix
using char3x2 = matrix<std::int8_t, 3, 2>;
/// 4x2 char matrix
using char4x2 = matrix<std::int8_t, 4, 2>;
/// 1x3 char matrix
using char1x3 = char3;
/// 2x3 char matrix
using char2x3 = matrix<std::int8_t, 2, 3>;
/// 3x3 char matrix
using char3x3 = matrix<std::int8_t, 3, 3>;
/// 4x3 char matrix
using char4x3 = matrix<std::int8_t, 4, 3>;
/// 1x4 char matrix
using char1x4 = char4;
/// 2x4 char matrix
using char2x4 = matrix<std::int8_t, 2>;
/// 3x4 char matrix
using char3x4 = matrix<std::int8_t, 3>;
/// 4x4 char matrix
using char4x4 = matrix<std::int8_t>;

/// 1x1 unsigned char matrix
using uchar1x1 = uchar1;
/// 2x1 unsigned char matrix
using uchar2x1 = matrix<std::uint8_t, 2, 1>;
/// 3x1 unsigned char matrix
using uchar3x1 = matrix<std::uint8_t, 3, 1>;
/// 4x1 unsigned char matrix
using uchar4x1 = matrix<std::uint8_t, 4, 1>;
/// 1x2 unsigned char matrix
using uchar1x2 = uchar2;
/// 2x2 unsigned char matrix
using uchar2x2 = matrix<std::uint8_t, 2, 2>;
/// 3x2 unsigned char matrix
using uchar3x2 = matrix<std::uint8_t, 3, 2>;
/// 4x2 unsigned char matrix
using uchar4x2 = matrix<std::uint8_t, 4, 2>;
/// 1x3 unsigned char matrix
using uchar1x3 = uchar3;
/// 2x3 unsigned char matrix
using uchar2x3 = matrix<std::uint8_t, 2, 3>;
/// 3x3 unsigned char matrix
using uchar3x3 = matrix<std::uint8_t, 3, 3>;
/// 4x3 unsigned char matrix
using uchar4x3 = matrix<std::uint8_t, 4, 3>;
/// 1x4 unsigned char matrix
using uchar1x4 = uchar4;
/// 2x4 unsigned char matrix
using uchar2x4 = matrix<std::uint8_t, 2>;
/// 3x4 unsigned char matrix
using uchar3x4 = matrix<std::uint8_t, 3>;
/// 4x4 unsigned char matrix
using uchar4x4 = matrix<std::uint8_t>;

/// 1x1 short matrix
using short1x1 = short1;
/// 2x1 short matrix
using short2x1 = matrix<std::int16_t, 2, 1>;
/// 3x1 short matrix
using short3x1 = matrix<std::int16_t, 3, 1>;
/// 4x1 short matrix
using short4x1 = matrix<std::int16_t, 4, 1>;
/// 1x2 short matrix
using short1x2 = short2;
/// 2x2 short matrix
using short2x2 = matrix<std::int16_t, 2, 2>;
/// 3x2 short matrix
using short3x2 = matrix<std::int16_t, 3, 2>;
/// 4x2 short matrix
using short4x2 = matrix<std::int16_t, 4, 2>;
/// 1x3 short matrix
using short1x3 = short3;
/// 2x3 short matrix
using short2x3 = matrix<std::int16_t, 2, 3>;
/// 3x3 short matrix
using short3x3 = matrix<std::int16_t, 3, 3>;
/// 4x3 short matrix
using short4x3 = matrix<std::int16_t, 4, 3>;
/// 1x4 short matrix
using short1x4 = short4;
/// 2x4 short matrix
using short2x4 = matrix<std::int16_t, 2>;
/// 3x4 short matrix
using short3x4 = matrix<std::int16_t, 3>;
/// 4x4 short matrix
using short4x4 = matrix<std::int16_t>;

/// 1x1 unsigned short matrix
using ushort1x1 = ushort1;
/// 2x1 unsigned short matrix
using ushort2x1 = matrix<std::uint16_t, 2, 1>;
/// 3x1 unsigned short matrix
using ushort3x1 = matrix<std::uint16_t, 3, 1>;
/// 4x1 unsigned short matrix
using ushort4x1 = matrix<std::uint16_t, 4, 1>;
/// 1x2 unsigned short matrix
using ushort1x2 = ushort2;
/// 2x2 unsigned short matrix
using ushort2x2 = matrix<std::uint16_t, 2, 2>;
/// 3x2 unsigned short matrix
using ushort3x2 = matrix<std::uint16_t, 3, 2>;
/// 4x2 unsigned short matrix
using ushort4x2 = matrix<std::uint16_t, 4, 2>;
/// 1x3 unsigned short matrix
using ushort1x3 = ushort3;
/// 2x3 unsigned short matrix
using ushort2x3 = matrix<std::uint16_t, 2, 3>;
/// 3x3 unsigned short matrix
using ushort3x3 = matrix<std::uint16_t, 3, 3>;
/// 4x3 unsigned short matrix
using ushort4x3 = matrix<std::uint16_t, 4, 3>;
/// 1x4 unsigned short matrix
using ushort1x4 = ushort4;
/// 2x4 unsigned short matrix
using ushort2x4 = matrix<std::uint16_t, 2>;
/// 3x4 unsigned short matrix
using ushort3x4 = matrix<std::uint16_t, 3>;
/// 4x4 unsigned short matrix
using ushort4x4 = matrix<std::uint16_t>;

/// 1x1 integer matrix
using int1x1 = int1;
/// 2x1 integer matrix
using int2x1 = matrix<std::int32_t, 2, 1>;
/// 3x1 integer matrix
using int3x1 = matrix<std::int32_t, 3, 1>;
/// 4x1 integer matrix
using int4x1 = matrix<std::int32_t, 4, 1>;
/// 1x2 integer matrix
using int1x2 = int2;
/// 2x2 integer matrix
using int2x2 = matrix<std::int32_t, 2, 2>;
/// 3x2 integer matrix
using int3x2 = matrix<std::int32_t, 3, 2>;
/// 4x2 integer matrix
using int4x2 = matrix<std::int32_t, 4, 2>;
/// 1x3 integer matrix
using int1x3 = int3;
/// 2x3 integer matrix
using int2x3 = matrix<std::int32_t, 2, 3>;
/// 3x3 integer matrix
using int3x3 = matrix<std::int32_t, 3, 3>;
/// 4x3 integer matrix
using int4x3 = matrix<std::int32_t, 4, 3>;
/// 1x4 integer matrix
using int1x4 = int4;
/// 2x4 integer matrix
using int2x4 = matrix<std::int32_t, 2>;
/// 3x4 integer matrix
using int3x4 = matrix<std::int32_t, 3>;
/// 4x4 integer matrix
using int4x4 = matrix<std::int32_t>;

/// 1x1 unsigned integer matrix
using uint1x1 = uint1;
/// 2x1 unsigned integer matrix
using uint2x1 = matrix<std::uint32_t, 2, 1>;
/// 3x1 unsigned integer matrix
using uint3x1 = matrix<std::uint32_t, 3, 1>;
/// 4x1 unsigned integer matrix
using uint4x1 = matrix<std::uint32_t, 4, 1>;
/// 1x2 unsigned integer matrix
using uint1x2 = uint2;
/// 2x2 unsigned integer matrix
using uint2x2 = matrix<std::uint32_t, 2, 2>;
/// 3x2 unsigned integer matrix
using uint3x2 = matrix<std::uint32_t, 3, 2>;
/// 4x2 unsigned integer matrix
using uint4x2 = matrix<std::uint32_t, 4, 2>;
/// 1x3 unsigned integer matrix
using uint1x3 = uint3;
/// 2x3 unsigned integer matrix
using uint2x3 = matrix<std::uint32_t, 2, 3>;
/// 3x3 unsigned integer matrix
using uint3x3 = matrix<std::uint32_t, 3, 3>;
/// 4x3 unsigned integer matrix
using uint4x3 = matrix<std::uint32_t, 4, 3>;
/// 1x4 unsigned integer matrix
using uint1x4 = uint4;
/// 2x4 unsigned integer matrix
using uint2x4 = matrix<std::uint32_t, 2>;
/// 3x4 unsigned integer matrix
using uint3x4 = matrix<std::uint32_t, 3>;
/// 4x4 unsigned integer matrix
using uint4x4 = matrix<std::uint32_t>;

/// 1x1 long matrix
using long1x1 = long1;
/// 2x1 long matrix
using long2x1 = matrix<std::int64_t, 2, 1>;
/// 3x1 long matrix
using long3x1 = matrix<std::int64_t, 3, 1>;
/// 4x1 long matrix
using long4x1 = matrix<std::int64_t, 4, 1>;
/// 1x2 long matrix
using long1x2 = long2;
/// 2x2 long matrix
using long2x2 = matrix<std::int64_t, 2, 2>;
/// 3x2 long matrix
using long3x2 = matrix<std::int64_t, 3, 2>;
/// 4x2 long matrix
using long4x2 = matrix<std::int64_t, 4, 2>;
/// 1x3 long matrix
using long1x3 = long3;
/// 2x3 long matrix
using long2x3 = matrix<std::int64_t, 2, 3>;
/// 3x3 long matrix
using long3x3 = matrix<std::int64_t, 3, 3>;
/// 4x3 long matrix
using long4x3 = matrix<std::int64_t, 4, 3>;
/// 1x4 long matrix
using long1x4 = long4;
/// 2x4 long matrix
using long2x4 = matrix<std::int64_t, 2>;
/// 3x4 long matrix
using long3x4 = matrix<std::int64_t, 3>;
/// 4x4 long matrix
using long4x4 = matrix<std::int64_t>;

/// 1x1 unsigned long matrix
using ulong1x1 = ulong1;
/// 2x1 unsigned long matrix
using ulong2x1 = matrix<std::uint64_t, 2, 1>;
/// 3x1 unsigned long matrix
using ulong3x1 = matrix<std::uint64_t, 3, 1>;
/// 4x1 unsigned long matrix
using ulong4x1 = matrix<std::uint64_t, 4, 1>;
/// 1x2 unsigned long matrix
using ulong1x2 = ulong2;
/// 2x2 unsigned long matrix
using ulong2x2 = matrix<std::uint64_t, 2, 2>;
/// 3x2 unsigned long matrix
using ulong3x2 = matrix<std::uint64_t, 3, 2>;
/// 4x2 unsigned long matrix
using ulong4x2 = matrix<std::uint64_t, 4, 2>;
/// 1x3 unsigned long matrix
using ulong1x3 = ulong3;
/// 2x3 unsigned long matrix
using ulong2x3 = matrix<std::uint64_t, 2, 3>;
/// 3x3 unsigned long matrix
using ulong3x3 = matrix<std::uint64_t, 3, 3>;
/// 4x3 unsigned long matrix
using ulong4x3 = matrix<std::uint64_t, 4, 3>;
/// 1x4 unsigned long matrix
using ulong1x4 = ulong4;
/// 2x4 unsigned long matrix
using ulong2x4 = matrix<std::uint64_t, 2>;
/// 3x4 unsigned long matrix
using ulong3x4 = matrix<std::uint64_t, 3>;
/// 4x4 unsigned long matrix
using ulong4x4 = matrix<std::uint64_t>;

/// 1x1 float matrix
using float1x1 = float1;
/// 2x1 float matrix
using float2x1 = matrix<float, 2, 1>;
/// 3x1 float matrix
using float3x1 = matrix<float, 3, 1>;
/// 4x1 float matrix
using float4x1 = matrix<float, 4, 1>;
/// 1x2 float matrix
using float1x2 = float2;
/// 2x2 float matrix
using float2x2 = matrix<float, 2, 2>;
/// 3x2 float matrix
using float3x2 = matrix<float, 3, 2>;
/// 4x2 float matrix
using float4x2 = matrix<float, 4, 2>;
/// 1x3 float matrix
using float1x3 = float3;
/// 2x3 float matrix
using float2x3 = matrix<float, 2, 3>;
/// 3x3 float matrix
using float3x3 = matrix<float, 3, 3>;
/// 4x3 float matrix
using float4x3 = matrix<float, 4, 3>;
/// 1x4 float matrix
using float1x4 = float4;
/// 2x4 float matrix
using float2x4 = matrix<float, 2>;
/// 3x4 float matrix
using float3x4 = matrix<float, 3>;
/// 4x4 float matrix
using float4x4 = matrix<float>;

/// 1x1 double matrix
using double1x1 = double1;
/// 2x1 double matrix
using double2x1 = matrix<double, 2, 1>;
/// 3x1 double matrix
using double3x1 = matrix<double, 3, 1>;
/// 4x1 double matrix
using double4x1 = matrix<double, 4, 1>;
/// 1x2 double matrix
using double1x2 = double2;
/// 2x2 double matrix
using double2x2 = matrix<double, 2, 2>;
/// 3x2 double matrix
using double3x2 = matrix<double, 3, 2>;
/// 4x2 double matrix
using double4x2 = matrix<double, 4, 2>;
/// 1x3 double matrix
using double1x3 = double3;
/// 2x3 double matrix
using double2x3 = matrix<double, 2, 3>;
/// 3x3 double matrix
using double3x3 = matrix<double, 3, 3>;
/// 4x3 double matrix
using double4x3 = matrix<double, 4, 3>;
/// 1x4 double matrix
using double1x4 = double4;
/// 2x4 double matrix
using double2x4 = matrix<double, 2>;
/// 3x4 double matrix
using double3x4 = matrix<double, 3>;
/// 4x4 double matrix
using double4x4 = matrix<double>;

/**
 * @brief Returns the absolute value of x (per-component)
 */
using internal::abs;

/**
 * @brief Returns the arccosine of x (per-component)
 */
using internal::acos;

/**
 * @brief Determines if all components of x are truthy
 */
using internal::all;

/**
 * @brief Determines if any components of x are truthy
 */
using internal::any;

/**
 * @brief Returns the arcsine of x (per-component)
 */
using internal::asin;

/**
 * @brief Returns the arctangent of x (per-component)
 */
using internal::atan;

/**
 * @brief Returns the arctangent of y and x (per-component)
 */
using internal::atan2;

/**
 * @brief Returns the smallest integer value that is greater than or equal to x (per-component)
 */
using internal::ceil;

/**
 * @brief Clamps x to the range [min, max] (per-component)
 */
using internal::clamp;

/**
 * @brief Returns the cosine of x (per-component)
 */
using internal::cos;

/**
 * @brief Returns the hyperbolic cosine of x (per-component)
 */
using internal::cosh;

/**
 * @brief Returns the cross product of two 3D vectors
 */
using internal::cross;

/**
 * @brief Converts x from radians to degrees (per-component)
 */
using internal::degrees;

/**
 * @brief Returns the determinant of the specified square matrix
 */
using internal::determinant;

/**
 * @brief Returns a distance scalar between two vectors
 */
using internal::distance;

/**
 * @brief Returns the dot product of two vectors
 */
using internal::dot;

/**
 * @brief Calculates a distance vector for lighting
 */
using internal::dst;

/**
 * @brief Returns the base-e exponential of x (per-component)
 */
using internal::exp;

/**
 * @brief Returns the base 2 exponential of x (per-component)
 */
using internal::exp2;

/**
 * @brief Flips the surface-normal (if needed) to face in a direction opposite to i; returns the result in n
 */
using internal::faceforward;

/**
 * @brief Returns the largest integer that is less than or equal to x (per-component)
 */
using internal::floor;

/**
 * @brief Returns the fused multiply-addition of a*b+c (per-component)
 */
using internal::fma;

/**
 * @brief Returns the floating-point remainder of x/y (per-component)
 */
using internal::fmod;

/**
 * @brief Returns the fractional part of x; which is greater than or equal to 0 and less than 1 (per-component)
 */
using internal::frac;

/**
 * @brief Returns the result of multiplying x by two, raised to the power of exp (per-component)
 */
using internal::ldexp;

/**
 * @brief Returns the length of the specified vector
 */
using internal::length;

/**
 * @brief Performs a linear interpolation (per-component)
 */
using internal::lerp;

/**
 * @brief Returns a lighting coefficient vector
 */
using internal::lit;

/**
 * @brief Returns the base-e logarithm of x (per-component)
 */
using internal::log;

/**
 * @brief Returns the base-10 logarithm of x (per-component)
 */
using internal::log10;

/**
 * @brief Returns the base-2 logarithm of x (per-component)
 */
using internal::log2;

/**
 * @brief Performs an arithmetic multiply/add operation on a, b and c (per-component)
 */
using internal::mad;

/**
 * @brief Selects the greater of x and y (per-component)
 */
using internal::max;

/**
 * @brief Selects the lesser of x and y (per-component)
 */
using internal::min;

/**
 * @brief Splits the value x into fractional and integer parts, each of which has the same sign as x (per-component)
 */
using internal::modf;

/**
 * @brief Multiplies x and y using matrix math; the inner dimension x-columns and y-rows must be equal
 */
using internal::mul;

/**
 * @brief Normalizes the specified vector according to x/length(x)
 */
using internal::normalize;

/**
 * @brief Returns x raised to y (per-component)
 */
using internal::pow;

/**
 * @brief Converts x from degrees to radians (per-component)
 */
using internal::radians;

/**
 * @brief Returns the reciprocal of x (per-component)
 */
using internal::rcp;

/**
 * @brief Returns a reflection vector using an incident ray and a surface normal
 */
using internal::reflect;

/**
 * @brief Returns a refraction vector using an entering ray, a surface normal, and a refraction index
 */
using internal::refract;

/**
 * @brief Rounds x to the nearest integer; halfway cases are rounded away from zero (per-component)
 */
using internal::round;

/**
 * @brief Returns the reciprocal of the square root of x (per-component)
 */
using internal::rsqrt;

/**
 * @brief Clamps x within the range [0, 1] (per-component)
 */
using internal::saturate;

/**
 * @brief Returns the sign of x (per-component)
 */
using internal::sign;

/**
 * @brief Returns the sine of x (per-component)
 */
using internal::sin;

/**
 * @brief Returns the sine and cosine of x (per-component)
 */
using internal::sincos;

/**
 * @brief Returns the hyperbolic sine of x (per-component)
 */
using internal::sinh;

/**
 * @brief Returns a smooth Hermite interpolation between 0 and 1, if x is in the range [min, max] (per-component)
 */
using internal::smoothstep;

/**
 * @brief Returns the square root of x (per-component)
 */
using internal::sqrt;

/**
 * @brief Compares y and x, returning 0 or 1 based on which value is greater (per-component)
 */
using internal::step;

/**
 * @brief Returns the tangent of x (per-component)
 */
using internal::tan;

/**
 * @brief Returns the hyperbolic tangent of x (per-component)
 */
using internal::tanh;

/**
 * @brief Transposes the specified input matrix
 */
using internal::transpose;

/**
 * @brief Truncates x to the integer component (per-component)
 */
using internal::trunc;
}

#endif
