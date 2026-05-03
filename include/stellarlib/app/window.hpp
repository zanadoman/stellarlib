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

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

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
class window final
{
friend internal::lifecycle<class context>;

public:
	/**
	 * @brief Window initialization descriptor
	 */
	struct info final
	{
		/**
		 * @brief Title of the window
		 */
		std::string title;
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

private:
	std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> _handle{nullptr, nullptr};

	[[nodiscard]]
	explicit window(const info &info);

	void iterate();

	void event(const SDL_Event &event) const;
};
}

#endif
