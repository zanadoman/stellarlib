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

#ifndef STELLARLIB_GFX_RENDERER_HPP
#define STELLARLIB_GFX_RENDERER_HPP

#include <stellarlib/ext/type_traits.hpp>
#include <stellarlib/gfx/pods.hpp>
#include <stellarlib/gfx/texture.hpp>
#include <stellarlib/lin/lin.hpp>
#include <stellarlib/res/res.hpp>

#include <SDL3/SDL_gpu.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

/**
 * @brief Graphics and rendering abstractions
 */
namespace stellarlib::gfx
{
/**
 * @brief Common renderer interface
 * @tparam Self Implementation provider
 */
template <typename Self>
class [[nodiscard]] renderer
{
friend Self;

public:
	/**
	 * @brief Presentation method
	 */
	enum class [[nodiscard]] presentation : std::uint8_t
	{
		letterbox,
		stretch
	};

	/**
	 * @brief Renderer initialization descriptor
	 */
	struct [[nodiscard]] info final
	{
		/**
		 * @brief Minimum framebuffer aspect ratio
		 */
		std::optional<float> min_aspect;

		/**
		 * @brief Maximum framebuffer aspect ratio
		 */
		std::optional<float> max_aspect;

		/**
		 * @brief Maximum framebuffer resolution
		 */
		std::optional<lin::uint2> max_resolution;

		/**
		 * @brief Framebuffer filtering method
		 */
		res::image::filter filter;

		/**
		 * @brief Framebuffer presentation method
		 */
		enum presentation presentation;

		/**
		 * @brief Enable synchronization
		 */
		bool vsync;

		/**
		 * @brief Enable validation
		 */
		bool validation;
	};

	/**
	 * @brief Blit operation descriptor
	 */
	struct [[nodiscard]] blit_info final : ext::pin
	{
		/**
		 * @brief Source texture
		 */
		const texture &src_texture;

		/**
		 * @brief Source area
		 */
		aabb<float> src_area;

		/**
		 * @brief Destination texture
		 */
		texture &dst_texture;

		/**
		 * @brief Destination area
		 */
		aabb<float> dst_area;

		/**
		 * @brief Filtering method
		 */
		res::image::filter filter;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 7> padding;
	};

	/**
	 * @brief Returns a reference to the internal handle
	 * @return Reference to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit constexpr operator SDL_GPUDevice &() const
	{
		return static_cast<SDL_GPUDevice &>(static_cast<const Self &>(*this));
	}

	/**
	 * @brief Returns a shared pointer to the internal handle
	 * @return Shared pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit constexpr operator std::shared_ptr<SDL_GPUDevice>() const
	{
		return static_cast<std::shared_ptr<SDL_GPUDevice>>(static_cast<const Self &>(*this));
	}

	/**
	 * @brief Returns the minimum framebuffer aspect ratio
	 * @return Minimum framebuffer aspect ratio
	 */
	[[nodiscard]]
	constexpr auto min_aspect() const
		-> std::optional<float>
	{
		return static_cast<const Self &>(*this).min_aspect();
	}

	/**
	 * @brief Sets the minimum framebuffer aspect ratio
	 * @param min_aspect Minimum framebuffer aspect ratio
	 */
	constexpr void set_min_aspect(std::optional<float> min_aspect)
	{
		static_cast<Self &>(*this).set_min_aspect(min_aspect);
	}

	/**
	 * @brief Returns the maximum framebuffer aspect ratio
	 * @return Maximum framebuffer aspect ratio
	 */
	[[nodiscard]]
	constexpr auto max_aspect() const
		-> std::optional<float>
	{
		return static_cast<const Self &>(*this).max_aspect();
	}

	/**
	 * @brief Sets the maximum framebuffer aspect ratio
	 * @param max_aspect Maximum framebuffer aspect ratio
	 */
	constexpr void set_max_aspect(std::optional<float> max_aspect)
	{
		static_cast<Self &>(*this).set_max_aspect(max_aspect);
	}

	/**
	 * @brief Returns the maximum framebuffer resolution
	 * @return Maximum framebuffer resolution
	 */
	[[nodiscard]]
	constexpr auto max_resolution() const
		-> const std::optional<lin::uint2> &
	{
		return static_cast<const Self &>(*this).max_resolution();
	}

	/**
	 * @brief Sets the maximum framebuffer resolution
	 * @param max_resolution Maximum framebuffer resolution
	 */
	constexpr void set_max_resolution(const std::optional<lin::uint2> &max_resolution)
	{
		static_cast<Self &>(*this).set_max_resolution(max_resolution);
	}

	/**
	 * @brief Returns the framebuffer filtering method
	 * @return Framebuffer filtering method
	 */
	[[nodiscard]]
	constexpr auto filter() const
		-> res::image::filter
	{
		return static_cast<const Self &>(*this).filter();
	}

	/**
	 * @brief Sets the framebuffer filtering method
	 * @param filter Framebuffer filtering method
	 */
	constexpr void set_filter(res::image::filter filter)
	{
		static_cast<Self &>(*this).set_filter(filter);
	}

	/**
	 * @brief Returns the framebuffer presentation method
	 * @return Framebuffer presentation method
	 */
	[[nodiscard]]
	constexpr auto presentation() const
		-> enum presentation
	{
		return static_cast<const Self &>(*this).presentation();
	}

	/**
	 * @brief Sets the framebuffer presentation method
	 * @param presentation Framebuffer presentation method
	 */
	constexpr void set_presentation(enum presentation presentation)
	{
		static_cast<Self &>(*this).set_presentation(presentation);
	}

	/**
	 * @brief Returns whether synchronization is enabled
	 * @return Whether synchronization is enabled
	 */
	[[nodiscard]]
	constexpr auto vsync() const
		-> bool
	{
		return static_cast<const Self &>(*this).vsync();
	}

	/**
	 * @brief Sets whether synchronization is enabled
	 * @param vsync Whether synchronization is enabled
	 */
	constexpr void set_vsync(bool vsync)
	{
		static_cast<Self &>(*this).set_vsync(vsync);
	}

	/**
	 * @brief Uploads an image
	 * @param image Image to upload
	 * @param mipmaps Enable mipmap generation
	 * @return Uploaded image
	 */
	[[nodiscard]]
	constexpr auto upload_image(const res::image &image, bool mipmaps)
		-> texture
	{
		return static_cast<Self &>(*this).upload_image(image, mipmaps);
	}

	/**
	 * @brief Blits from a texture into another
	 * @param info Operation descriptor
	 * @param idle Wait for idle
	 */
	constexpr void blit_texture(const blit_info &info, bool idle)
	{
		static_cast<Self &>(*this).blit_texture(info, idle);
	}

	/**
	 * @brief Downloads a texture
	 * @param texture Texture to download
	 * @param idle Wait for idle
	 * @return Downloaded texture
	 */
	[[nodiscard]]
	constexpr auto download_texture(const texture &texture, bool idle)
		-> res::image
	{
		return static_cast<Self &>(*this).download_texture(texture, idle);
	}

	/**
	 * @brief Returns a reference to the virtual framebuffer
	 * @return Reference to the virtual framebuffer
	 */
	[[nodiscard]]
	constexpr auto framebuffer() const
		-> const std::optional<texture> &
	{
		return static_cast<const Self &>(*this).framebuffer();
	}

	/**
	 * @brief Returns a reference to the virtual framebuffer
	 * @return Reference to the virtual framebuffer
	 */
	[[nodiscard]]
	constexpr auto framebuffer()
		-> std::optional<texture> &
	{
		return static_cast<Self &>(*this).framebuffer();
	}

private:
	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	constexpr renderer() noexcept = default;

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr renderer(const renderer &other) noexcept = default;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr renderer(renderer &&other) noexcept = default;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(const renderer &other) noexcept
		-> renderer & = default;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(renderer &&other) noexcept
		-> renderer & = default;

	/**
	 * @brief Destructor
	 */
	constexpr ~renderer() noexcept = default;
};
}

#endif
