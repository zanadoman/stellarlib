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

#include <stellarlib/ecs/archetype.hpp>
#include <stellarlib/ecs/command_queue.hpp>
#include <stellarlib/ecs/query.hpp>
#include <stellarlib/ecs/sparse_map.hpp>
#include <stellarlib/ecs/sparse_set.hpp>
#include <stellarlib/ecs/sparse_storage.hpp>
#include <stellarlib/ecs/stack_vector.hpp>
#include <stellarlib/ext/type_traits.hpp>
#include <stellarlib/ext/utility.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <functional>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace stellarlib::ecs
{
class world final
{
public:
	[[nodiscard]]
	explicit world() noexcept;

	[[nodiscard]]
	constexpr world(const world &) noexcept = delete;

	[[nodiscard]]
	world(world &&) noexcept;

	constexpr auto operator=(const world &) noexcept
		-> world & = delete;

	auto operator=(world &&) noexcept
		-> world &;

	~world() noexcept;

	template <typename ...T>
	constexpr auto spawn(T &&...components) noexcept
	{
		auto entity{_entities.size() + _spawned.size()};

		if (_despawned.size()) {
			entity = *(_despawned.end() - 1);
			_despawned.pop();
		}

		if (_lock) {
			if (const auto pair{_entities.at(entity)}) {
				pair->second = false;
			}
			else {
				_spawned.insert(entity);
			}
		}

		const auto command{[this] (const auto entity, auto &&...components) noexcept -> void {
			[this] <std::size_t ...I> (const auto entity, T &&...components, const std::index_sequence<I...>) noexcept -> void {
				const auto &ids{internal::sparse_storage::ids<T...>()};
				(_components.at<T>(ids[I]).insert(entity, std::forward<T>(components)), ...);
			}(entity, std::forward<T>(components)..., std::index_sequence_for<T...>{});

			relocate<const archetype &>(entity, archetype::of<T...>());
		}};

		if (_lock) {
			_commands.enqueue([cpt = std::pair{command, std::tuple{entity, std::forward<T>(components)...}}] mutable noexcept -> void {
				std::apply(cpt.first, std::move(cpt.second));
			});
		}
		else {
			command(entity, std::forward<T>(components)...);
		}

		return entity;
	}

	template <typename ...T>
	[[nodiscard]]
	constexpr auto insert(const std::uint32_t entity, T &&...components) noexcept
		-> std::expected<void, std::tuple<T...>>
		requires (0 < sizeof...(T))
	{
		if (const auto pair{_entities.at(entity)}; (!pair || pair->second) && !_spawned.contains(entity)) {
			return std::unexpected{std::tuple{std::forward<T>(components)...}};
		}

		const auto command{[this] (const auto entity, auto &&...components) noexcept -> void {
			[this] <std::size_t ...I> (const auto entity, T &&...components, const std::index_sequence<I...>) noexcept -> void {
				const auto &ids{internal::sparse_storage::ids<T...>()};
				(_components.at<T>(ids[I]).insert(entity, std::forward<T>(components)), ...);
			}(entity, std::forward<T>(components)..., std::index_sequence_for<T...>{});

			cache = _archetypes[_entities[entity].first].first;

			if constexpr (1 < sizeof...(T)) {
				cache.insert(archetype::of<T...>());
			}
			else {
				cache.insert(internal::sparse_storage::ids<T...>().front());
			}

			relocate(entity, _entities[entity].first);
		}};

		if (_lock) {
			_commands.enqueue([cpt = std::pair{command, std::tuple{entity, std::forward<T>(components)...}}] mutable noexcept -> void {
				std::apply(cpt.first, std::move(cpt.second));
			});
		}
		else {
			command(entity, std::forward<T>(components)...);
		}

		return {};
	}

	[[nodiscard]]
	auto size() const noexcept
		-> std::size_t;

	[[nodiscard]]
	auto contains(std::uint32_t entity) const noexcept
		-> bool;

	template <typename ...T>
	[[nodiscard]]
	constexpr auto contains(const std::uint32_t entity) const noexcept
		requires (0 < sizeof...(T))
	{
		if (const auto pair{_entities.at(entity)}) {
			return [] <std::size_t ...I> [[nodiscard]] (const auto &archetype, const std::index_sequence<I...>) noexcept -> auto {
				const auto &ids{internal::sparse_storage::ids<T...>()};
				return std::tuple{archetype.contains(ids[I])...};
			}(_archetypes[pair->first].first, std::index_sequence_for<T...>{});
		}

		return std::tuple{ext::expand_as_v<T, false>...};
	}

	[[nodiscard]]
	auto at(std::uint32_t entity) const noexcept
		-> const archetype *;

	template <typename ...T>
	[[nodiscard]]
	constexpr auto at(const std::uint32_t entity) const noexcept
		requires (0 < sizeof...(T))
	{
		return [this] <std::size_t ...I> [[nodiscard]] (const auto entity, const std::index_sequence<I...>) noexcept -> auto {
			const auto &ids{internal::sparse_storage::ids<T...>()};
			return std::tuple{_components.at<T>(ids[I]).at(entity)...};
		}(entity, std::index_sequence_for<T...>{});
	}

	[[nodiscard]]
	auto operator[](std::uint32_t entity) const noexcept
		-> const archetype &;

	template <typename ...T>
	[[nodiscard]]
	constexpr auto operator[](const std::uint32_t entity) const noexcept
		requires (0 < sizeof...(T))
	{
		return [this] <std::size_t ...I> [[nodiscard]] (const auto entity, const std::index_sequence<I...>) noexcept -> auto {
			const auto &ids{internal::sparse_storage::ids<T...>()};
			return std::tuple<T &...>{_components.operator[]<T>(ids[I])[entity]...};
		}(entity, std::index_sequence_for<T...>{});
	}

	[[nodiscard]]
	constexpr auto query() noexcept
	{
		++_lock;
		return internal::query{_archetypes | std::views::transform([] [[nodiscard]] (const auto &pair) noexcept -> auto {
			return pair.second | std::views::transform([&] [[nodiscard]] (const auto entity) noexcept -> auto {
				return std::tuple<std::uint32_t, const archetype &>{entity, pair.first};
			});
		}) | std::views::join, _execute};
	}

	template <typename T>
	[[nodiscard]]
	constexpr auto query() noexcept
	{
		++_lock;
		return internal::query{_components.at<T>(internal::sparse_storage::ids<T>().front()).zip(), _execute};
	}

	template <typename ...T>
	[[nodiscard]]
	constexpr auto query() noexcept
		requires (1 < sizeof...(T))
	{
		const auto id{ext::scoped_typeid<world, std::tuple<T...>, std::uint16_t>()};

		if (_queries.extend(id + 1, static_cast<std::uint16_t>(-1)) || _queries[id] == -1) {
			const auto &archetype{archetype::of<T...>()};

			const auto pair{std::ranges::find_if(_indices, [&] [[nodiscard]] (const auto &pair) noexcept -> auto {
				return pair.first == archetype;
			})};

			if (pair == _indices.end()) {
				_queries[id] = _indices.size();
				_indices.push(archetype, internal::stack_vector<std::uint16_t>{});

				for (const auto index : std::views::iota(std::uint16_t{}, _archetypes.size())) {
					if (archetype <= _archetypes[index].first) {
						(_indices.end() - 1)->second.push(index);
					}
				}
			}
			else {
				_queries[id] = static_cast<std::uint16_t>(pair - _indices.begin());
			}
		}

		++_lock;
		return internal::query{std::ranges::subrange{_indices[_queries[id]].second} | std::views::transform([this] [[nodiscard]] (const auto index) noexcept -> const auto & {
			return _archetypes[index].second;
		}) | std::views::join | std::views::transform([components = [this] <std::size_t ...I> [[nodiscard]] (const std::index_sequence<I...>) noexcept -> auto {
			const auto &ids{internal::sparse_storage::ids<T...>()};
			return std::tuple<const internal::sparse_map<std::uint32_t, T> &...>{_components.operator[]<T>(ids[I])...};
		}(std::index_sequence_for<T...>{})] [[nodiscard]] (const auto entity) noexcept -> auto {
			return [] <std::size_t ...I> [[nodiscard]] (const auto entity, const auto &components, const std::index_sequence<I...>) noexcept -> auto {
				return std::tuple<std::uint32_t, T &...>{entity, std::get<I>(components)[entity]...};
			}(entity, components, std::index_sequence_for<T...>{});
		}), _execute};
	}

	template <typename ...T>
	constexpr void erase(const std::uint32_t entity) noexcept
		requires (0 < sizeof...(T))
	{
		if (const auto pair{_entities.at(entity)}; (!pair || pair->second) && !_spawned.contains(entity)) {
			return;
		}

		const auto command{[this] (const auto entity) noexcept -> void {
			[this] <std::size_t ...I> (const auto entity, const std::index_sequence<I...>) noexcept -> void {
				const auto &ids{internal::sparse_storage::ids<T...>()};
				(_components.at<T>(ids[I]).erase(entity), ...);
			}(entity, std::index_sequence_for<T...>{});

			cache = _archetypes[_entities[entity].first].first;

			if constexpr (1 < sizeof...(T)) {
				cache.erase(archetype::of<T...>());
			}
			else {
				cache.erase(internal::sparse_storage::ids<T...>().front());
			}

			relocate(entity, _entities[entity].first);
		}};

		if (_lock) {
			_commands.enqueue([cpt = std::pair{command, entity}] noexcept -> void {
				cpt.first(cpt.second);
			});
		}
		else {
			command(entity);
		}
	}

	void despawn(std::uint32_t entity) noexcept;

	void clear() noexcept;

private:
	static thread_local archetype cache;
	internal::sparse_storage _components;
	internal::sparse_set _spawned;
	internal::stack_vector<std::uint32_t, std::uint32_t> _despawned;
	internal::sparse_map<std::uint32_t, std::pair<std::uint16_t, bool>> _entities;
	internal::stack_vector<std::pair<archetype, internal::sparse_set>, std::uint16_t> _archetypes;
	internal::stack_vector<std::uint16_t, std::uint16_t> _queries;
	internal::stack_vector<std::pair<archetype, internal::stack_vector<std::uint16_t>>, std::uint16_t> _indices;
	std::size_t _lock{};
	internal::command_queue _commands;

	std::function<void ()> _execute{[this] noexcept -> void {
		if (!--_lock) {
			_commands.execute();
			_spawned.clear();
		}
	}};

	template <typename T>
	constexpr void relocate(const std::uint32_t entity, T arg) noexcept
	{
		if constexpr (std::is_same_v<T, std::uint16_t>) {
			_archetypes[arg].second.erase(entity);
		}

		const auto pair{std::ranges::find_if(_archetypes, [&] [[nodiscard]] (const auto &pair) noexcept -> auto {
			if constexpr (std::is_same_v<T, const archetype &>) {
				return pair.first == arg;
			}
			else if constexpr (std::is_same_v<T, std::uint16_t>) {
				return pair.first == cache;
			}
		})};

		if (pair == _archetypes.end()) {
			if constexpr (std::is_same_v<T, const archetype &>) {
				_entities.insert(entity, _archetypes.size(), false);
				_archetypes.push(arg, internal::sparse_set{});
			}
			else if constexpr (std::is_same_v<T, std::uint16_t>) {
				_entities[entity].first = _archetypes.size();
				_archetypes.push(cache, internal::sparse_set{});
			}

			(_archetypes.end() - 1)->second.insert(entity);

			for (auto &pair : _indices) {
				if (pair.first <= (_archetypes.end() - 1)->first) {
					pair.second.push(_archetypes.size() - 1);
				}
			}
		}
		else {
			if constexpr (std::is_same_v<T, const archetype &>) {
				_entities.insert(entity, pair - _archetypes.begin(), false);
			}
			else if constexpr (std::is_same_v<T, std::uint16_t>) {
				_entities[entity].first = static_cast<std::uint16_t>(pair - _archetypes.begin());
			}

			pair->second.insert(entity);
		}
	}
};
}

#endif
