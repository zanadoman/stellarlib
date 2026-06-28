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

#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <ranges>
#include <stdexcept>
#include <typeinfo>
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
 * @tparam Base Common interface type or void
 * @tparam Scope Type ID space
 */
template <typename Base, typename Scope = void>
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
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr type_map(const type_map &) noexcept = delete;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr type_map(type_map &&other) = default;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const type_map &) noexcept
		-> type_map & = delete;

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

			if constexpr (std::is_same_v<Base, void>) {
				_elems.emplace_back(new (std::nothrow) T{std::forward<Args>(args)...}, [] (const auto elem) -> void {
					delete static_cast<const T *>(elem);
				});
			}
			else {
				_elems.emplace_back(new (std::nothrow) T{std::forward<Args>(args)...});
			}
		}
		else if constexpr (sizeof...(Args) == 1 && (std::is_assignable_v<T &, Args> && ...)) {
			if constexpr (std::is_same_v<Base, void>) {
				*static_cast<T *>(_elems[_map[id]].get()) = (std::forward<Args>(args), ...);
			}
			else {
				*static_cast<T *>(_elems[_map[id]]) = (std::forward<Args>(args), ...);
			}
		}
		else {
			std::destroy_at(_elems.data() + _map[id]);

			if constexpr (std::is_same_v<Base, void>) {
				std::construct_at(_elems.data() + _map[id], new (std::nothrow) T{std::forward<Args>(args)...}, [] (const auto elem) -> void {
					delete static_cast<const T *>(elem);
				});
			}
			else {
				std::destroy_at(_elems.data() + _map[id]);
				std::construct_at(_elems.data() + _map[id], new (std::nothrow) T{std::forward<Args>(args)...});
			}
		}
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

		if (!contains(id)) {
			throw std::out_of_range{typeid(T).name()};
		}

		if constexpr (std::is_same_v<Base, void>) {
			return *static_cast<const T *>(_elems[_map[id]].get());
		}
		else {
			return *static_cast<const T *>(_elems[_map[id]]);
		}
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

		if (!contains(id)) {
			throw std::out_of_range{typeid(T).name()};
		}

		if constexpr (std::is_same_v<Base, void>) {
			return *static_cast<T *>(_elems[_map[id]].get());
		}
		else {
			return *static_cast<T *>(_elems[_map[id]]);
		}
	}

	/**
	 * @brief Returns a view to the contained elements
	 * @return View to the contained elements
	 */
	[[nodiscard]]
	constexpr auto view() const
	{
		if constexpr (std::is_same_v<Base, void>) {
			return _elems | std::views::transform([] [[nodiscard]] (const auto &elem) -> auto {
				return static_cast<const void *>(elem.get());
			});
		}
		else {
			return _elems | std::views::transform([] [[nodiscard]] (const auto elem) -> const auto & {
				return *static_cast<const Base *>(elem);
			});
		}
	}

	/**
	 * @brief Returns a view to the contained elements
	 * @return View to the contained elements
	 */
	[[nodiscard]]
	constexpr auto view()
	{
		if constexpr (std::is_same_v<Base, void>) {
			return _elems | std::views::transform([] [[nodiscard]] (const auto &elem) -> auto {
				return elem.get();
			});
		}
		else {
			return _elems | std::views::transform([] [[nodiscard]] (const auto elem) -> auto & {
				return *static_cast<Base *>(elem);
			});
		}
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

		if constexpr (!std::is_same_v<Base, void>) {
			delete _elems.back();
		}

		_elems.pop_back();
	}

	/**
	 * @brief Removes all elements from the container
	 */
	constexpr void clear()
	{
		_map.clear();
		_ids.clear();

		if constexpr (!std::is_same_v<Base, void>) {
			for (const auto elem : _elems) {
				delete elem;
			}
		}

		_elems.clear();
	}

private:
	std::vector<std::size_t> _map{};
	std::vector<std::conditional_t<std::is_same_v<Base, void>, std::unique_ptr<void, void (*)(const void *)>, Base *>> _elems{};
	std::vector<std::size_t> _ids{};

	[[nodiscard]]
	constexpr auto contains(const std::size_t id) const
	{
		return id < _map.size() && _map[id] != std::numeric_limits<std::size_t>::max();
	}
};
}

#endif
