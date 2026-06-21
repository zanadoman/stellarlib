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
 */
class [[nodiscard]] renderer
{
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
		 * @brief Enable debugging
		 */
		bool debug;
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
		 * @brief Source mip level
		 */
		std::uint32_t src_level;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 4> padding1;

		/**
		 * @brief Destination texture
		 */
		texture &dst_texture;

		/**
		 * @brief Destination area
		 */
		aabb<float> dst_area;

		/**
		 * @brief Destination mip level
		 */
		std::uint32_t dst_level;

		/**
		 * @brief Filtering method
		 */
		res::image::filter filter;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 3> padding2;
	};

	/**
	 * @brief Destructor
	 */
	virtual ~renderer() noexcept;

	/**
	 * @brief Returns a pointer to the internal handle
	 * @return Pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	virtual explicit constexpr operator SDL_GPUDevice *() const = 0;

	/**
	 * @brief Returns a shared pointer to the internal handle
	 * @return Shared pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	virtual explicit constexpr operator std::shared_ptr<SDL_GPUDevice>() const = 0;

	/**
	 * @brief Returns the minimum framebuffer aspect ratio
	 * @return Minimum framebuffer aspect ratio
	 */
	[[nodiscard]]
	virtual constexpr auto min_aspect() const
		-> std::optional<float> = 0;

	/**
	 * @brief Sets the minimum framebuffer aspect ratio
	 * @param min_aspect Minimum framebuffer aspect ratio
	 */
	virtual void set_min_aspect(std::optional<float> min_aspect);

	/**
	 * @brief Returns the maximum framebuffer aspect ratio
	 * @return Maximum framebuffer aspect ratio
	 */
	[[nodiscard]]
	virtual constexpr auto max_aspect() const
		-> std::optional<float> = 0;

	/**
	 * @brief Sets the maximum framebuffer aspect ratio
	 * @param max_aspect Maximum framebuffer aspect ratio
	 */
	virtual void set_max_aspect(std::optional<float> max_aspect);

	/**
	 * @brief Returns the maximum resolution of the framebuffer
	 * @return Maximum resolution of the framebuffer
	 */
	[[nodiscard]]
	virtual constexpr auto max_resolution() const
		-> const std::optional<lin::uint2> & = 0;

	/**
	 * @brief Sets the maximum resolution of the framebuffer
	 * @param max_resolution Maximum resolution of the framebuffer
	 */
	virtual void set_max_resolution(const std::optional<lin::uint2> &max_resolution);

	/**
	 * @brief Returns the framebuffer filtering method
	 * @return Framebuffer filtering method
	 */
	[[nodiscard]]
	virtual constexpr auto filter() const
		-> res::image::filter = 0;

	/**
	 * @brief Sets the framebuffer filtering method
	 * @param filter Framebuffer filtering method
	 */
	virtual void set_filter(res::image::filter filter);

	/**
	 * @brief Returns the framebuffer presentation method
	 * @return Framebuffer presentation method
	 */
	[[nodiscard]]
	virtual constexpr auto presentation() const
		-> enum presentation = 0;

	/**
	 * @brief Sets the framebuffer presentation method
	 * @param presentation Framebuffer presentation method
	 */
	virtual void set_presentation(enum presentation presentation);

	/**
	 * @brief Returns whether synchronization is enabled
	 * @return Whether synchronization is enabled
	 */
	[[nodiscard]]
	virtual constexpr auto vsync() const
		-> bool = 0;

	/**
	 * @brief Sets whether synchronization is enabled
	 * @param vsync Whether synchronization is enabled
	 */
	virtual void set_vsync(bool vsync);

	/**
	 * @brief Uploads an image
	 * @param image Image to upload
	 * @param mipmaps Enable mipmap generation
	 * @return Uploaded image
	 */
	[[nodiscard]]
	virtual constexpr auto upload_image(const res::image &image, bool mipmaps)
		-> texture = 0;

	/**
	 * @brief Blits from a texture into another
	 * @param info Operation descriptor
	 * @param idle Wait for idle
	 */
	virtual void blit_texture(const blit_info &info, bool idle);

	/**
	 * @brief Downloads a texture
	 * @param texture Texture to download
	 * @param idle Wait for idle
	 * @return Downloaded texture
	 */
	[[nodiscard]]
	virtual constexpr auto download_texture(const texture &texture, bool idle)
		-> res::image = 0;

	/**
	 * @brief Returns a reference to the virtual framebuffer
	 * @return Reference to the virtual framebuffer
	 */
	[[nodiscard]]
	virtual constexpr auto framebuffer() const
		-> const std::optional<texture> & = 0;

	/**
	 * @brief Returns a reference to the virtual framebuffer
	 * @return Reference to the virtual framebuffer
	 */
	[[nodiscard]]
	virtual constexpr auto framebuffer()
		-> std::optional<texture> & = 0;

protected:
	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	renderer() noexcept;

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	renderer(const renderer &other) noexcept;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	renderer(renderer &&other) noexcept;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(const renderer &other) noexcept
		-> renderer &;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(renderer &&other) noexcept
		-> renderer &;
};
}

#endif
