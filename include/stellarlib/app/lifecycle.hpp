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

#ifndef STELLARLIB_APP_SUBSYSTEM_HPP
#define STELLARLIB_APP_SUBSYSTEM_HPP

#include <stellarlib/app/context.hpp>
#include <stellarlib/ecs/ecs.hpp>

#include <SDL3/SDL_events.h>

#include <utility>

namespace stellarlib::app::internal
{
class lifecycle final
{
friend context::impl;

public:
	[[nodiscard]]
	constexpr lifecycle() noexcept = delete;

	[[nodiscard]]
	constexpr lifecycle(const lifecycle &) noexcept = delete;

	[[nodiscard]]
	constexpr lifecycle(lifecycle &&) noexcept = delete;

	constexpr auto operator=(const lifecycle &) noexcept
		-> lifecycle & = delete;

	constexpr auto operator=(lifecycle &&) noexcept
		-> lifecycle & = delete;

	constexpr ~lifecycle() noexcept = delete;

private:
	template <typename T, typename ...Args>
	[[nodiscard]]
	static constexpr auto construct(Args &&...args)
	{
		return T{std::forward<Args>(args)...};
	}

	template <typename T, typename ...Args>
	[[nodiscard]]
	static constexpr auto iterate(const T &self, Args &&...args)
	{
		return self.iterate(std::forward<Args>(args)...);
	}

	template <typename T, typename ...Args>
	[[nodiscard]]
	static constexpr auto iterate(T &self, Args &&...args)
	{
		return self.iterate(std::forward<Args>(args)...);
	}

	template <typename T, typename ...Args>
	[[nodiscard]]
	static constexpr auto event(const T &self, Args &&...args)
	{
		return self.event(std::forward<Args>(args)...);
	}

	template <typename T, typename ...Args>
	[[nodiscard]]
	static constexpr auto event(T &self, Args &&...args)
	{
		return self.event(std::forward<Args>(args)...);
	}
};
}

#endif
