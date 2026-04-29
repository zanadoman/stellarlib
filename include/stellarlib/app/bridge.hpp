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

#ifndef STELLARLIB_APP_BRIDGE_HPP
#define STELLARLIB_APP_BRIDGE_HPP

#include <SDL3/SDL_events.h>

#include <utility>

namespace stellarlib::app::internal
{
template <typename System>
class bridge final
{
friend System;

public:
	[[nodiscard]]
	constexpr bridge() noexcept = delete;

	[[nodiscard]]
	constexpr bridge(const bridge &) noexcept = delete;

	[[nodiscard]]
	constexpr bridge(bridge &&) noexcept = delete;

	constexpr auto operator=(const bridge &) noexcept
		-> bridge & = delete;

	constexpr auto operator=(bridge &&) noexcept
		-> bridge & = delete;

	constexpr ~bridge() noexcept = delete;

private:
	template <typename Subsystem, typename Info>
	[[nodiscard]]
	static constexpr auto init(Info &&info)
	{
		return Subsystem{std::forward<Info>(info)};
	}

	template <typename Subsystem>
	[[nodiscard]]
	static constexpr auto iterate(Subsystem &subsystem)
	{
		return subsystem.iterate();
	}

	template <typename Subsystem>
	[[nodiscard]]
	static constexpr auto event(Subsystem &subsystem, const SDL_Event &event)
	{
		return subsystem.event(event);
	}
};
}

#endif
