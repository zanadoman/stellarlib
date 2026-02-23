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

#ifndef STELLARLIB_ECS_WORLD_HPP
#define STELLARLIB_ECS_WORLD_HPP

#include <stellarlib/ecs/bitset.hpp>
#include <stellarlib/ecs/sparse_map.hpp>
#include <stellarlib/ecs/sparse_set.hpp>
#include <stellarlib/ecs/sparse_storage.hpp>
#include <stellarlib/ecs/stack_vector.hpp>
#include <stellarlib/ext/utility.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <tuple>
#include <ranges>

namespace stellarlib::ecs
{
class world final
{
public:
	[[nodiscard]]
	explicit constexpr world() noexcept = default;

	[[nodiscard]]
	constexpr world(const world &) noexcept = default;

	[[nodiscard]]
	constexpr world(world &&) noexcept = default;

	constexpr auto operator=(const world &) noexcept
		-> world & = default;

	constexpr auto operator=(world &&) noexcept
		-> world & = default;

	constexpr ~world() noexcept = default;

	template <typename ...T>
	constexpr auto spawn(T &&...components) noexcept
	{
		auto entity{_entities.size()};

		if (_queue.size()) {
			entity = *(_queue.end() - 1);
			_queue.pop();
		}

		_bitset.clear();

		([&]() -> void {
			const auto id{_components.id<T>()};
			_bitset.insert(id);
			_components.at<T>(id).insert(entity, std::forward<T>(components));
		}(), ...);

		const auto archetype{std::ranges::find_if(_archetypes, [this](const auto &pair) -> bool {
			return pair.first == _bitset;
		})};

		if (archetype == _archetypes.end()) {
			_entities.insert(entity, _archetypes.size());
			_archetypes.push(std::pair{_bitset, internal::sparse_set<std::uint32_t>{}});
			(_archetypes.end() - 1)->second.insert(entity);

			for (auto &index : _indices) {
				if (index.first <= (_archetypes.end() - 1)->first) {
					index.second.push(_archetypes.size() - 1);
				}
			}
		}
		else {
			archetype->second.insert(entity);
			_entities.insert(entity, static_cast<std::size_t>(archetype - _archetypes.begin()));
		}

		return entity;
	}

	[[nodiscard]]
	constexpr auto query() const noexcept
	{
		return _entities.keys();
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto query() noexcept
	{
		return _components.at<T>().zip();
	}

	template <typename ...T>
	[[nodiscard]]
	constexpr auto query() noexcept
		requires (1 < sizeof...(T))
	{
		const auto id{ext::scoped_typeid<world, std::tuple<T...>>()};

		if (_queries.extend(id + 1, static_cast<std::size_t>(-1)) || _queries[id] == static_cast<std::size_t>(-1)) {
			_bitset.clear();
			(_bitset.insert(_components.id<T>()), ...);

			const auto index{std::ranges::find_if(_indices, [this](const auto &pair) -> bool {
				return pair.first == _bitset;
			})};

			if (index == _indices.end()) {
				_queries[id] = _indices.size();
				_indices.push(std::pair{_bitset, internal::stack_vector<std::size_t>{}});

				for (const auto i : std::views::iota(std::size_t{}, _archetypes.size())) {
					if (_bitset <= _archetypes[i].first) {
						(_indices.end() - 1)->second.push(i);
					}
				}
			}
			else {
				_queries[id] = static_cast<std::size_t>(index - _indices.begin());
			}
		}

		return _indices[_queries[id]].second
			| std::views::transform([this](const auto index) -> internal::sparse_set<std::uint32_t> & {
				return _archetypes[index].second;
			})
			| std::views::join
			| std::views::transform([this](const auto entity) -> std::tuple<std::uint32_t, T *...> {
				return {entity, std::addressof(_components.operator[]<T>()[entity])...};
			});
	}

	constexpr auto despawn(const std::uint32_t entity) noexcept
	{
		if (const auto index{_entities.at(entity)}) {
			_entities.erase(entity);
			_archetypes[*index].second.erase(entity);
			_components.erase(entity);
			_queue.push(entity);
		}
	}

private:
	internal::stack_vector<std::uint32_t> _queue;
	internal::sparse_map<std::uint32_t, std::size_t> _entities;
	internal::stack_vector<std::pair<internal::bitset, internal::sparse_set<std::uint32_t>>> _archetypes;
	internal::sparse_storage _components;
	internal::stack_vector<std::size_t, std::size_t> _queries;
	internal::stack_vector<std::pair<internal::bitset, internal::stack_vector<std::size_t>>> _indices;
	internal::bitset _bitset;
};
}

#endif
