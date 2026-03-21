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

#include <stellarlib/ecs/world.hpp>

#include <stellarlib/ecs/archetype.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ranges>
#include <utility>

namespace stellarlib::ecs
{
world::world() noexcept = default;

world::world(world &&) noexcept = default;

auto world::operator=(world &&) noexcept
	-> world & = default;

world::~world() noexcept = default;

auto world::size() const noexcept
	-> std::size_t
{
	return _entities.size();
}

auto world::contains(const std::uint32_t entity) const noexcept
	-> bool
{
	return _entities.contains(entity);
}

auto world::at(const std::uint32_t entity) const noexcept
	-> const archetype *
{
	if (const auto pair{_entities.at(entity)}) {
		return std::addressof(_archetypes[pair->first].first);
	}

	return nullptr;
}

auto world::operator[](const std::uint32_t entity) const noexcept
	-> const archetype &
{
	return _archetypes[_entities[entity].first].first;
}

void world::despawn(const std::uint32_t entity) noexcept
{
	const auto pair{_entities.at(entity)};

	if (_spawned.contains(entity)) {
		_spawned.erase(entity);
	}
	else if (!static_cast<bool>(pair) || pair->second) {
		return;
	}
	else {
		pair->second = true;
	}

	_despawned.push(entity);

	if (static_cast<bool>(_lock)) {
		_commands.enqueue([cpt = std::pair{this, entity}] noexcept -> void {
			cpt.first->_components.erase(cpt.second);
			cpt.first->_archetypes[cpt.first->_entities[cpt.second].first].second.erase(cpt.second);

			if (cpt.first->_entities[cpt.second].second) {
				cpt.first->_entities.erase(cpt.second);
			}
		});
	}
	else {
		_components.erase(entity);
		_archetypes[_entities[entity].first].second.erase(entity);
		_entities.erase(entity);
	}
}

void world::clear() noexcept
{
	_spawned.clear();

	for (const auto [entity, pair] : _entities.zip() | std::views::reverse) {
		if (!pair.second) {
			pair.second = true;
			_despawned.push(entity);
		}
	}

	const auto command{[this] noexcept -> void {
		_components.clear();

		for (auto &pair : _archetypes) {
			pair.second.clear();
		}

		_entities.clear();
	}};

	if (static_cast<bool>(_lock)) {
		_commands.enqueue([command] noexcept -> void {
			command();
		});
	}
	else {
		command();
	}
}

thread_local archetype world::cache{};
}
