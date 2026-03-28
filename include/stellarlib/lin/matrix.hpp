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

#ifndef STELLARLIB_LIN_MATRIX_HPP
#define STELLARLIB_LIN_MATRIX_HPP

#include <array>
#include <cstddef>
#include <memory>
#include <ranges>
#include <type_traits>
#include <utility>

namespace stellarlib::lin::internal
{
template <typename T, std::size_t M, std::size_t N>
class matrix final : public std::array<T, M * N>
{
public:
	[[nodiscard]]
	constexpr matrix() noexcept = default;

	template <typename ...Args>
	[[nodiscard]]
	explicit constexpr matrix(Args &&...args) noexcept
		requires (std::is_convertible_v<Args, T> && ...)
		: std::array<T, M * N>{static_cast<T>(std::forward<Args>(args))...}
	{}

	[[nodiscard]]
	explicit constexpr matrix(const std::array<T, M * N> &elems) noexcept
		: std::array<T, M * N>{elems}
	{}

	template <typename U>
	[[nodiscard]]
	explicit constexpr matrix(const std::array<U, M * N> &elems) noexcept
	{
		for (const auto [lhs, rhs] : std::views::zip(*this, elems)) {
			lhs = rhs;
		}
	}

	[[nodiscard]]
	explicit constexpr matrix(const matrix<T, N, M> &other) noexcept
		requires (M == 1 || N == 1)
		: std::array<T, M * N>{other}
	{}

	template <typename U>
	[[nodiscard]]
	explicit constexpr matrix(const matrix<U, N, M> &other) noexcept
		requires (M == 1 || N == 1)
	{
		for (const auto [lhs, rhs] : std::views::zip(*this, other)) {
			lhs = rhs;
		}
	}

	[[nodiscard]]
	constexpr matrix(const matrix &) noexcept = default;

	[[nodiscard]]
	constexpr matrix(matrix &&) noexcept = default;

	template <typename ...Args>
	constexpr auto operator=(Args &&...args) noexcept
		-> auto &
	{
		std::construct_at(this, std::forward<Args>(args)...);
		return *this;
	}

	template <typename U>
	constexpr auto operator=(const std::array<U, M * N> &elems) noexcept
		-> auto &
	{
		std::construct_at(this, elems);
		return *this;
	}

	template <typename U>
	constexpr auto operator=(const matrix<U, N, M> &other) noexcept
		-> auto &
	{
		std::construct_at(this, other);
		return *this;
	}

	constexpr auto operator=(const matrix &) noexcept
		-> matrix & = default;

	constexpr auto operator=(matrix &&) noexcept
		-> matrix & = default;

	constexpr ~matrix() noexcept = default;

	template <typename U>
	[[nodiscard]]
	explicit constexpr operator U() noexcept
		requires (std::is_convertible_v<T, U> && M * N == 1)
	{
		return std::array<T, M * N>::front();
	}

#define STELLARLIB_LIN_MATRIX_ACCESSOR_SINGLE_IMPL(expr, i)\
	[[nodiscard]]\
	constexpr auto expr() const noexcept\
	{\
		return elem<i>();\
	}\
\
	[[nodiscard]]\
	constexpr auto expr() noexcept\
		-> auto &\
	{\
		return elem<i>();\
	}

#define STELLARLIB_LIN_MATRIX_ACCESSOR_DOUBLE_IMPL(expr1, expr2, i)\
	STELLARLIB_LIN_MATRIX_ACCESSOR_SINGLE_IMPL(expr1, i);\
	STELLARLIB_LIN_MATRIX_ACCESSOR_SINGLE_IMPL(expr2, i);

#define STELLARLIB_LIN_MATRIX_SWIZZLE_SINGLE_IMPL(expr, ...)\
	[[nodiscard]]\
	constexpr auto expr() const noexcept\
	{\
		return swizzle<__VA_ARGS__>();\
	}

#define STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(expr1, expr2, ...)\
	STELLARLIB_LIN_MATRIX_SWIZZLE_SINGLE_IMPL(expr1, __VA_ARGS__);\
	STELLARLIB_LIN_MATRIX_SWIZZLE_SINGLE_IMPL(expr2, __VA_ARGS__);

	STELLARLIB_LIN_MATRIX_ACCESSOR_DOUBLE_IMPL(x, r, 0);
	STELLARLIB_LIN_MATRIX_ACCESSOR_DOUBLE_IMPL(y, g, 1);
	STELLARLIB_LIN_MATRIX_ACCESSOR_DOUBLE_IMPL(z, b, 2);
	STELLARLIB_LIN_MATRIX_ACCESSOR_DOUBLE_IMPL(w, a, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xx, rr, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xy, rg, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xz, rb, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xw, ra, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yx, gr, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yy, gg, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yz, gb, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yw, ga, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zx, br, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zy, bg, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zz, bb, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zw, ba, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wx, ar, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wy, ag, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wz, ab, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ww, aa, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxx, rrr, 0, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxy, rrg, 0, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxz, rrb, 0, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxw, rra, 0, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyx, rgr, 0, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyy, rgg, 0, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyz, rgb, 0, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyw, rga, 0, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzx, rbr, 0, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzy, rbg, 0, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzz, rbb, 0, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzw, rba, 0, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwx, rar, 0, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwy, rag, 0, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwz, rab, 0, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xww, raa, 0, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxx, grr, 1, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxy, grg, 1, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxz, grb, 1, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxw, gra, 1, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyx, ggr, 1, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyy, ggg, 1, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyz, ggb, 1, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyw, gga, 1, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzx, gbr, 1, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzy, gbg, 1, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzz, gbb, 1, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzw, gba, 1, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywx, gar, 1, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywy, gag, 1, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywz, gab, 1, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yww, gaa, 1, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxx, brr, 2, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxy, brg, 2, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxz, brb, 2, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxw, bra, 2, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyx, bgr, 2, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyy, bgg, 2, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyz, bgb, 2, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyw, bga, 2, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzx, bbr, 2, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzy, bbg, 2, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzz, bbb, 2, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzw, bba, 2, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwx, bar, 2, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwy, bag, 2, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwz, bab, 2, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zww, baa, 2, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxx, arr, 3, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxy, arg, 3, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxz, arb, 3, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxw, ara, 3, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyx, agr, 3, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyy, agg, 3, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyz, agb, 3, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyw, aga, 3, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzx, abr, 3, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzy, abg, 3, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzz, abb, 3, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzw, aba, 3, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwx, aar, 3, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwy, aag, 3, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwz, aab, 3, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(www, aaa, 3, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxxx, rrrr, 0, 0, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxxy, rrrg, 0, 0, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxxz, rrrb, 0, 0, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxxw, rrra, 0, 0, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxyx, rrgr, 0, 0, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxyy, rrgg, 0, 0, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxyz, rrgb, 0, 0, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxyw, rrga, 0, 0, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxzx, rrbr, 0, 0, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxzy, rrbg, 0, 0, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxzz, rrbb, 0, 0, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxzw, rrba, 0, 0, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxwx, rrar, 0, 0, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxwy, rrag, 0, 0, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxwz, rrab, 0, 0, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xxww, rraa, 0, 0, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyxx, rgrr, 0, 1, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyxy, rgrg, 0, 1, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyxz, rgrb, 0, 1, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyxw, rgra, 0, 1, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyyx, rggr, 0, 1, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyyy, rggg, 0, 1, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyyz, rggb, 0, 1, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyyw, rgga, 0, 1, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyzx, rgbr, 0, 1, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyzy, rgbg, 0, 1, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyzz, rgbb, 0, 1, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyzw, rgba, 0, 1, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xywx, rgar, 0, 1, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xywy, rgag, 0, 1, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xywz, rgab, 0, 1, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xyww, rgaa, 0, 1, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzxx, rbrr, 0, 2, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzxy, rbrg, 0, 2, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzxz, rbrb, 0, 2, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzxw, rbra, 0, 2, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzyx, rbgr, 0, 2, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzyy, rbgg, 0, 2, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzyz, rbgb, 0, 2, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzyw, rbga, 0, 2, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzzx, rbbr, 0, 2, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzzy, rbbg, 0, 2, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzzz, rbbb, 0, 2, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzzw, rbba, 0, 2, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzwx, rbar, 0, 2, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzwy, rbag, 0, 2, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzwz, rbab, 0, 2, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xzww, rbaa, 0, 2, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwxx, rarr, 0, 3, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwxy, rarg, 0, 3, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwxz, rarb, 0, 3, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwxw, rara, 0, 3, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwyx, ragr, 0, 3, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwyy, ragg, 0, 3, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwyz, ragb, 0, 3, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwyw, raga, 0, 3, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwzx, rabr, 0, 3, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwzy, rabg, 0, 3, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwzz, rabb, 0, 3, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwzw, raba, 0, 3, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwwx, raar, 0, 3, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwwy, raag, 0, 3, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwwz, raab, 0, 3, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(xwww, raaa, 0, 3, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxxx, grrr, 1, 0, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxxy, grrg, 1, 0, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxxz, grrb, 1, 0, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxxw, grra, 1, 0, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxyx, grgr, 1, 0, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxyy, grgg, 1, 0, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxyz, grgb, 1, 0, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxyw, grga, 1, 0, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxzx, grbr, 1, 0, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxzy, grbg, 1, 0, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxzz, grbb, 1, 0, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxzw, grba, 1, 0, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxwx, grar, 1, 0, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxwy, grag, 1, 0, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxwz, grab, 1, 0, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yxww, graa, 1, 0, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyxx, ggrr, 1, 1, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyxy, ggrg, 1, 1, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyxz, ggrb, 1, 1, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyxw, ggra, 1, 1, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyyx, gggr, 1, 1, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyyy, gggg, 1, 1, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyyz, gggb, 1, 1, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyyw, ggga, 1, 1, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyzx, ggbr, 1, 1, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyzy, ggbg, 1, 1, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyzz, ggbb, 1, 1, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyzw, ggba, 1, 1, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yywx, ggar, 1, 1, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yywy, ggag, 1, 1, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yywz, ggab, 1, 1, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yyww, ggaa, 1, 1, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzxx, gbrr, 1, 2, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzxy, gbrg, 1, 2, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzxz, gbrb, 1, 2, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzxw, gbra, 1, 2, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzyx, gbgr, 1, 2, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzyy, gbgg, 1, 2, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzyz, gbgb, 1, 2, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzyw, gbga, 1, 2, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzzx, gbbr, 1, 2, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzzy, gbbg, 1, 2, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzzz, gbbb, 1, 2, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzzw, gbba, 1, 2, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzwx, gbar, 1, 2, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzwy, gbag, 1, 2, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzwz, gbab, 1, 2, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(yzww, gbaa, 1, 2, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywxx, garr, 1, 3, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywxy, garg, 1, 3, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywxz, garb, 1, 3, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywxw, gara, 1, 3, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywyx, gagr, 1, 3, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywyy, gagg, 1, 3, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywyz, gagb, 1, 3, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywyw, gaga, 1, 3, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywzx, gabr, 1, 3, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywzy, gabg, 1, 3, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywzz, gabb, 1, 3, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywzw, gaba, 1, 3, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywwx, gaar, 1, 3, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywwy, gaag, 1, 3, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywwz, gaab, 1, 3, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(ywww, gaaa, 1, 3, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxxx, brrr, 2, 0, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxxy, brrg, 2, 0, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxxz, brrb, 2, 0, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxxw, brra, 2, 0, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxyx, brgr, 2, 0, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxyy, brgg, 2, 0, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxyz, brgb, 2, 0, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxyw, brga, 2, 0, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxzx, brbr, 2, 0, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxzy, brbg, 2, 0, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxzz, brbb, 2, 0, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxzw, brba, 2, 0, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxwx, brar, 2, 0, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxwy, brag, 2, 0, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxwz, brab, 2, 0, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zxww, braa, 2, 0, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyxx, bgrr, 2, 1, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyxy, bgrg, 2, 1, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyxz, bgrb, 2, 1, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyxw, bgra, 2, 1, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyyx, bggr, 2, 1, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyyy, bggg, 2, 1, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyyz, bggb, 2, 1, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyyw, bgga, 2, 1, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyzx, bgbr, 2, 1, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyzy, bgbg, 2, 1, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyzz, bgbb, 2, 1, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyzw, bgba, 2, 1, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zywx, bgar, 2, 1, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zywy, bgag, 2, 1, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zywz, bgab, 2, 1, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zyww, bgaa, 2, 1, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzxx, bbrr, 2, 2, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzxy, bbrg, 2, 2, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzxz, bbrb, 2, 2, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzxw, bbra, 2, 2, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzyx, bbgr, 2, 2, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzyy, bbgg, 2, 2, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzyz, bbgb, 2, 2, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzyw, bbga, 2, 2, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzzx, bbbr, 2, 2, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzzy, bbbg, 2, 2, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzzz, bbbb, 2, 2, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzzw, bbba, 2, 2, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzwx, bbar, 2, 2, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzwy, bbag, 2, 2, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzwz, bbab, 2, 2, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zzww, bbaa, 2, 2, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwxx, barr, 2, 3, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwxy, barg, 2, 3, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwxz, barb, 2, 3, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwxw, bara, 2, 3, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwyx, bagr, 2, 3, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwyy, bagg, 2, 3, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwyz, bagb, 2, 3, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwyw, baga, 2, 3, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwzx, babr, 2, 3, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwzy, babg, 2, 3, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwzz, babb, 2, 3, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwzw, baba, 2, 3, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwwx, baar, 2, 3, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwwy, baag, 2, 3, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwwz, baab, 2, 3, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(zwww, baaa, 2, 3, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxxx, arrr, 3, 0, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxxy, arrg, 3, 0, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxxz, arrb, 3, 0, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxxw, arra, 3, 0, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxyx, argr, 3, 0, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxyy, argg, 3, 0, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxyz, argb, 3, 0, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxyw, arga, 3, 0, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxzx, arbr, 3, 0, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxzy, arbg, 3, 0, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxzz, arbb, 3, 0, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxzw, arba, 3, 0, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxwx, arar, 3, 0, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxwy, arag, 3, 0, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxwz, arab, 3, 0, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wxww, araa, 3, 0, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyxx, agrr, 3, 1, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyxy, agrg, 3, 1, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyxz, agrb, 3, 1, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyxw, agra, 3, 1, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyyx, aggr, 3, 1, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyyy, aggg, 3, 1, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyyz, aggb, 3, 1, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyyw, agga, 3, 1, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyzx, agbr, 3, 1, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyzy, agbg, 3, 1, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyzz, agbb, 3, 1, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyzw, agba, 3, 1, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wywx, agar, 3, 1, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wywy, agag, 3, 1, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wywz, agab, 3, 1, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wyww, agaa, 3, 1, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzxx, abrr, 3, 2, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzxy, abrg, 3, 2, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzxz, abrb, 3, 2, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzxw, abra, 3, 2, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzyx, abgr, 3, 2, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzyy, abgg, 3, 2, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzyz, abgb, 3, 2, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzyw, abga, 3, 2, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzzx, abbr, 3, 2, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzzy, abbg, 3, 2, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzzz, abbb, 3, 2, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzzw, abba, 3, 2, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzwx, abar, 3, 2, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzwy, abag, 3, 2, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzwz, abab, 3, 2, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wzww, abaa, 3, 2, 3, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwxx, aarr, 3, 3, 0, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwxy, aarg, 3, 3, 0, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwxz, aarb, 3, 3, 0, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwxw, aara, 3, 3, 0, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwyx, aagr, 3, 3, 1, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwyy, aagg, 3, 3, 1, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwyz, aagb, 3, 3, 1, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwyw, aaga, 3, 3, 1, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwzx, aabr, 3, 3, 2, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwzy, aabg, 3, 3, 2, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwzz, aabb, 3, 3, 2, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwzw, aaba, 3, 3, 2, 3);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwwx, aaar, 3, 3, 3, 0);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwwy, aaag, 3, 3, 3, 1);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwwz, aaab, 3, 3, 3, 2);
	STELLARLIB_LIN_MATRIX_SWIZZLE_DOUBLE_IMPL(wwww, aaaa, 3, 3, 3, 3);

private:
	template <std::size_t I>
	[[nodiscard]]
	constexpr auto elem() const noexcept
		requires (I < M * N && (M == 1 || N == 1))
	{
		return std::array<T, M * N>::operator[](I);
	}

	template <std::size_t I>
	[[nodiscard]]
	constexpr auto elem() noexcept
		-> auto &
		requires (I < M * N && (M == 1 || N == 1))
	{
		return std::array<T, M * N>::operator[](I);
	}

	template <std::size_t ...I>
	[[nodiscard]]
	constexpr auto swizzle() const noexcept
	{
		return matrix<T, 1, sizeof...(I)>{elem<I>()...};
	}
};

#define STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N>\
constexpr auto operator op (matrix<T, M, N> &self) noexcept\
	-> auto &\
{\
	for (auto &elem : self) {\
		op elem;\
	}\
\
	return self;\
}\
\
template <typename T, std::size_t M, std::size_t N>\
constexpr auto operator op (matrix<T, M, N> &self, int) noexcept\
	-> matrix<T, M, N>\
{\
	const auto res{self};\
	op self;\
	return res;\
}

#define STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &self) noexcept\
	-> matrix<T, M, N>\
{\
	matrix<T, M, N> res;\
\
	for (const auto [res, elem] : std::views::zip(res, self)) {\
		res = static_cast<T>(op elem);\
	}\
\
	return res;\
}

#define STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N, typename U>\
constexpr auto operator op##= (matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> auto &\
{\
	for (auto &lhs : lhs) {\
		lhs op##= rhs;\
	}\
\
	return lhs;\
}\
\
template <typename T, std::size_t M, std::size_t N, typename U>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	auto res{lhs};\
	res op##= rhs;\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const T lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	matrix<std::common_type_t<T, U>, M, N> res;\
\
	for (const auto [res, rhs] : std::views::zip(res, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
constexpr auto operator op##= (matrix<T, M, N> &lhs, const matrix<U, N, M> &rhs) noexcept\
	-> auto &\
	requires (M == 1 || N == 1)\
{\
	for (const auto [lhs, rhs] : std::views::zip(lhs, rhs)) {\
		lhs op##= rhs;\
	}\
\
	return lhs;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
constexpr auto operator op##= (matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> auto &\
{\
	for (const auto [lhs, rhs] : std::views::zip(lhs, rhs)) {\
		lhs op##= rhs;\
	}\
\
	return lhs;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, N, M> &rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
	requires (M == 1 || N == 1)\
{\
	auto res{lhs};\
	res op##= rhs;\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<std::common_type_t<T, U>, M, N>\
{\
	auto res{lhs};\
	res op##= rhs;\
	return res;\
}

#define STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(op)\
template <typename T, std::size_t M, std::size_t N, typename U>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const U rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, lhs] : std::views::zip(res, lhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const T lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, rhs] : std::views::zip(res, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, N, M> &rhs) noexcept\
	-> matrix<bool, M, N>\
	requires (M == 1 || N == 1)\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, lhs, rhs] : std::views::zip(res, lhs, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}\
\
template <typename T, typename U, std::size_t M, std::size_t N>\
[[nodiscard]]\
constexpr auto operator op (const matrix<T, M, N> &lhs, const matrix<U, M, N> &rhs) noexcept\
	-> matrix<bool, M, N>\
{\
	matrix<bool, M, N> res;\
\
	for (const auto [res, lhs, rhs] : std::views::zip(res, lhs, rhs)) {\
		res = lhs op rhs;\
	}\
\
	return res;\
}

STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(++);
STELLARLIB_LIN_MATRIX_PREFIX_POSTFIX_OPERATOR_IMPL(--);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(+);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(-);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(!);
STELLARLIB_LIN_MATRIX_UNARY_OPERATOR_IMPL(~);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(*);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(/);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(%);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(+);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(-);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(<<);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(>>);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(<);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(<=);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(>);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(>=);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(==);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(!=);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(&);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(^);
STELLARLIB_LIN_MATRIX_BINARY_OPERATOR_IMPL(|);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(&&);
STELLARLIB_LIN_MATRIX_BOOLEAN_OPERATOR_IMPL(||);
}

#endif
