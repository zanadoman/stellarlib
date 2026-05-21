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

/**
 * @brief Graphics and rendering abstractions
 */
namespace stellarlib::gfx
{
/**
 * @brief Generic GPU resource
 * @tparam T Type of the resource
 */
template <typename T, void (*DELETER)(SDL_GPUDevice *, T *)>
class [[nodiscard]] resource final
{
public:
	/**
	 * @brief Parameterized constructor
	 * @param handle Handle of the resource
	 * @param device Device of the resource
	 * @param mutex Device synchronization primitive
	 */
	[[nodiscard]]
	constexpr resource(T *handle, SDL_GPUDevice *device, const std::weak_ptr<std::recursive_mutex> &mutex)
		: _handle{handle}
		, _device{device}
		, _mutex{mutex}
	{}

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr resource(const resource &) noexcept = delete;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr resource(resource &&other)
		: _handle{std::exchange(other._handle, {})}
		, _device{std::exchange(other._device, {})}
		, _mutex{std::exchange(other._mutex, {})}
	{}

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const resource &) noexcept
		-> resource & = delete;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(resource &&other)
		-> resource &
	{
		if (std::addressof(other) != this) {
			std::destroy_at(this);
			std::construct_at(this, std::move(other));
		}

		return *this;
	}

	/**
	 * @brief Destructor
	 */
	constexpr ~resource()
	{
		if (const auto mutex{_mutex.lock()}) {
			const std::scoped_lock<std::recursive_mutex> guard{*mutex};
			DELETER(_device, _handle);
		}
	}

	/**
	 * @brief Returns a pointer to the internal handle
	 * @return Pointer to the internal handle
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit constexpr operator T *() const
	{
		return _handle;
	}

	/**
	 * @brief Returns a pointer to the internal device
	 * @return Pointer to the internal device
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit constexpr operator const SDL_GPUDevice *() const
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
