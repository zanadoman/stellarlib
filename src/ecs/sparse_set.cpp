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

#include <stellarlib/ecs/sparse_set.hpp>

#include <cstdint>

namespace stellarlib::ecs::internal
{
sparse_set::sparse_set() noexcept = default;

sparse_set::sparse_set(sparse_set &&) noexcept = default;

auto sparse_set::operator=(sparse_set &&) noexcept
	-> sparse_set & = default;

sparse_set::~sparse_set() noexcept = default;

void sparse_set::insert(const std::uint32_t key) noexcept
{
	_sparse.extend(key + 1, static_cast<std::uint32_t>(-1));
	_sparse[key] = _keys.size();
	_keys.push(key);
}

auto sparse_set::size() const noexcept
	-> std::uint32_t
{
	return _keys.size();
}

auto sparse_set::contains(const std::uint32_t key) const noexcept
	-> bool
{
	return key < _sparse.size() && _sparse[key] != static_cast<std::uint32_t>(-1);
}

auto sparse_set::begin() const noexcept
	-> const std::uint32_t *
{
	return _keys.begin();
}

auto sparse_set::end() const noexcept
	-> const std::uint32_t *
{
	return _keys.end();
}

void sparse_set::erase(const std::uint32_t key) noexcept
{
	if (_sparse[key] != _keys.size() - 1) {
		_keys[_sparse[key]] = *(_keys.end() - 1);
		_sparse[_keys[_sparse[key]]] = _sparse[key];
	}

	_sparse[key] = static_cast<std::uint32_t>(-1);
	_keys.pop();
}

void sparse_set::clear() noexcept
{
	_sparse.clear();
	_keys.clear();
}
}
