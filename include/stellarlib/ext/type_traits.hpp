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

#ifndef STELLARLIB_EXT_TYPE_TRAITS_HPP
#define STELLARLIB_EXT_TYPE_TRAITS_HPP

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>

/**
 * @brief Standard library extensions
 */
namespace stellarlib::ext
{
/**
 * @brief Expands a parameter pack into a repeated type
 * @tparam Pack Parameter pack used to drive expansion
 * @tparam T Type to substitute for each element
 */
template <typename Pack, typename T>
using expand_as = T;

/**
 * @brief Expands a parameter pack into a repeated value
 * @tparam Pack Parameter pack used to drive expansion
 * @tparam VALUE Value to substitute for each element
 */
template <typename Pack, auto VALUE>
static constexpr auto expand_as_v{VALUE};

/**
 * @brief Evaluates whether T is eligible for bit-wise relocation
 * @tparam T Type to evaluate
 */
template <typename T>
using is_trivially_relocatable = std::bool_constant<std::is_trivially_move_constructible_v<T> && std::is_trivially_destructible_v<T>>;

/**
 * @brief Evaluates whether T is eligible for bit-wise relocation
 * @tparam T Type to evaluate
 */
template <typename T>
constexpr bool is_trivially_relocatable_v{is_trivially_relocatable<T>::value};

/**
 * @brief Compile-time padding for explicit field layout alignment
 * @tparam AlignTo Type whose alignment is used as the target
 * @tparam Fields Types whose combined size is being padded
 */
template <typename AlignTo, typename ...Fields>
struct padding final
{
	/**
	 * @brief Size of the padding in bytes
	 */
	static constexpr std::size_t size{(alignof(AlignTo) - (sizeof(Fields) + ...) % alignof(AlignTo)) % alignof(AlignTo)};

	/**
	 * @brief Padding bytes
	 */
	[[no_unique_address]] std::conditional_t<static_cast<bool>(size), std::array<std::byte, size>, std::tuple<>> bytes;
};
}

#endif
