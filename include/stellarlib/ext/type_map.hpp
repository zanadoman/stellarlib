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

#ifndef STELLARLIB_EXT_TYPE_MAP_HPP
#define STELLARLIB_EXT_TYPE_MAP_HPP

#include <stellarlib/ext/utility.hpp>

#include <any>
#include <cstddef>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

/**
 * @brief Standard library extensions
 */
namespace stellarlib::ext
{
/**
 * @brief Heterogeneous container
 * @tparam Scope Type ID space
 */
template <typename Scope = void>
class [[nodiscard]] type_map final
{
public:
	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	constexpr type_map() = default;

	/**
	 * @brief Parameterized constructor
	 * @param elems Element instances
	 */
	template <typename ...T>
	[[nodiscard]]
	explicit constexpr type_map(T &&...elems)
	{
		(insert(std::forward<T>(elems)), ...);
	}

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr type_map(const type_map &other) = default;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr type_map(type_map &&other) = default;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(const type_map &other)
		-> type_map & = default;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(type_map &&other)
		-> type_map & = default;

	/**
	 * @brief Destructor
	 */
	constexpr ~type_map() = default;

	/**
	 * @brief Inserts an element into the container
	 * @tparam T Type of the element
	 * @param elem Element instance
	 */
	template <typename T>
	constexpr void insert(T &&elem)
	{
		emplace<T>(std::forward<T>(elem));
	}

	/**
	 * @brief Inserts an element into the container
	 * @tparam T Type of the element
	 * @tparam Args Type of the arguments
	 * @param args Element arguments
	 */
	template <typename T, typename ...Args>
	constexpr void emplace(Args &&...args)
	{
		const auto id{ext::scoped_typeid<Scope, T>()};

		if (_map.size() <= id || _map[id] == std::numeric_limits<std::size_t>::max()) {
			if (_map.size() <= id) {
				_map.resize(id + 1, std::numeric_limits<std::size_t>::max());
			}

			_map[id] = _ids.size();
			_ids.emplace_back(id);
			_elems.emplace_back(T{std::forward<Args>(args)...});
		}
		else if constexpr (sizeof...(Args) == 1 && (std::is_assignable_v<T &, Args> && ...)) {
			std::any_cast<T &>(_elems[_map[id]]) = (std::forward<Args>(args), ...);
		}
		else {
			std::destroy_at(_elems.data() + _map[id]);
			std::construct_at(_elems.data() + _map[id], T{std::forward<Args>(args)...});
		}
	}

	/**
	 * @brief Returns whether the container is empty
	 * @return Whether the container is empty
	 */
	[[nodiscard]]
	constexpr auto empty() const
	{
		return _ids.empty();
	}

	/**
	 * @brief Returns the number of stored elements
	 * @return Number of stored elements
	 */
	[[nodiscard]]
	constexpr auto size() const
	{
		return _ids.size();
	}

	/**
	 * @brief Evaluates whether the container contains an element
	 * @tparam T Type of the element
	 * @return Whether the container contains the element
	 */
	template <typename T>
	[[nodiscard]]
	constexpr auto contains() const
	{
		const auto id{ext::scoped_typeid<Scope, T>()};
		return contains(id);
	}

	/**
	 * @brief Returns a reference to an element
	 * @tparam T Type of the element
	 * @return Reference to the element
	 */
	template <typename T>
	[[nodiscard]]
	constexpr auto at() const
		-> const auto &
	{
		const auto id{ext::scoped_typeid<Scope, T>()};
		return contains(id) ? std::any_cast<const T &>(_elems[_map[id]]) : throw std::bad_any_cast{};
	}

	/**
	 * @brief Returns a reference to an element
	 * @tparam T Type of the element
	 * @return Reference to the element
	 */
	template <typename T>
	[[nodiscard]]
	constexpr auto at()
		-> auto &
	{
		const auto id{ext::scoped_typeid<Scope, T>()};
		return contains(id) ? std::any_cast<T &>(_elems[_map[id]]) : throw std::bad_any_cast{};
	}

	/**
	 * @brief Removes an element from the container
	 * @tparam T Type of the element
	 */
	template <typename T>
	constexpr void erase()
	{
		const auto id{ext::scoped_typeid<Scope, T>()};

		if (!contains(id)) {
			return;
		}

		if (_map[id] != _ids.size() - 1) {
			_elems[_map[id]] = std::move(_elems.back());
			_ids[_map[id]] = _ids.back();
			_map[_ids[_map[id]]] = _map[id];
		}

		_map[id] = std::numeric_limits<std::size_t>::max();
		_ids.pop_back();
		_elems.pop_back();
	}

	/**
	 * @brief Removes all elements from the container
	 */
	constexpr void clear()
	{
		_map.clear();
		_ids.clear();
		_elems.clear();
	}

private:
	std::vector<std::size_t> _map{};
	std::vector<std::any> _elems{};
	std::vector<std::size_t> _ids{};

	[[nodiscard]]
	constexpr auto contains(const std::size_t id) const
	{
		return id < _map.size() && _map[id] != std::numeric_limits<std::size_t>::max();
	}
};

type_map()
	-> type_map<void>;
}

#endif
