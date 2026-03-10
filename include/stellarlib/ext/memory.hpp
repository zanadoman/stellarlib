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

#include <bit>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <utility>

namespace stellarlib::ext
{
template <typename T, typename SizeType = std::size_t>
class vector_allocator : std::allocator<T>
{
public:
	using value_type = std::allocator<T>::value_type;
	using size_type = SizeType;
	using difference_type = std::allocator<value_type>::difference_type;
	using propagate_on_container_move_assignment = std::allocator<value_type>::propagate_on_container_move_assignment;

	[[nodiscard]]
	explicit constexpr vector_allocator() noexcept = default;

	[[nodiscard]]
	constexpr vector_allocator(const vector_allocator &) noexcept = default;

	[[nodiscard]]
	constexpr vector_allocator(vector_allocator &&) noexcept = default;

	constexpr auto operator=(const vector_allocator &) noexcept
		-> vector_allocator & = default;

	constexpr auto operator=(vector_allocator &&) noexcept
		-> vector_allocator & = default;

	constexpr ~vector_allocator() noexcept = default;

	constexpr void allocate(value_type *&begin, const size_type capacity) const noexcept
	{
		begin = static_cast<value_type *>(std::aligned_alloc(alignof(value_type), capacity * sizeof(value_type)));
	}

	constexpr void reallocate(value_type *&begin, const size_type capacity) const noexcept
	{
		begin = static_cast<value_type *>(std::realloc(begin, capacity * sizeof(value_type)));
	}

	constexpr void reallocate(value_type *&begin, const size_type size, size_type &capacity) const noexcept
	{
		if constexpr (is_trivially_relocatable_v<value_type>) {
			capacity += capacity / 4;
			reallocate(begin, capacity);
		}
		else {
			capacity = std::bit_ceil(capacity);
			const auto dst{static_cast<value_type *>(std::aligned_alloc(alignof(value_type), capacity * sizeof(value_type)))};

			for (const auto src : std::views::iota(begin, begin + size)) {
				std::construct_at(src + (dst - begin), std::move(*src));
				std::destroy_at(src);
			}

			deallocate(begin);
			begin = dst;
		}
	}

	[[nodiscard]]
	constexpr auto operator==(const vector_allocator &) const noexcept
		-> bool = default;

	constexpr void deallocate(value_type *begin) const noexcept
	{
		std::free(begin);
	}
};

class arena final
{
public:
	[[nodiscard]]
	explicit arena() noexcept;

	[[nodiscard]]
	constexpr arena(const arena &) noexcept = delete;

	[[nodiscard]]
	arena(arena &&other) noexcept;

	constexpr auto operator=(const arena &) noexcept
		-> arena & = delete;

	auto operator=(arena &&other) noexcept
		-> arena &;

	~arena() noexcept;

	template <typename T>
	[[nodiscard]]
	constexpr auto allocate() noexcept
	{
		if (const auto ptr{std::align(alignof(T), sizeof(T), _cursor, _capacity)}) {
			_cursor = static_cast<T *>(_cursor) + 1;
			_capacity -= sizeof(T);
			return static_cast<T *>(ptr);
		}

		return static_cast<T *>(nullptr);
	}

	void deallocate() noexcept;

private:
	static std::size_t capacity;
	static std::size_t alignment;
	void *_begin{std::aligned_alloc(alignment, capacity)};
	void *_cursor{_begin};
	std::size_t _capacity{capacity};
};
}

#endif
