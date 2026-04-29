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

#include <stellarlib/app/clock.hpp>

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_hints.h>

#include <chrono>
#include <stdexcept>
#include <string>

namespace stellarlib::app
{
clock::~clock() = default;

auto clock::target_frequency() const
	-> float
{
	return std::stof(SDL_GetHint(SDL_HINT_MAIN_CALLBACK_RATE));
}

void clock::set_target_frequency(const float target_frequency)
{
	if (target_frequency < 0.0F) {
		SDL_InvalidParamError("target_frequency");
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetHintWithPriority(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(target_frequency).c_str(), SDL_HINT_OVERRIDE)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

auto clock::max_delta() const
	-> float
{
	return _max_delta;
}

void clock::set_max_delta(const float max_delta)
{
	if (max_delta < 0.0F) {
		SDL_InvalidParamError("max_delta");
		throw std::runtime_error{SDL_GetError()};
	}

	_max_delta = max_delta;
}

auto clock::delta() const
	-> float
{
	return _delta;
}

clock::clock(const info &info)
	: _max_delta{info.max_delta}
	, _last_tick{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()}
{
	set_target_frequency(info.target_frequency);
}

void clock::iterate()
{
	const auto current_tick{std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()};
	_delta = lin::min(static_cast<float>(current_tick - _last_tick), _max_delta);
	_last_tick = current_tick;
}
}
