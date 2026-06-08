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

#include <stellarlib/gfx/texture.hpp>
#include <stellarlib/res/res.hpp>

#include <SDL3/SDL_gpu.h>

#include <memory>

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
	 * @brief Renderer initialization descriptor
	 */
	struct [[nodiscard]] info final
	{
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
	 * @brief Downloads a texture
	 * @param texture Texture to download
	 * @param idle Wait for idle
	 * @return Downloaded texture
	 */
	[[nodiscard]]
	virtual constexpr auto download_texture(const texture &texture, bool idle)
		-> res::image = 0;

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
