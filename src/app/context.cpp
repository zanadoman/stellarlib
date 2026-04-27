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

#include <stellarlib/app/clock.hpp>
#include <stellarlib/app/lifecycle.hpp>
#include <stellarlib/app/metadata.hpp>
#include <stellarlib/app/bridge.hpp>
#include <stellarlib/app/scene.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <functional>
#include <memory>
#include <variant>

namespace stellarlib::app
{
context::~context()
{
	if (_scene) {
		internal::lifecycle::end(*_scene, *this);
	}
}

auto context::world() const
	-> const ecs::world &
{
	return _world;
}

auto context::world()
	-> ecs::world &
{
	return _world;
}

auto context::metadata() const
	-> const app::metadata &
{
	return _metadata;
}

auto context::metadata()
	-> app::metadata &
{
	return _metadata;
}

auto context::clock() const
	-> const app::clock &
{
	return _clock;
}

auto context::clock()
	-> app::clock &
{
	return _clock;
}

context::context(const info &info)
	: _metadata{internal::bridge<context>::init<app::metadata>(info.metadata)}
	, _clock{internal::bridge<context>::init<app::clock>(info.clock)}
{
	if (const auto main{std::get_if<scene *>(std::addressof(info.main))}) {
		_scene.reset(*main);
	}
	else if (const auto callback{std::get_if<std::function<scene * (context &)>>(std::addressof(info.main))}; *callback) {
		_scene.reset((*callback)(*this));
	}

	if (_scene) {
		internal::lifecycle::begin(*_scene, *this);
	}
}

auto context::iterate()
	-> SDL_AppResult
{
	if (!_scene) {
		return SDL_APP_SUCCESS;
	}

	const auto scene{internal::lifecycle::update(*_scene, *this)};
	internal::bridge<context>::iterate(_clock);

	if (!static_cast<bool>(scene)) {
		return SDL_APP_SUCCESS;
	}

	if (scene != _scene.get()) {
		internal::lifecycle::end(*_scene, *this);
		_scene.reset(scene);
		internal::lifecycle::begin(*_scene, *this);
	}

	return SDL_APP_CONTINUE;
}

auto context::event(const SDL_Event *event) const
	-> SDL_AppResult
{
	return event->type == SDL_EVENT_QUIT ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}
}
