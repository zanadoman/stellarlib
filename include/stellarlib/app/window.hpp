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

#ifndef STELLARLIB_APP_WINDOW_HPP
#define STELLARLIB_APP_WINDOW_HPP

#include <stellarlib/app/lifecycle.hpp>
#include <stellarlib/gfx/gfx.hpp>
#include <stellarlib/res/res.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

/**
 * @brief Application runtime
 */
namespace stellarlib::app
{
/**
 * @brief Window subsystem
 */
class [[nodiscard]] window final
{
friend internal::lifecycle<class context>;

public:
	/**
	 * @brief Window initialization descriptor
	 */
	struct [[nodiscard]] info final
	{
		/**
		 * @brief Title of the window
		 */
		std::string title;

		/**
		 * @brief Icon of the window
		 */
		res::image icon;

		/**
		 * @brief Enable GPU synchronization
		 */
		bool vsync;

		/**
		 * @brief Enable GPU debugging
		 */
		bool debug;

		/**
		 * @brief Explicit padding
		 */
		std::array<std::byte, 6> padding;
	};

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr window(const window &) noexcept = delete;

	/**
	 * @brief Deleted move constructor
	 */
	[[nodiscard]]
	constexpr window(window &&) noexcept = delete;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const window &) noexcept
		-> window & = delete;

	/**
	 * @brief Deleted move assignment operator
	 */
	constexpr auto operator=(window &&) noexcept
		-> window & = delete;

	/**
	 * @brief Destructor
	 */
	~window();

	/**
	 * @brief Returns the title of the window
	 * @return Title of the window
	 */
	[[nodiscard]]
	auto title() const
		-> std::string;

	/**
	 * @brief Sets the title of the window
	 * @param title Title of the window
	 */
	void set_title(const std::string &title);

	/**
	 * @brief Returns whether GPU synchronization is enabled
	 * @return Whether GPU synchronization is enabled
	 */
	[[nodiscard]]
	auto vsync() const
		-> bool;

	/**
	 * @brief Sets whether GPU synchronization is enabled
	 * @param vsync Whether GPU synchronization is enabled
	 */
	void set_vsync(bool vsync);

	/**
	 * @brief Uploads an image into the GPU
	 * @param image Image to upload
	 * @param mipmaps Enable mipmap generation
	 * @return Uploaded image
	 */
	auto upload_image(const res::image &image, bool mipmaps)
		-> gfx::texture;

private:
	SDL_Window *_handle{};
	std::shared_ptr<SDL_GPUDevice> _device;
	bool _vsync{true};
	[[maybe_unused]] std::array<std::byte, 3> _padding;
	std::uint32_t _transbuf_size{};
	SDL_GPUTransferBuffer *_transbuf{};
	SDL_GPUFence *_fence{};

	[[nodiscard]]
	explicit window(const info &info);

	void iterate();

	void event(const SDL_Event &event) const;
};
}

#endif
