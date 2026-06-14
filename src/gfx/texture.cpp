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

#include <stellarlib/gfx/texture.hpp>

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

namespace stellarlib::gfx
{
texture::texture(const std::shared_ptr<SDL_GPUDevice> &device, const lin::uint2 size, const bool mipmaps)
	: _device{device}
	, _size{size}
{
	SDL_GPUTextureCreateInfo info{
		.format = format,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		.width = _size.x(),
		.height = _size.y(),
		.layer_count_or_depth = 1,
		.num_levels = 1
	};

	if (mipmaps) {
		info.num_levels += lin::cast<std::uint32_t>(lin::log(lin::max(info.width, info.height)));
	}

	_handle = SDL_CreateGPUTexture(_device.get(), std::addressof(info));
	_mipmaps = 1 < info.num_levels;
	_levels = info.num_levels;

	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

texture::texture(texture &&other)
	: _handle{std::exchange(other._handle, {})}
	, _device{std::exchange(other._device, {})}
	, _size{std::exchange(other._size, {})}
	, _mipmaps{std::exchange(other._mipmaps, {})}
	, _levels{std::exchange(other._levels, {})}
{}

auto texture::operator=(texture &&other)
	-> texture &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
	}

	return *this;
}

texture::~texture()
{
	SDL_ReleaseGPUTexture(_device.get(), _handle);
}

texture::operator SDL_GPUTexture *() const
{
	return _handle;
}

texture::operator const SDL_GPUDevice *() const
{
	return _device.get();
}

auto texture::size() const
	-> lin::uint2
{
	return _size;
}

auto texture::mipmaps() const
	-> bool
{
	return _mipmaps;
}

auto texture::levels() const
	-> std::uint32_t
{
	return _levels;
}
}
