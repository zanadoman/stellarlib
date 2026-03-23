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

#ifndef STELLARLIB_ECS_ARCHETYPE_HPP
#define STELLARLIB_ECS_ARCHETYPE_HPP

#include <stellarlib/ecs/sparse_storage.hpp>
#include <stellarlib/ext/memory.hpp>

#include <cstdint>
#include <type_traits>

/**
 * @brief Entity component system
 */
namespace stellarlib::ecs
{
/**
 * @brief Represents the archetype of an entity
 */
class archetype final : ext::vector_allocator<std::uintmax_t>
{
public:
	/**
	 * @brief Retrieves an archetype for a set of component types
	 * @tparam T Component types
	 * @return Archetype for a set of component types
	 */
	template <typename ...T>
	[[nodiscard]]
	static constexpr auto of() noexcept
		-> std::conditional_t<0 < sizeof...(T), const archetype &, archetype>
	{
		if constexpr (0 < sizeof...(T)) {
			static const auto cache{[] [[nodiscard]] noexcept -> archetype {
				archetype archetype{};

				for (const auto id : internal::sparse_storage::ids<T...>()) {
					archetype.insert(id);
				}

				return archetype;
			}()};

			return cache;
		}
		else {
			return archetype{};
		}
	}

	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	explicit archetype() noexcept;

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	archetype(const archetype &other) noexcept;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	archetype(archetype &&other) noexcept;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(const archetype &other) noexcept
		-> archetype &;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(archetype &&other) noexcept
		-> archetype &;

	/**
	 * @brief Destructor
	 */
	~archetype() noexcept;

	/**
	 * @brief Inserts a component ID into the archetype
	 * @param id Component ID
	 */
	void insert(std::uintmax_t id) noexcept;

	/**
	 * @brief Inserts all component IDs that exist in another instance into the archetype
	 * @param other Other instance
	 */
	void insert(const archetype &other) noexcept;

	/**
	 * @brief Evaluates whether the archetype contains a component ID
	 * @param id Component ID
	 * @return Whether the archetype contains the component ID
	 */
	[[nodiscard]]
	auto contains(std::uintmax_t id) const noexcept
		-> bool;

	/**
	 * @brief Comparison operator
	 * @param other Other instance
	 * @return Result of the comparison
	 */
	[[nodiscard]]
	auto operator==(const archetype &other) const noexcept
		-> bool;

	/**
	 * @brief Evaluates whether the current instance is the subtype of an other instance
	 * @param other Other instance
	 * @return Whether the current instance is the subtype of the other instance
	 */
	[[nodiscard]]
	auto operator<=(const archetype &other) const noexcept
		-> bool;

	/**
	 * @brief Evaluates whether the current instance is the supertype of an other instance
	 * @param other Other instance
	 * @return Whether the current instance is the supertype of the other instance
	 */
	[[nodiscard]]
	auto operator>=(const archetype &other) const noexcept
		-> bool;

	/**
	 * @brief Removes a component ID from the archetype
	 * @param id Component ID
	 */
	void erase(std::uintmax_t id) noexcept;

	/**
	 * @brief Removes all component IDs that exist in another instance from the archetype
	 * @param other Other instance
	 */
	void erase(const archetype &other) noexcept;

	/**
	 * @brief Removes all component IDs from the archetype
	 */
	void clear() noexcept;

private:
	std::uintmax_t *_begin{};
	std::uintmax_t *_end{};
	std::uintmax_t _size{};
	std::uintmax_t _capacity{};
};
}

#endif
