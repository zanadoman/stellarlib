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

#ifndef STELLARLIB_ECS_SPARSE_SET_HPP
#define STELLARLIB_ECS_SPARSE_SET_HPP

#include <stellarlib/ecs/stack_vector.hpp>

#include <cstdint>

namespace stellarlib::ecs::internal
{
class sparse_set final
{
public:
	[[nodiscard]]
	explicit sparse_set() noexcept;

	[[nodiscard]]
	constexpr sparse_set(const sparse_set &) noexcept = delete;

	[[nodiscard]]
	sparse_set(sparse_set &&) noexcept;

	constexpr auto operator=(const sparse_set &) noexcept
		-> sparse_set & = delete;

	auto operator=(sparse_set &&) noexcept
		-> sparse_set &;

	~sparse_set() noexcept;

	void insert(std::uint32_t key) noexcept;

	[[nodiscard]]
	auto size() const noexcept
		-> std::uint32_t;

	[[nodiscard]]
	auto contains(std::uint32_t key) const noexcept
		-> bool;

	[[nodiscard]]
	auto begin() const noexcept
		-> const std::uint32_t *;

	[[nodiscard]]
	auto end() const noexcept
		-> const std::uint32_t *;

	void erase(std::uint32_t key) noexcept;

	void clear() noexcept;

private:
	stack_vector<std::uint32_t, std::uint32_t> _sparse;
	stack_vector<std::uint32_t, std::uint32_t> _keys;
};
}

#endif
