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

#ifndef STELLARLIB_EXT_MEMORY_HPP
#define STELLARLIB_EXT_MEMORY_HPP

#include <stellarlib/ext/type_traits.hpp>

#include <SDL3/SDL_stdinc.h>

#include <bit>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <utility>

/**
 * @brief Standard library extensions
 */
namespace stellarlib::ext
{
/**
 * @brief Linear memory allocator optimized for vectors
 * @tparam T Value type of the allocator
 * @tparam SizeType Size type of the allocator
 */
template <typename T, typename SizeType = std::size_t>
class vector_allocator : std::allocator<T>
{
public:
	/**
	 * @brief Value type of the allocator
	 */
	using value_type = std::allocator<T>::value_type;

	/**
	 * @brief Size type of the allocator
	 */
	using size_type = SizeType;

	/**
	 * @brief Difference type of the allocator
	 */
	using difference_type = std::allocator<value_type>::difference_type;

	/**
	 * @brief Whether the allocator can be propagated on container move assignment
	 */
	using propagate_on_container_move_assignment = std::allocator<value_type>::propagate_on_container_move_assignment;

	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	constexpr vector_allocator() noexcept = default;

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr vector_allocator(const vector_allocator &other) noexcept = default;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	constexpr vector_allocator(vector_allocator &&other) noexcept = default;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(const vector_allocator &other) noexcept
		-> vector_allocator & = default;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	constexpr auto operator=(vector_allocator &&other) noexcept
		-> vector_allocator & = default;

	/**
	 * @brief Destructor
	 */
	constexpr ~vector_allocator() noexcept = default;

	/**
	 * @brief Allocates uninitialized memory for N instances of type T
	 * @param begin Updated to point to the allocated memory
	 * @param capacity Number of instances to allocate
	 */
	constexpr void allocate(value_type *&begin, const size_type capacity) const noexcept
	{
		begin = static_cast<value_type *>(std::malloc(capacity * sizeof(value_type)));
	}

	/**
	 * @brief Reallocates uninitialized memory for N instances of type T via bit-wise relocation
	 * @param begin Pointer for existing memory; Updated to point to the reallocated memory
	 * @param capacity Number of instances to allocate
	 */
	constexpr void reallocate(value_type *&begin, const size_type capacity) const noexcept
	{
		begin = static_cast<value_type *>(std::realloc(begin, capacity * sizeof(value_type)));
	}

	/**
	 * @brief Reallocates uninitialized memory for at least N instances of type T via element-wise move
	 * @param begin Pointer for existing memory; Updated to point to the reallocated memory
	 * @param size Number of initialized instances in existing memory
	 * @param capacity Requested capacity; Updated to the actual allocated capacity
	 */
	constexpr void reallocate(value_type *&begin, const size_type size, size_type &capacity) const noexcept
	{
		if constexpr (is_trivially_relocatable_v<value_type>) {
			capacity += capacity / 4;
			reallocate(begin, capacity);
		}
		else {
			capacity = std::bit_ceil(capacity);
			const auto dst{static_cast<value_type *>(std::malloc(capacity * sizeof(value_type)))};

			for (const auto src : std::views::iota(begin, begin + size)) {
				std::construct_at(src + (dst - begin), std::move(*src));
				std::destroy_at(src);
			}

			deallocate(begin);
			begin = dst;
		}
	}

	/**
	 * @brief Comparison operator
	 * @param other Other instance
	 * @return Result of the comparison
	 */
	[[nodiscard]]
	constexpr auto operator==(const vector_allocator &other) const noexcept
		-> bool = default;

	/**
	 * @brief Deallocates uninitialized memory
	 * @param begin Pointer for existing memory
	 */
	constexpr void deallocate(value_type *begin) const noexcept
	{
		std::free(begin);
	}
};

/**
 * @brief Fixed-size, page-aligned memory arena
 */
class arena : std::allocator<void>
{
public:
	/**
	 * @brief Value type of the allocator
	 */
	using value_type = std::allocator<void>::value_type;

	/**
	 * @brief Size type of the allocator
	 */
	using size_type = std::allocator<void>::size_type;

	/**
	 * @brief Difference type of the allocator
	 */
	using difference_type = std::allocator<void>::difference_type;

	/**
	 * @brief Whether the allocator can be propagated on container move assignment
	 */
	using propagate_on_container_move_assignment = std::allocator<void>::propagate_on_container_move_assignment;

	/**
	 * @brief Parameterized constructor
	 * @param capacity Requested capacity of the arena in bytes
	 */
	[[nodiscard]]
	explicit arena(size_type capacity) noexcept;

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr arena(const arena &) noexcept = delete;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	arena(arena &&other) noexcept;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const arena &) noexcept
		-> arena & = delete;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(arena &&other) noexcept
		-> arena &;

	/**
	 * @brief Destructor
	 */
	~arena() noexcept;

	/**
	 * @brief Returns the remaining capacity of the arena in bytes
	 * @return Remaining capacity of the arena in bytes
	 */
	[[nodiscard]]
	auto capacity() const noexcept
		-> size_type;

	/**
	 * @brief Returns uninitialized memory for type T
	 * @tparam T Type to determine alignment and size
	 * @return Uninitialized memory for type T, or nullptr if arena is exhausted
	 */
	template <typename T>
	[[nodiscard]]
	constexpr auto allocate() noexcept
	{
		if (const auto ptr{std::align(alignof(T), sizeof(T), _cursor, _size)}) {
			_cursor = static_cast<T *>(_cursor) + 1;
			_size -= sizeof(T);
			return static_cast<T *>(ptr);
		}

		return static_cast<T *>(nullptr);
	}

	/**
	 * @brief Comparison operator
	 * @param other Other instance
	 * @return Result of the comparison
	 */
	[[nodiscard]]
	auto operator==(const arena &other) const noexcept
		-> bool;

	/**
	 * @brief Resets the arena
	 */
	void deallocate() noexcept;

private:
	static size_type page_capacity;
	static size_type page_alignment;
	size_type _capacity;
	value_type *_cursor{SDL_aligned_alloc(page_alignment, _capacity)};
	size_type _size{_capacity};
	value_type *_begin{_cursor};
};

/**
 * @brief Dynamic arena allocator
 */
class arena_allocator : std::allocator<void>
{
public:
	/**
	 * @brief Value type of the allocator
	 */
	using value_type = std::allocator<void>::value_type;

	/**
	 * @brief Size type of the allocator
	 */
	using size_type = std::allocator<void>::size_type;

	/**
	 * @brief Difference type of the allocator
	 */
	using difference_type = std::allocator<void>::difference_type;

	/**
	 * @brief Whether the allocator can be propagated on container move assignment
	 */
	using propagate_on_container_move_assignment = std::allocator<void>::propagate_on_container_move_assignment;

	/**
	 * @brief Default constructor
	 */
	[[nodiscard]]
	arena_allocator() noexcept;

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr arena_allocator(const arena_allocator &) noexcept = delete;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	arena_allocator(arena_allocator &&other) noexcept;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const arena_allocator &) noexcept
		-> arena_allocator & = delete;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(arena_allocator &&other) noexcept
		-> arena_allocator &;

	/**
	 * @brief Destructor
	 */
	~arena_allocator() noexcept;

	/**
	 * @brief Returns uninitialized memory for type T
	 * @tparam T Type to determine alignment and size
	 * @return Uninitialized memory for type T
	 */
	template <typename T>
	[[nodiscard]]
	constexpr auto allocate() noexcept
	{
		if (const auto ptr{_begin[_cursor].allocate<T>()}) {
			return ptr;
		}

		_begin = static_cast<arena *>(std::realloc(_begin, (++_cursor + 1) * sizeof(arena)));
		std::construct_at(_begin + _cursor, sizeof(T) <= _capacity ? _capacity : sizeof(T));
		_capacity += _begin[_cursor].capacity();
		return _begin[_cursor].allocate<T>();
	}

	/**
	 * @brief Comparison operator
	 * @param other Other instance
	 * @return Result of the comparison
	 */
	[[nodiscard]]
	auto operator==(const arena_allocator &other) const noexcept
		-> bool;

	/**
	 * @brief Resets the arena
	 */
	void deallocate() noexcept;

private:
	arena *_begin{std::construct_at(static_cast<arena *>(std::malloc(sizeof(arena))), 0)};
	size_type _cursor{};
	size_type _capacity{_begin->capacity()};
};
}

#endif
