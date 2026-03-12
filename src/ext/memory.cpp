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

#include <stellarlib/ext/memory.hpp>

#include <stellarlib/ext/functional.hpp>

#include <SDL3/SDL_cpuinfo.h>

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <utility>

namespace stellarlib::ext
{
auto arena::size() noexcept
	-> size_type
{
	return capacity;
}

arena::arena() noexcept = default;

arena::arena(arena &&other) noexcept
	: _begin{std::exchange(other._begin, {})}
	, _cursor{std::exchange(other._cursor, {})}
	, _capacity{std::exchange(other._capacity, {})}
{}

auto arena::operator=(arena &&other) noexcept
	-> arena &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
	}

	return *this;
}

arena::~arena() noexcept
{
	std::free(_begin);
}

auto arena::operator==(const arena &other) const noexcept
	-> bool
{
	return std::addressof(other) == this;
}

void arena::deallocate() noexcept
{
	_cursor = _begin;
	_capacity = capacity;
}

arena::size_type arena::capacity{[] [[nodiscard]] noexcept -> auto {
	const auto page{SDL_GetSystemPageSize()};
	return 0 < page ? static_cast<size_type>(page) : 4096;
}()};

arena::size_type arena::alignment{truthy(capacity % alignof(std::max_align_t)) ? alignof(std::max_align_t) : capacity};

arena_allocator::arena_allocator() noexcept
{
	std::construct_at(_begin);
}

arena_allocator::arena_allocator(arena_allocator &&other) noexcept
	: _begin{std::exchange(other._begin, {})}
	, _cursor{std::exchange(other._cursor, {})}
	, _capacity{std::exchange(other._capacity, {})}
{}

auto arena_allocator::operator=(arena_allocator &&other) noexcept
	-> arena_allocator &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
	}

	return *this;
}

arena_allocator::~arena_allocator() noexcept
{
	std::destroy_n(_begin, _capacity);
	std::free(_begin);
}

auto arena_allocator::operator==(const arena_allocator &other) const noexcept
	-> bool
{
	return std::addressof(other) == this;
}

void arena_allocator::deallocate() noexcept
{
	for (const auto arena : std::views::iota(_begin, _begin + _cursor + 1)) {
		arena->deallocate();
	}

	_cursor = 0;
}
}
