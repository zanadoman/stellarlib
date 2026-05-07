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

#ifndef STELLARLIB_RES_IMAGE_HPP
#define STELLARLIB_RES_IMAGE_HPP

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <utility>

namespace stellarlib::res
{
class image final
{
public:
	enum class address_mode : std::uint8_t
	{
		repeat,
		mirrored_repeat,
		clamp_to_edge
	};

	enum class filter : std::uint8_t
	{
		nearest,
		linear
	};

	enum class blend_factor : std::uint8_t
	{
		zero,
		one,
		src_color,
		one_minus_src_color,
		dst_color,
		one_minus_dst_color,
		src_alpha,
		one_minus_src_alpha,
		dst_alpha,
		one_minus_dst_alpha,
		src_alpha_saturate
	};

	enum class blend_op : std::uint8_t
	{
		add,
		subtract,
		reverse_subtract,
		min,
		max
	};

	struct blend_state final
	{
		blend_factor src_color_blend_factor;
		blend_factor dst_color_blend_factor;
		blend_op color_blend_op;
		blend_factor src_alpha_blend_factor;
		blend_factor dst_alpha_blend_factor;
		blend_op alpha_blend_op;
	};

	static constexpr auto format{SDL_PIXELFORMAT_ABGR8888};

	[[nodiscard]]
	explicit image(lin::uint2 size);

	template <std::ranges::input_range R>
	[[nodiscard]]
	constexpr image(const lin::uint2 size, R &&range)
		: image{size}
	{
		clear(std::forward<R>(range));
	}

	[[nodiscard]]
	explicit image(const std::filesystem::path &path);

	[[nodiscard]]
	image(const image &other);

	[[nodiscard]]
	image(image &&other) noexcept;

	auto operator=(const image &other)
		-> image &;

	auto operator=(image &&other) noexcept
		-> image &;

	~image();

	[[nodiscard]]
	explicit operator SDL_Surface *() const;

	[[nodiscard]]
	auto size() const
		-> lin::uint2;

	[[nodiscard]]
	auto operator[](lin::uint2 coord) const
		-> const lin::uchar4 &;

	[[nodiscard]]
	auto operator[](lin::uint2 coord)
		-> lin::uchar4 &;

	[[nodiscard]]
	auto bytes() const
		-> std::span<const std::uint8_t>;

	[[nodiscard]]
	auto bytes()
		-> std::span<std::uint8_t>;

	[[nodiscard]]
	auto pixels() const
		-> std::span<const lin::uchar4>;

	[[nodiscard]]
	auto pixels()
		-> std::span<lin::uchar4>;

	[[nodiscard]]
	auto sample(lin::float2 uv, address_mode address_mode, filter filter) const
		-> lin::float4;

	void blend(lin::uint2 coord, const lin::float4 &src, const std::optional<blend_state> &blend_state);

	[[nodiscard]]
	auto operator==(const image &other) const
		-> bool;

	void save(const std::filesystem::path &path) const;

	template <std::ranges::input_range R>
	constexpr void clear(R &&range)
	{
		for (const auto [dst, src] : std::views::zip(std::span{static_cast<std::ranges::range_value_t<R> *>(_handle->pixels), bytes().size() / sizeof(std::ranges::range_value_t<R>)}, std::forward<R>(range))) {
			std::memcpy(std::addressof(dst), std::addressof(src), sizeof(std::ranges::range_value_t<R>));
		}
	}

private:
	std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> _handle;
};
}

#endif
