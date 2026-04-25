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

#ifndef STELLARLIB_APP_CONTEXT_IMPL_HPP
#define STELLARLIB_APP_CONTEXT_IMPL_HPP

#include <stellarlib/app/context.hpp>

#include <stellarlib/app/clock.hpp>
#include <stellarlib/app/main.hpp>
#include <stellarlib/app/metadata.hpp>
#include <stellarlib/app/scene.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <memory>

namespace stellarlib::app
{
class context::impl final
{
public:
	[[nodiscard]]
	explicit impl(const app::info &info);

	[[nodiscard]]
	constexpr impl(const impl &) noexcept = delete;

	[[nodiscard]]
	constexpr impl(impl &&) noexcept = delete;

	constexpr auto operator=(const impl &) noexcept
		-> impl & = delete;

	constexpr auto operator=(impl &&) noexcept
		-> impl & = delete;

	~impl();

	[[nodiscard]]
	auto world() const noexcept
		-> const ecs::world &;

	[[nodiscard]]
	auto world() noexcept
		-> ecs::world &;

	[[nodiscard]]
	auto metadata() const noexcept
		-> const class metadata &;

	[[nodiscard]]
	auto metadata() noexcept
		-> class metadata &;

	[[nodiscard]]
	auto clock() const noexcept
		-> const class clock &;

	[[nodiscard]]
	auto clock() noexcept
		-> class clock &;

	[[nodiscard]]
	auto iterate()
		-> SDL_AppResult;

	[[nodiscard]]
	auto event(const SDL_Event *event)
		-> SDL_AppResult;

private:
	ecs::world _world;
	[[no_unique_address]] class metadata _metadata;
	class clock _clock;
	std::unique_ptr<scene> _scene;
};
}

#endif
