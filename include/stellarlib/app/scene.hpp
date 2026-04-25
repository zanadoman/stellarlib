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

#ifndef STELLARLIB_APP_SCENE_HPP
#define STELLARLIB_APP_SCENE_HPP

#include <stellarlib/app/context.hpp>

namespace stellarlib::app
{
class scene
{
public:
	virtual ~scene() noexcept;

	virtual void begin(context ctx);

	[[nodiscard]]
	virtual constexpr auto update(context)
		-> scene * = 0;

	virtual void end(context ctx);

protected:
	[[nodiscard]]
	scene() noexcept;

	[[nodiscard]]
	scene(const scene &) noexcept;

	[[nodiscard]]
	scene(scene &&) noexcept;

	auto operator=(const scene &) noexcept
		-> scene &;

	auto operator=(scene &&) noexcept
		-> scene &;
};
}

#endif
