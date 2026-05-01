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

#ifndef STELLARLIB_APP_SCENE_HPP
#define STELLARLIB_APP_SCENE_HPP

#include <memory>
#include <optional>

/**
 * @brief Application runtime
 */
namespace stellarlib::app
{
/**
 * @brief Application scene interface
 */
class scene
{
friend class context;

public:
	/**
	 * @brief Destructor
	 */
	virtual ~scene() noexcept = 0;

protected:
	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	scene() noexcept;

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	scene(const scene &other) noexcept;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	scene(scene &&other) noexcept;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(const scene &other) noexcept
		-> scene &;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(scene &&other) noexcept
		-> scene &;

	/**
	 * @brief Called after the previous scene has finished deactivation and before the first update
	 * @param ctx Global application context
	 */
	virtual void begin(class context &ctx);

	/**
	 * @brief Called once per frame
	 * @param ctx Global application context
	 * @return std::nullopt to continue execution, a scene instance to request a transition or nullptr to terminate the application
	 */
	[[nodiscard]]
	virtual constexpr auto update(class context &ctx)
		-> std::optional<std::unique_ptr<scene>> = 0;

	/**
	 * @brief Called after the last update and before the next scene begins activation
	 * @param ctx Global application context
	 */
	virtual void end(class context &ctx);
};
}

#endif
