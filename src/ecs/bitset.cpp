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

#include <stellarlib/ecs/bitset.hpp>

#include <stellarlib/ext/bit.hpp>
#include <stellarlib/ext/functional.hpp>
#include <stellarlib/ext/memory.hpp>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <ranges>
#include <utility>

namespace stellarlib::ecs
{
bitset::bitset(const bitset &other) noexcept
	: _size{other._size}
{
	if (ext::truthy(_size)) {
		_capacity = _size;
		ext::vector_allocator<std::size_t>::allocate(_begin, _capacity);
		std::ranges::copy(other.segments(), _begin);
		_end = _begin + _size;
	}
}

bitset::bitset(bitset &&other) noexcept
	: _size{other._size}
	, _capacity{other._capacity}
	, _begin{other._begin}
	, _end{other._end}
{
	other._begin = nullptr;
}

auto bitset::operator=(const bitset &other) noexcept
	-> bitset &
{
	if (std::addressof(other) == this) {
		return *this;
	}

	_size = other._size;

	if (_capacity < _size) {
		_capacity = _size;
		ext::vector_allocator<std::size_t>::reallocate(_begin, _capacity);
	}

	std::ranges::copy(other.segments(), _begin);
	_end = _begin + _size;
	return *this;
}

auto bitset::operator=(bitset &&other) noexcept
	-> bitset &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
	}

	return *this;
}

bitset::~bitset() noexcept
{
	ext::vector_allocator<std::size_t>::deallocate(_begin);
}

void bitset::insert(const std::size_t elem) noexcept
{
	const auto index{ext::bit_index(elem)};

	if (index < _size) {
		_begin[index] |= ext::bit_mask(elem);
		return;
	}

	if (_capacity <= index) {
		_capacity = index + 1;
		ext::vector_allocator<std::size_t>::reallocate(_begin, _capacity);
		std::fill(_begin + _size, _begin + index, 0);
	}

	_size = index + 1;
	_begin[index] = ext::bit_mask(elem);
	_end = _begin + _size;
}

auto bitset::contains(const std::size_t elem) const noexcept
	-> bool
{
	const auto index{ext::bit_index(elem)};
	return index < _size && ext::truthy((_begin[index] & ext::bit_mask(elem)));
}

auto bitset::operator==(const bitset &other) const noexcept
	-> bool
{
	return std::ranges::equal(other.segments(), segments());
}

auto bitset::operator<=(const bitset &other) const noexcept
	-> bool
{
	return _size <= other._size && std::ranges::all_of(std::views::zip(segments(), other.segments()), ext::zip::subset<std::size_t>);
}

auto bitset::operator>=(const bitset &other) const noexcept
	-> bool
{
	return other <= *this;
}

void bitset::erase(const std::size_t elem) noexcept
{
	const auto index{ext::bit_index(elem)};

	if (_size <= index) {
		return;
	}

	_begin[index] &= ~ext::bit_mask(elem);

	if (index != _size - 1) {
		return;
	}

	for (; ext::truthy(_size) && ext::falsy(_begin[_size - 1]); --_size) {}

	_end = _begin + _size;
}

void bitset::clear() noexcept
{
	std::ranges::fill(segments(), 0);
}

auto bitset::segments() const noexcept
	-> std::ranges::subrange<std::size_t *, std::size_t *>
{
	return std::ranges::subrange{_begin, _end};
}
}
