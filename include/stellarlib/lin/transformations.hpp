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

#ifndef STELLARLIB_LIN_TRANSFORMATIONS_HPP
#define STELLARLIB_LIN_TRANSFORMATIONS_HPP

#include <stellarlib/lin/intrinsics.hpp>
#include <stellarlib/lin/matrix.hpp>

#include <cstddef>
#include <type_traits>

/**
 * @brief Linear algebra utilities
 */
namespace stellarlib::lin
{
/**
 * @brief Translates a 3x3 transformation matrix using a 2D vector
 * @param m 3x3 transformation matrix to be translated
 * @param v 2D translation vector
 * @return Translated 3x3 transformation matrix
 */
template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto translate(const internal::matrix<T, 3, 3> &m, const internal::matrix<U, M, N> &v) noexcept
	-> internal::matrix<std::common_type_t<T, U>, 3, 3>
	requires (M * N == 2)
{
	internal::matrix<std::common_type_t<T, U>, 3, 3> res;
	res[0] = m[0];
	res[1] = m[1];
	res[2] = m[0] * v.x() + m[1] * v.y() + m[2];
	return res;
}

/**
 * @brief Translates a 4x4 transformation matrix using a 3D vector
 * @param m 4x4 transformation matrix to be translated
 * @param v 3D translation vector
 * @return Translated 4x4 transformation matrix
 */
template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto translate(const internal::matrix<T, 4, 4> &m, const internal::matrix<U, M, N> &v) noexcept
	-> internal::matrix<std::common_type_t<T, U>, 4, 4>
	requires (M * N == 3)
{
	internal::matrix<std::common_type_t<T, U>, 4, 4> res;
	res[0] = m[0];
	res[1] = m[1];
	res[2] = m[2];
	res[3] = m[0] * v.x() + m[1] * v.y() + m[2] * v.z() + m[3];
	return res;
}

/**
 * @brief Rotates a 3x3 transformation matrix using radians
 * @param m 3x3 transformation matrix to be rotated
 * @param angle Rotation angle in radians
 * @return Rotated 3x3 transformation matrix
 */
template <typename T, typename U>
[[nodiscard]]
constexpr auto rotate(const internal::matrix<T, 3, 3> &m, const U angle) noexcept
	-> internal::matrix<std::common_type_t<T, U>, 3, 3>
	requires (std::is_arithmetic_v<U>)
{
	const auto c{internal::cos(angle)};
	const auto s{internal::sin(angle)};
	internal::matrix<std::common_type_t<T, U>, 3, 3> res;
	res[0] = m[0] * c + m[1] * s;
	res[1] = m[0] * -s + m[1] * c;
	res[2] = m[2];
	return res;
}

/**
 * @brief Rotates a 4x4 transformation matrix using radians and a 3D axis
 * @param m 4x4 transformation matrix to be rotated
 * @param angle Rotation angle in radians
 * @param axis 3D rotation axis
 * @return Rotated 4x4 transformation matrix
 */
template <typename T, typename U, typename V, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto rotate(const internal::matrix<T, 4, 4> &m, const U angle, const internal::matrix<V, M, N> &axis) noexcept
	-> internal::matrix<std::common_type_t<T, U, V>, 4, 4>
	requires (std::is_arithmetic_v<U> && M * N == 3)
{
	const auto c{internal::cos(angle)};
	const auto n{internal::normalize(axis)};
	const auto t{(1 - c) * n};
	const auto s{internal::sin(angle)};
	internal::matrix<std::common_type_t<T, U, V>, 4, 4> res;
	res[0] = m[0] * internal::mad(t.x(), n.x(), c) + m[1] * (t.x() * n.y() + s * n.z()) + m[2] * (t.x() * n.z() - s * n.y());
	res[1] = m[0] * (t.y() * n.x() - s * n.z()) + m[1] * internal::mad(t.y(), n.y(), c) + m[2] * (t.y() * n.z() + s * n.x());
	res[2] = m[0] * (t.z() * n.x() + s * n.y()) + m[1] * (t.z() * n.y() - s * n.x()) + m[2] * internal::mad(t.z(), n.z(), c);
	res[3] = m[3];
	return res;
}

/**
 * @brief Scales a 3x3 transformation matrix using a 2D vector
 * @param m 3x3 transformation matrix to be scaled
 * @param v 2D scaling vector
 * @return Scaled 3x3 transformation matrix
 */
template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto scale(const internal::matrix<T, 3, 3> &m, const internal::matrix<U, M, N> &v) noexcept
	-> internal::matrix<std::common_type_t<T, U>, 3, 3>
	requires (M * N == 2)
{
	internal::matrix<std::common_type_t<T, U>, 3, 3> res;
	res[0] = m[0] * v.x();
	res[1] = m[1] * v.y();
	res[2] = m[2];
	return res;
}

/**
 * @brief Scales a 4x4 transformation matrix using a 3D vector
 * @param m 4x4 transformation matrix to be scaled
 * @param v 3D scaling vector
 * @return Scaled 4x4 transformation matrix
 */
template <typename T, typename U, std::size_t M, std::size_t N>
[[nodiscard]]
constexpr auto scale(const internal::matrix<T, 4, 4> &m, const internal::matrix<U, M, N> &v) noexcept
	-> internal::matrix<std::common_type_t<T, U>, 4, 4>
	requires (M * N == 3)
{
	internal::matrix<std::common_type_t<T, U>, 4, 4> res;
	res[0] = m[0] * v.x();
	res[1] = m[1] * v.y();
	res[2] = m[2] * v.z();
	res[3] = m[3];
	return res;
}

/**
 * @brief Constructs a 4x4 perspective matrix
 * @param fovy Vertical field of view of the camera
 * @param aspect Aspect ratio of the viewport
 * @param near Distance of the near clipping plane
 * @return Constructed 4x4 perspective matrix
 */
template <typename T, typename U, typename V>
[[nodiscard]]
constexpr auto perspective(const T fovy, const U aspect, const V near) noexcept
	requires (std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>)
{
	const auto f{internal::rcp(internal::tan(fovy / 2))};

	return internal::matrix<std::common_type_t<T, U, V>, 4, 4>{
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, -1, -1,
		0, 0, -near, 0
	};
}
}

#endif
