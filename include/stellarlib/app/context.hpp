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

#ifndef STELLARLIB_APP_CONTEXT_HPP
#define STELLARLIB_APP_CONTEXT_HPP

#include <stellarlib/app/clock.hpp>
#include <stellarlib/app/lifecycle.hpp>
#include <stellarlib/app/metadata.hpp>
#include <stellarlib/app/scene.hpp>
#include <stellarlib/app/window.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <variant>

/**
 * @brief Application runtime
 */
namespace stellarlib::app
{
/**
 * @brief Application context
 */
class context final
{
friend internal::lifecycle<class main>;

public:
	/**
	 * @brief Context initialization descriptor
	 */
	struct info final
	{
		/**
		 * @brief Metadata initialization descriptor
		 */
		app::metadata::info metadata;

		/**
		 * @brief Clock initialization descriptor
		 */
		app::clock::info clock;

		/**
		 * @brief Window initialization descriptor
		 */
		app::window::info window;

		/**
		 * @brief Explicit padding
		 */
		std::uint64_t padding;

		/**
		 * @brief Initial scene, application termination, or deferred scene factory
		 */
		std::variant<std::unique_ptr<scene>, std::function<std::unique_ptr<scene> (context &)>> main;
	};

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr context(const context &) noexcept = delete;

	/**
	 * @brief Deleted move constructor
	 */
	[[nodiscard]]
	constexpr context(context &&) noexcept = delete;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const context &) noexcept
		-> context & = delete;

	/**
	 * @brief Deleted move assignment operator
	 */
	constexpr auto operator=(context &&) noexcept
		-> context & = delete;

	/**
	 * @brief Destructor
	 */
	~context();

	/**
	 * @brief Returns a reference to the metadata subsystem
	 * @return Reference to the metadata subsystem
	 */
	[[nodiscard]]
	auto metadata() const
		-> const app::metadata &;

	/**
	 * @brief Returns a reference to the metadata subsystem
	 * @return Reference to the metadata subsystem
	 */
	[[nodiscard]]
	auto metadata()
		-> app::metadata &;

	/**
	 * @brief Returns a reference to the clock subsystem
	 * @return Reference to the clock subsystem
	 */
	[[nodiscard]]
	auto clock() const
		-> const app::clock &;

	/**
	 * @brief Returns a reference to the clock subsystem
	 * @return Reference to the clock subsystem
	 */
	[[nodiscard]]
	auto clock()
		-> app::clock &;

	/**
	 * @brief Returns a reference to the window subsystem
	 * @return Reference to the window subsystem
	 */
	[[nodiscard]]
	auto window() const
		-> const app::window &;

	/**
	 * @brief Returns a reference to the window subsystem
	 * @return Reference to the window subsystem
	 */
	[[nodiscard]]
	auto window()
		-> app::window &;

	/**
	 * @brief Returns a reference to the ECS subsystem
	 * @return Reference to the ECS subsystem
	 */
	[[nodiscard]]
	auto world() const
		-> const ecs::world &;

	/**
	 * @brief Returns a reference to the ECS subsystem
	 * @return Reference to the ECS subsystem
	 */
	[[nodiscard]]
	auto world()
		-> ecs::world &;

private:
	[[no_unique_address]] app::metadata _metadata;
	app::clock _clock;
	std::uint32_t _padding1;
	app::window _window;
	ecs::world _world;
	std::unique_ptr<scene> _scene;
	std::uint64_t _padding2;

	[[nodiscard]]
	explicit context(info info);

	[[nodiscard]]
	auto iterate()
		-> SDL_AppResult;

	[[nodiscard]]
	auto event(const SDL_Event &event) const
		-> SDL_AppResult;
};
}

#endif
