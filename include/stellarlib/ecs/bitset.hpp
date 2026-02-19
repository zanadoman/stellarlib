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

#ifndef STELLARLIB_ECS_BITSET_HPP
#define STELLARLIB_ECS_BITSET_HPP

#include <stellarlib/ext/memory.hpp>

#include <cstddef>
#include <memory>
#include <ranges>

namespace stellarlib::ecs
{
class bitset final : ext::vector_allocator<std::size_t>
{
public:
	[[nodiscard]]
	explicit bitset() noexcept = default;

	[[nodiscard]]
	bitset(const bitset &other) noexcept;

	[[nodiscard]]
	bitset(bitset &&) noexcept = default;

	auto operator=(const bitset &other) noexcept
		-> bitset &;

	auto operator=(bitset &&) noexcept
		-> bitset & = default;

	~bitset() noexcept = default;

	void insert(std::size_t elem) noexcept;

	[[nodiscard]]
	auto contains(std::size_t elem) const noexcept
		-> bool;

	[[nodiscard]]
	auto operator==(const bitset &other) const noexcept
		-> bool;

	[[nodiscard]]
	auto operator<=(const bitset &other) const noexcept
		-> bool;

	[[nodiscard]]
	auto operator>=(const bitset &other) const noexcept
		-> bool;

	void erase(std::size_t elem) noexcept;

	void clear() noexcept;

private:
	std::size_t _size{};
	std::size_t _capacity{};
	std::unique_ptr<std::size_t, void (*)(std::size_t *)> _begin{nullptr, ext::vector_allocator<std::size_t>::deallocate};
	std::size_t *_end{};

	[[nodiscard]]
	auto segments() const noexcept
		-> std::ranges::subrange<std::size_t *, std::size_t *>;
};
}

#endif
