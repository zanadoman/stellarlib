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

#ifndef STELLARLIB_GFX_RESOURCE_HPP
#define STELLARLIB_GFX_RESOURCE_HPP

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <mutex>
#include <utility>

namespace stellarlib::gfx
{
template <typename T>
class [[nodiscard]] resource final
{
public:
	[[nodiscard]]
	constexpr resource(T *handle, SDL_GPUDevice *device, std::weak_ptr<std::recursive_mutex> mutex)
		: _handle{handle}
		, _device{device}
		, _mutex{std::move(mutex)}
	{}

	[[nodiscard]]
	constexpr resource(const resource &) noexcept = delete;

	[[nodiscard]]
	constexpr resource(resource &&other)
		: _handle{std::exchange(other._handle, {})}
		, _device{std::exchange(other._device, {})}
		, _mutex{std::exchange(other._mutex, {})}
	{}

	constexpr auto operator=(const resource &) noexcept
		-> resource & = delete;

	auto operator=(resource &&other)
		-> resource &
	{
		if (std::addressof(other) != this) {
			std::destroy_at(this);
			std::construct_at(this, std::move(other));
		}

		return *this;
	}

	constexpr ~resource()
	{
		if (const auto mutex{_mutex.lock()}) {
			const std::lock_guard<std::recursive_mutex> guard{*mutex};
			SDL_ReleaseGPUTexture(_device, _handle);
		}
	}

	[[nodiscard]]
	explicit operator T *() const
	{
		return _handle;
	}

	[[nodiscard]]
	explicit operator const SDL_GPUDevice *() const
	{
		return _device;
	}

private:
	T *_handle{};
	SDL_GPUDevice *_device{};
	std::weak_ptr<std::recursive_mutex> _mutex;
};
}

#endif
