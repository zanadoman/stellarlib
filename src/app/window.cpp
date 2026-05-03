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

#include <stellarlib/app/window.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_video.h>

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace stellarlib::app
{
window::~window()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

auto window::title() const
	-> std::string
{
	return SDL_GetWindowTitle(_handle.get());
}

void window::set_title(const std::string &title)
{
	if (!SDL_SetWindowTitle(_handle.get(), title.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}
}

window::window(const info &info)
{
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_handle = {SDL_CreateWindow(info.title.c_str(), 0, 0, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE), SDL_DestroyWindow};

	if (!_handle) {
		throw std::runtime_error{SDL_GetError()};
	}
}

void window::iterate()
{
	const auto surface{SDL_GetWindowSurface(_handle.get())};

	if (!static_cast<bool>(surface)) {
		throw std::runtime_error{SDL_GetError()};
	}

	const auto format{SDL_GetPixelFormatDetails(surface->format)};

	if (!static_cast<bool>(format)) {
		throw std::runtime_error{SDL_GetError()};
	}

	std::fill_n(static_cast<std::uint8_t *>(surface->pixels), surface->w * surface->h * format->bytes_per_pixel, 0);

	if (!SDL_UpdateWindowSurface(_handle.get())) {
		throw std::runtime_error{SDL_GetError()};
	}
}

void window::event(const SDL_Event &event) const
{
	if (event.type == SDL_EVENT_WINDOW_DESTROYED && event.window.windowID == SDL_GetWindowID(_handle.get())) {
		SDL_Unsupported();
		throw std::runtime_error{SDL_GetError()};
	}
}
}
