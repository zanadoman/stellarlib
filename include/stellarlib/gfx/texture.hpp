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

#ifndef STELLARLIB_GFX_TEXTURE_HPP
#define STELLARLIB_GFX_TEXTURE_HPP

#include <SDL3/SDL_gpu.h>

#include <memory>

/**
 * @brief Graphics and rendering abstractions
 */
namespace stellarlib::gfx
{
/**
 * @brief Texture resource
 */
class [[nodiscard]] texture final
{
public:
	/**
	 * @brief Parameterized constructor
	 * @param device Device of the texture
	 * @param info Creation info of the texture
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	texture(const std::shared_ptr<SDL_GPUDevice> &device, const SDL_GPUTextureCreateInfo &info);

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr texture(const texture &) noexcept = delete;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	texture(texture &&other);

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const texture &) noexcept
		-> texture & = delete;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(texture &&other)
		-> texture &;

	/**
	 * @brief Destructor
	 */
	~texture();

	/**
	 * @brief Returns a pointer to the internal handle
	 * @return Pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit operator SDL_GPUTexture *() const;

	/**
	 * @brief Returns a pointer to the internal device
	 * @return Pointer to the internal device
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit operator const SDL_GPUDevice *() const;

private:
	SDL_GPUTexture *_handle{};
	std::shared_ptr<SDL_GPUDevice> _device;
};
}

#endif
