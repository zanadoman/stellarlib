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
#include <stellarlib/ext/type_map.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <array>
#include <cstddef>
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
class [[nodiscard]] context final
{
friend internal::lifecycle<class host>;

public:
	/**
	 * @brief Context initialization descriptor
	 */
	struct [[nodiscard]] info final
	{
		/**
		 * @brief Metadata initialization descriptor
		 */
		metadata::info metadata;

		/**
		 * @brief Clock initialization descriptor
		 */
		clock::info clock;

		/**
		 * @brief Window initialization descriptor
		 */
		window::info window;

		/**
		 * @brief Initial scene, application termination, or deferred scene factory
		 */
		std::variant<std::unique_ptr<scene>, std::function<std::unique_ptr<scene> (context &)>> entry;
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
		-> const class metadata &;

	/**
	 * @brief Returns a reference to the metadata subsystem
	 * @return Reference to the metadata subsystem
	 */
	[[nodiscard]]
	auto metadata()
		-> class metadata &;

	/**
	 * @brief Returns a reference to the clock subsystem
	 * @return Reference to the clock subsystem
	 */
	[[nodiscard]]
	auto clock() const
		-> const class clock &;

	/**
	 * @brief Returns a reference to the clock subsystem
	 * @return Reference to the clock subsystem
	 */
	[[nodiscard]]
	auto clock()
		-> class clock &;

	/**
	 * @brief Returns a reference to the window subsystem
	 * @return Reference to the window subsystem
	 */
	[[nodiscard]]
	auto window() const
		-> const class window &;

	/**
	 * @brief Returns a reference to the window subsystem
	 * @return Reference to the window subsystem
	 */
	[[nodiscard]]
	auto window()
		-> class window &;

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

	/**
	 * @brief Returns a reference to the global store
	 * @return Reference to the global store
	 */
	[[nodiscard]]
	auto store() const
		-> const ext::type_map<void, context> &;

	/**
	 * @brief Returns a reference to the global store
	 * @return Reference to the global store
	 */
	[[nodiscard]]
	auto store()
		-> ext::type_map<void, context> &;

private:
	[[no_unique_address]] class metadata _metadata;
	class clock _clock;
	[[maybe_unused]] std::array<std::byte, 4> _padding1;
	class window _window;
	[[maybe_unused]] std::array<std::byte, 8> _padding2;
	ecs::world _world{};
	ext::type_map<void, context> _store{};
	scene *_scene{};

	[[nodiscard]]
	explicit context(info info);

	[[nodiscard]]
	auto iterate()
		-> SDL_AppResult;

	[[nodiscard]]
	auto event(const SDL_Event &event)
		-> SDL_AppResult;
};
}

#endif
