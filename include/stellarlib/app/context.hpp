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
#include <stellarlib/app/fwd.hpp>
#include <stellarlib/app/metadata.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_init.h>

#include <functional>
#include <memory>
#include <variant>

namespace stellarlib::app
{
class context final
{
friend internal::bridge<main>;

public:
	struct info final
	{
		metadata::info metadata;
		clock::info clock;
		std::variant<scene *, std::function<scene * (context &)>> main;
	};

	[[nodiscard]]
	constexpr context(const context &) noexcept = delete;

	[[nodiscard]]
	constexpr context(context &&) noexcept = delete;

	constexpr auto operator=(const context &) noexcept
		-> context & = delete;

	constexpr auto operator=(context &&) noexcept
		-> context & = delete;

	~context();

	[[nodiscard]]
	auto world() const
		-> const ecs::world &;

	[[nodiscard]]
	auto world()
		-> ecs::world &;

	[[nodiscard]]
	auto clock() const
		-> const app::clock &;

	[[nodiscard]]
	auto clock()
		-> app::clock &;

	[[nodiscard]]
	auto metadata() const
		-> const app::metadata &;

	[[nodiscard]]
	auto metadata()
		-> app::metadata &;

private:
	ecs::world _world;
	[[no_unique_address]] app::metadata _metadata;
	app::clock _clock;
	std::unique_ptr<scene> _scene;

	[[nodiscard]]
	explicit context(const info &info);

	[[nodiscard]]
	auto iterate()
		-> SDL_AppResult;

	[[nodiscard]]
	auto event(const SDL_Event *event)
		-> SDL_AppResult;
};
}

#endif
