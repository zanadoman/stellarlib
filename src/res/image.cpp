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

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
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

image::image(const lin::ushort2 &size)
	: _handle{SDL_CreateSurface(static_cast<std::int32_t>(lin::round(size.x())), static_cast<std::int32_t>(lin::round(size.y())), SDL_PIXELFORMAT_ABGR8888), SDL_DestroySurface}
{
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

auto image::size() const
	-> lin::ushort2
{
	return lin::ushort2{_handle->w, _handle->h};
}

auto image::operator[](const lin::ushort2 &coord) const
	-> const lin::uchar4 &
{
	return *(static_cast<const lin::uchar4 *>(_handle->pixels) + lin::mad(coord.y(), _handle->pitch, coord.x()));
}

auto image::operator[](const lin::ushort2 &coord)
	-> lin::uchar4 &
{
	return *(static_cast<lin::uchar4 *>(_handle->pixels) + lin::mad(coord.y(), _handle->w, coord.x()));
}

auto image::data() const
	-> std::span<const std::uint8_t>
{
	return {static_cast<const std::uint8_t *>(_handle->pixels), static_cast<std::size_t>(_handle->pitch * _handle->h)};
}

auto image::data()
	-> std::span<std::uint8_t>
{
	return {static_cast<std::uint8_t *>(_handle->pixels), static_cast<std::size_t>(_handle->pitch * _handle->h)};
}

auto image::sample(lin::float2 coord)
	-> lin::float4
{
	coord = lin::saturate(coord * size());
	return lin::float4{(*this)[lin::ushort2{coord.x(), coord.y()}]} / 255.0F;
}

void image::paint(const lin::ushort2 &coord, lin::float4 color)
{
	color *= 255.0F;
	(*this)[coord] = lin::uchar4{color.r(), color.g(), color.b(), color.a()};
}

void image::save(const std::filesystem::path &path) const
{
	if (!SDL_SavePNG(_handle.get(), path.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}
}
}
