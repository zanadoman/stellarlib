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

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gpu.h>

#include <memory>
#include <stdexcept>
#include <utility>

namespace stellarlib::gfx
{
texture::texture(const std::shared_ptr<SDL_GPUDevice> &device, const SDL_GPUTextureCreateInfo &info)
	: _handle{SDL_CreateGPUTexture(device.get(), std::addressof(info))}
	, _device{device}
{
	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

texture::texture(texture &&other)
	: _handle{std::exchange(other._handle, {})}
	, _device{std::exchange(other._device, {})}
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
}
