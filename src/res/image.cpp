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

#include <stellarlib/res/image.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>

namespace stellarlib::res
{
image::image(const std::filesystem::path &path)
	: _handle{SDL_LoadPNG(path.c_str()), SDL_DestroySurface}
{
	if (!_handle) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (_handle->format == SDL_PIXELFORMAT_ABGR8888) {
		return;
	}

	_handle.reset(SDL_ConvertSurface(_handle.get(), SDL_PIXELFORMAT_ABGR8888));

	if (!_handle) {
		throw std::runtime_error{SDL_GetError()};
	}
}

image::image(const image &other)
	: _handle{SDL_DuplicateSurface(other._handle.get()), SDL_DestroySurface}
{
	if (!_handle) {
		throw std::runtime_error{SDL_GetError()};
	}
}

image::image(image &&other) noexcept = default;

auto image::operator=(const image &other)
	-> image &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, other);
	}

	return *this;
}

auto image::operator=(image &&other) noexcept
	-> image & = default;

image::~image() = default;

auto image::width()
	-> std::uint32_t
{
	return static_cast<std::uint32_t>(_handle->w);
}

auto image::height()
	-> std::uint32_t
{
	return static_cast<std::uint32_t>(_handle->h);
}
}
