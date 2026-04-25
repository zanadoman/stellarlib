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

#include <stellarlib/app/context.hpp>
#include "context_impl.hpp"

#include <stellarlib/app/clock.hpp>
#include <stellarlib/app/main.hpp>
#include <stellarlib/app/subsystem.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

namespace stellarlib::app
{
context::impl::impl(const app::info &info)
	: _clock{info.clock}
	, _scene{info.scene}
{
	_scene->begin(context{*this});
}

context::impl::~impl()
{
	_scene->end(context{*this});
}

auto context::impl::world() const noexcept
	-> const ecs::world &
{
	return _world;
}

auto context::impl::world() noexcept
	-> ecs::world &
{
	return _world;
}

auto context::impl::clock() const noexcept
	-> const class clock &
{
	return _clock;
}

auto context::impl::clock() noexcept
	-> class clock &
{
	return _clock;
}

auto context::impl::iterate()
	-> SDL_AppResult
{
	const auto scene{_scene->update(context{*this})};
	static_cast<internal::subsystem<app::clock> &>(_clock).iterate();

	if (!static_cast<bool>(scene)) {
		return SDL_APP_SUCCESS;
	}

	if (scene != _scene.get()) {
		_scene->end(context{*this});
		_scene.reset(scene);
		_scene->begin(context{*this});
	}

	return SDL_APP_CONTINUE;
}

auto context::impl::event(const SDL_Event *event)
	-> SDL_AppResult
{
	return event->type == SDL_EVENT_QUIT ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}
}
