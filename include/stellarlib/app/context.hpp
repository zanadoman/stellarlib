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

#ifndef STELLARLIB_APP_CONTEXT_HPP
#define STELLARLIB_APP_CONTEXT_HPP

#include <stellarlib/ecs/ecs.hpp>

namespace stellarlib::app
{
class clock;
class metadata;

class context final
{
public:
	class impl;

	[[nodiscard]]
	explicit context(impl &impl) noexcept;

	[[nodiscard]]
	constexpr context(const context &) noexcept = delete;

	[[nodiscard]]
	constexpr context(context &&) noexcept = delete;

	constexpr auto operator=(const context &) noexcept
		-> context & = delete;

	constexpr auto operator=(context &&) noexcept
		-> context & = delete;

	~context() noexcept;

	[[nodiscard]]
	auto world() const noexcept
		-> const ecs::world &;

	[[nodiscard]]
	auto world() noexcept
		-> ecs::world &;

	[[nodiscard]]
	auto clock() const noexcept
		-> const clock &;

	[[nodiscard]]
	auto clock() noexcept
		-> class clock &;

	[[nodiscard]]
	auto metadata() const noexcept
		-> const metadata &;

	[[nodiscard]]
	auto metadata() noexcept
		-> class metadata &;

private:
	impl &_impl;
};
}

#endif
