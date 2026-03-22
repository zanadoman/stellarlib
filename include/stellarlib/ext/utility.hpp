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

#ifndef STELLARLIB_EXT_UTILITY_HPP
#define STELLARLIB_EXT_UTILITY_HPP

#include <atomic>
#include <cstddef>

/**
 * @brief Standard library extensions
 */
namespace stellarlib::ext
{
/**
 * @brief Thread-safe sequential ID generator scoped by type
 * @tparam Scope Distinct type used to separate ID sequences
 * @tparam SizeType Integral type of the ID (default: std::size_t)
 * @return Next ID in the sequence for the given scope
 */
template <typename Scope, typename SizeType = std::size_t>
[[nodiscard]]
constexpr auto sequential_id() noexcept
{
	static std::atomic<SizeType> id{static_cast<SizeType>(-1)};
	return ++id;
}

/**
 * @brief Generates a unique ID per type within a scope
 * @tparam Scope Distinct type used to separate type ID spaces
 * @tparam T Type to retrieve an ID for
 * @tparam SizeType Integral type of the ID (default: std::size_t)
 * @return Unique ID corresponding to type T within the scope
 */
template <typename Scope, typename T, typename SizeType = std::size_t>
[[nodiscard]]
constexpr auto scoped_typeid() noexcept
{
	static const auto id{sequential_id<Scope, SizeType>()};
	return id;
}
}

#endif
