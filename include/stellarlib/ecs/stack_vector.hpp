/* clang-format off */

/*
  stellarlib
  Copyright (C) 2025 Domán Zana

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

#ifndef STELLARLIB_ECS_STACK_VECTOR_HPP
#define STELLARLIB_ECS_STACK_VECTOR_HPP

#include <bit>
#include <cstddef>
#include <memory>
#include <utility>

namespace stellarlib::ecs
{
template <typename T, typename size_type = std::size_t>
class stack_vector final : std::allocator<T>
{
public:
	[[nodiscard]]
	explicit constexpr stack_vector() = default;

	[[nodiscard]]
	constexpr stack_vector(const stack_vector<T, size_type> &other)
		: _capacity{other._capacity}
	{
		if (_capacity != 0) {
			_begin = std::allocator<T>::allocate(_capacity);
			std::uninitialized_copy(other._begin, other._end, _begin);
			_size = other._size;
			_end = _begin + _size;
		}
	}

	[[nodiscard]]
	constexpr stack_vector(stack_vector<T, size_type> &&other)
		: _capacity{other._capacity}
		, _size{other._size}
		, _begin{other._begin}
		, _end{other._end}
	{
		other._end = nullptr;
		other._begin = nullptr;
		other._capacity = 0;
	}

	constexpr auto operator=(const stack_vector<T, size_type> &other)
		-> stack_vector<T, size_type> &
	{
		if (std::addressof(other) == this) {
			return *this;
		}

		std::ranges::destroy(*this);

		if (_capacity < other._size) {
			std::allocator<T>::deallocate(_begin, _capacity);
			_capacity = std::bit_ceil(other._size);
			_begin = std::allocator<T>::allocate(_capacity);
		}

		std::uninitialized_copy(other._begin, other._end, _begin);
		_size = other._size;
		_end = _begin + _size;
		return *this;
	}

	constexpr auto operator=(stack_vector<T, size_type> &&other)
		-> stack_vector<T, size_type> &
	{
		if (std::addressof(other) != this) {
			this->~stack_vector();
			new (this) stack_vector<T>{std::move(other)};
		}

		return *this;
	}

	constexpr ~stack_vector()
	{
		std::ranges::destroy(*this);
		std::allocator<T>::deallocate(_begin, _capacity);
	}

	constexpr auto extend(const size_type size)
	{
		if (size <= _size) {
			return false;
		}

		if (_capacity < size) {
			realloc(size);
		}

		_end = _begin + size;
		std::uninitialized_default_construct(_begin + _size, _end);
		_size = size;
		return true;
	}

	template <typename ...Args>
	constexpr void push(Args &&...args)
	{
		if (_size == _capacity) {
			realloc(_capacity + 1);
			_end = _begin + _size;
		}

		new (_end) T{std::forward<Args>(args)...};
		++_end;
		++_size;
	}

	[[nodiscard]]
	constexpr auto size() const
	{
		return _size;
	}

	[[nodiscard]]
	constexpr auto operator[](const size_type i) const
		-> T &
	{
		return _begin[i];
	}

	[[nodiscard]]
	constexpr auto begin() const
	{
		return _begin;
	}

	[[nodiscard]]
	constexpr auto end() const
	{
		return _end;
	}

	constexpr void pop()
	{
		--_size;
		--_end;
		_end->~T();
	}

	constexpr void clear()
	{
		_size = 0;
		std::ranges::destroy(*this);
		_end = _begin;
	}

private:
	size_type _capacity{};
	size_type _size{};
	T *_begin{};
	T *_end{};

	constexpr void realloc(size_type capacity)
	{
		capacity = std::bit_ceil(capacity);
		auto tmp{std::allocator<T>::allocate(capacity)};

		for (auto dst{tmp}, src{_begin}; src != _end; ++dst, ++src) {
			new (dst) T{std::move(*src)};
			src->~T();
		}

		std::allocator<T>::deallocate(_begin, _capacity);
		_capacity = capacity;
		_begin = tmp;
	}
};
}

#endif
