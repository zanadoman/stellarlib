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

/**
 * @brief Resource management utilities
 */
namespace stellarlib::res
{
/**
 * @brief Image resource
 */
class [[nodiscard]] image final
{
public:
	/**
	 * @brief Addressing mode
	 */
	enum class [[nodiscard]] address_mode : std::uint8_t
	{
		repeat,
		mirrored_repeat,
		clamp_to_edge
	};

	/**
	 * @brief Filtering method
	 */
	enum class [[nodiscard]] filter : std::uint8_t
	{
		nearest,
		linear
	};

	/**
	 * @brief Blending factor
	 */
	enum class [[nodiscard]] blend_factor : std::uint8_t
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

	/**
	 * @brief Blending operation
	 */
	enum class [[nodiscard]] blend_op : std::uint8_t
	{
		add,
		subtract,
		reverse_subtract,
		min,
		max
	};

	/**
	 * @brief Blend state
	 */
	struct [[nodiscard]] blend_state final
	{
		/**
		 * @brief Source color blending factor
		 */
		blend_factor src_color_blend_factor;

		/**
		 * @brief Destination color blending factor
		 */
		blend_factor dst_color_blend_factor;

		/**
		 * @brief Color blending operator
		 */
		blend_op color_blend_op;

		/**
		 * @brief Source alpha blending factor
		 */
		blend_factor src_alpha_blend_factor;

		/**
		 * @brief Destination alpha blending factor
		 */
		blend_factor dst_alpha_blend_factor;

		/**
		 * @brief Alpha blending operator
		 */
		blend_op alpha_blend_op;
	};

	/**
	 * @brief Pixel format of the image
	 */
	static constexpr auto format{SDL_PIXELFORMAT_ABGR8888};

	/**
	 * @brief Constructs a zero initialized image with the specified size
	 * @param size Size of the image
	 */
	[[nodiscard]]
	explicit image(lin::uint2 size);

	/**
	 * @brief Constructs an image from the specified size and data
	 * @param size Size of the image
	 * @param range Data of the image
	 */
	template <std::ranges::input_range R>
	[[nodiscard]]
	constexpr image(const lin::uint2 size, R &&range)
		: image{size}
	{
		clear(std::forward<R>(range));
	}

	/**
	 * @brief Constructs an image from a PNG image
	 * @param path Path of the PNG image
	 */
	[[nodiscard]]
	explicit image(const std::filesystem::path &path);

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	image(const image &other);

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	image(image &&other);

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(const image &other)
		-> image &;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(image &&other)
		-> image &;

	/**
	 * @brief Destructor
	 */
	~image();

	/**
	 * @brief Returns a pointer to the internal handle
	 * @return Pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit operator SDL_Surface *() const;

	/**
	 * @brief Returns the size of the image
	 * @return Size of the image
	 */
	[[nodiscard]]
	auto size() const
		-> lin::uint2;

	/**
	 * @brief Returns the pixel at the specified coordinate
	 * @param coord Coordinate of the pixel
	 * @return The pixel at the specified coordinate
	 */
	[[nodiscard]]
	auto operator[](lin::uint2 coord) const
		-> const lin::uchar4 &;

	/**
	 * @brief Returns the pixel at the specified coordinate
	 * @param coord Coordinate of the pixel
	 * @return The pixel at the specified coordinate
	 */
	[[nodiscard]]
	auto operator[](lin::uint2 coord)
		-> lin::uchar4 &;

	/**
	 * @brief Returns the bytes of the image
	 * @return Bytes of the image
	 */
	[[nodiscard]]
	auto bytes() const
		-> std::span<const std::uint8_t>;

	/**
	 * @brief Returns the bytes of the image
	 * @return Bytes of the image
	 */
	[[nodiscard]]
	auto bytes()
		-> std::span<std::uint8_t>;

	/**
	 * @brief Returns the pixels of the image
	 * @return Pixels of the image
	 */
	[[nodiscard]]
	auto pixels() const
		-> std::span<const lin::uchar4>;

	/**
	 * @brief Returns the pixels of the image
	 * @return Pixels of the image
	 */
	[[nodiscard]]
	auto pixels()
		-> std::span<lin::uchar4>;

	/**
	 * @brief Samples a color from the image
	 * @param uv UV coordinate
	 * @param address_mode Addressing mode
	 * @param filter Filtering method
	 * @return Sampled color from the image
	 */
	[[nodiscard]]
	auto sample(lin::float2 uv, address_mode address_mode, filter filter) const
		-> lin::float4;

	/**
	 * @brief Blends a color into the image
	 * @param coord Pixel coordinate
	 * @param src Source color
	 * @param blend_state Optional blend state
	 */
	void blend(lin::uint2 coord, const lin::float4 &src, const std::optional<blend_state> &blend_state);

	/**
	 * @brief Comparison operator
	 * @param other Other instance
	 * @return Result of the comparison
	 */
	[[nodiscard]]
	auto operator==(const image &other) const
		-> bool;

	/**
	 * @brief Saves the image as a PNG image
	 * @param path Path of the PNG image
	 */
	void save(const std::filesystem::path &path) const;

	/**
	 * @brief Clears the image with the provided data
	 * @param range Data of the image
	 */
	template <std::ranges::input_range R>
	constexpr void clear(R &&range)
	{
		for (const auto [dst, src] : std::views::zip(std::span{static_cast<std::ranges::range_value_t<R> *>(_handle->pixels), bytes().size() / sizeof(std::ranges::range_value_t<R>)}, std::forward<R>(range))) {
			std::memcpy(std::addressof(dst), std::addressof(src), sizeof(std::ranges::range_value_t<R>));
		}
	}

private:
	SDL_Surface *_handle{};
};
}

#endif
