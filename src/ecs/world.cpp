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
#include <stellarlib/ext/functional.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>

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
	if (const auto id{_entities.at(entity)}) {
		return std::addressof(_archetypes[id->first].first);
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
	const auto id{_entities.at(entity)};

	if (_pending.contains(entity)) {
		_pending.erase(entity);
	}
	else if (ext::falsy(id) || id->second) {
		return;
	}
	else if (ext::truthy(_lock)) {
		id->second = true;
	}

	const auto command{[this](const auto entity) noexcept -> void {
		_archetypes[_entities[entity].first].second.erase(entity);
		_entities.erase(entity);
		_components.erase(entity);
	}};

	if (ext::truthy(_lock)) {
		_commands.enqueue([command, entity, _ = 0] noexcept -> void {
			command(entity);
		});
	}
	else {
		command(entity);
	}

	_queue.insert(entity);
}

void world::clear() noexcept
{
	_queue.clear();
	_entities.clear();

	for (auto &pair : _archetypes) {
		pair.second.clear();
	}

	_components.clear();
}

thread_local archetype world::cache{};
}
