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

#include <stellarlib/ext/memory.hpp>

#include <stellarlib/ext/type_traits.hpp>

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(cert-err58-cpp,performance-unnecessary-copy-initialization) */

static_assert(std::is_same_v<ext::vector_allocator<std::int32_t>::value_type, std::allocator<std::int32_t>::value_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::int32_t>::size_type, std::allocator<std::int32_t>::size_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::int32_t>::difference_type, std::allocator<std::int32_t>::difference_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::int32_t>::propagate_on_container_move_assignment, std::allocator<std::int32_t>::propagate_on_container_move_assignment>);
static_assert(std::is_same_v<ext::vector_allocator<std::string>::value_type, std::allocator<std::string>::value_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::string>::size_type, std::allocator<std::string>::size_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::string>::difference_type, std::allocator<std::string>::difference_type>);
static_assert(std::is_same_v<ext::vector_allocator<std::string>::propagate_on_container_move_assignment, std::allocator<std::string>::propagate_on_container_move_assignment>);

TEST(stellarlib_ext_memory, vector_allocator_should_acquire_and_release_trivial_arena)
{
	const ext::vector_allocator<std::int32_t> allocator{};
	std::int32_t *arena{};
	const auto size{100};
	allocator.allocate(arena, size);
	ASSERT_TRUE(arena);
	std::uninitialized_fill_n(arena, size, -1);
	std::destroy_n(arena, size);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_acquire_and_release_non_trivial_arena)
{
	const ext::vector_allocator<std::string> allocator{};
	std::string *arena{};
	const auto size{100};
	allocator.allocate(arena, size);
	ASSERT_TRUE(arena);
	std::uninitialized_fill_n(arena, size, std::to_string(-1));
	std::destroy_n(arena, size);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_handle_unsafe_arena)
{
	const ext::vector_allocator<std::int32_t> allocator{};
	std::int32_t *arena{};
	const std::size_t size1{100};
	allocator.allocate(arena, size1);
	for (const auto i : std::views::iota(std::size_t{}, size1)) {
		std::construct_at(arena + i, i);
	}
	const std::size_t size2{125};
	allocator.reallocate(arena, size2);
	ASSERT_TRUE(arena);
	std::uninitialized_fill(arena + size1, arena + size2, -1);
	for (const auto i : std::views::iota(std::size_t{}, size1)) {
		ASSERT_EQ(arena[i], i);
	}
	const std::size_t size3{62};
	allocator.reallocate(arena, size3);
	ASSERT_TRUE(arena);
	for (const auto i : std::views::iota(std::size_t{}, size3)) {
		ASSERT_EQ(arena[i], i);
	}
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_grow_trivial_arena)
{
	const ext::vector_allocator<std::int32_t> allocator{};
	std::int32_t *arena{};
	std::size_t capacity{1};
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 1);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 2);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 3);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 5);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 7);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 10);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_resize_trivial_arena)
{
	const ext::vector_allocator<std::int32_t> allocator{};
	std::int32_t *arena{};
	std::size_t size{100};
	allocator.allocate(arena, size);
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		std::construct_at(arena + i, i);
	}
	auto capacity{size};
	allocator.reallocate(arena, size, capacity);
	ASSERT_TRUE(arena);
	ASSERT_EQ(capacity, 125);
	std::uninitialized_fill(arena + size, arena + capacity, -1);
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		ASSERT_EQ(arena[i], i);
	}
	size = 50;
	std::destroy(arena + size, arena + capacity);
	capacity = size;
	allocator.reallocate(arena, size, capacity);
	ASSERT_TRUE(arena);
	ASSERT_EQ(capacity, 62);
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		ASSERT_EQ(arena[i], i);
	}
	std::destroy_n(arena, size);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_grow_non_trivial_arena)
{
	const ext::vector_allocator<std::string> allocator{};
	std::string *arena{};
	std::size_t capacity{1};
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 1);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 2);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 4);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 8);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 16);
	++capacity;
	allocator.reallocate(arena, 0, capacity);
	ASSERT_EQ(capacity, 32);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, vector_allocator_should_resize_non_trivial_arena)
{
	const ext::vector_allocator<std::string> allocator{};
	std::string *arena{};
	std::size_t size{100};
	allocator.allocate(arena, size);
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		std::construct_at(arena + i, std::to_string(i));
	}
	auto capacity{size};
	allocator.reallocate(arena, size, capacity);
	ASSERT_TRUE(arena);
	ASSERT_EQ(capacity, 128);
	std::uninitialized_fill(arena + size, arena + capacity, std::to_string(-1));
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		ASSERT_EQ(arena[i], std::to_string(i));
	}
	size = 50;
	std::destroy(arena + size, arena + capacity);
	capacity = size;
	allocator.reallocate(arena, size, capacity);
	ASSERT_TRUE(arena);
	ASSERT_EQ(capacity, 64);
	for (const auto i : std::views::iota(std::size_t{}, size)) {
		ASSERT_EQ(arena[i], std::to_string(i));
	}
	std::destroy_n(arena, size);
	allocator.deallocate(arena);
}

static_assert(ext::vector_allocator<std::int32_t>{} == ext::vector_allocator<std::int32_t>{});
static_assert(!(ext::vector_allocator<std::int32_t>{} != ext::vector_allocator<std::int32_t>{}));
static_assert(ext::vector_allocator<std::string>{} == ext::vector_allocator<std::string>{});
static_assert(!(ext::vector_allocator<std::string>{} != ext::vector_allocator<std::string>{}));

TEST(stellarlib_ext_memory, arena_should_move_via_ctor)
{
	ext::arena arena1{};
	const auto number1{arena1.allocate<std::int32_t>()};
	const auto string1{arena1.allocate<std::string>()};
	const auto string2{arena1.allocate<std::string>()};
	const auto number2{arena1.allocate<std::int32_t>()};
	auto arena2{std::move(arena1)};
	std::construct_at(number1, 5);
	std::construct_at(string1, "hello");
	std::construct_at(string2, "world");
	std::construct_at(number2, 10);
	ASSERT_EQ(*number1, 5);
	ASSERT_EQ(*string1, "hello");
	ASSERT_EQ(*string2, "world");
	ASSERT_EQ(*number2, 10);
	std::destroy_at(number2);
	std::destroy_at(string2);
	std::destroy_at(string1);
	std::destroy_at(number1);
	arena2.deallocate();
	ASSERT_EQ(arena2.allocate<std::int32_t>(), number1);
	ASSERT_EQ(arena2.allocate<std::string>(), string1);
	ASSERT_EQ(arena2.allocate<std::string>(), string2);
	ASSERT_EQ(arena2.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_should_skip_self_move_via_assignment)
{
	ext::arena arena{};
	const auto number1{arena.allocate<std::int32_t>()};
	const auto string1{arena.allocate<std::string>()};
	const auto string2{arena.allocate<std::string>()};
	const auto number2{arena.allocate<std::int32_t>()};
	arena = std::move(arena);
	std::construct_at(number1, 5);
	std::construct_at(string1, "hello");
	std::construct_at(string2, "world");
	std::construct_at(number2, 10);
	ASSERT_EQ(*number1, 5);
	ASSERT_EQ(*string1, "hello");
	ASSERT_EQ(*string2, "world");
	ASSERT_EQ(*number2, 10);
	std::destroy_at(number2);
	std::destroy_at(string2);
	std::destroy_at(string1);
	std::destroy_at(number1);
	arena.deallocate();
	ASSERT_EQ(arena.allocate<std::int32_t>(), number1);
	ASSERT_EQ(arena.allocate<std::string>(), string1);
	ASSERT_EQ(arena.allocate<std::string>(), string2);
	ASSERT_EQ(arena.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_should_move_via_assignment)
{
	ext::arena arena1{};
	const auto number1{arena1.allocate<std::int32_t>()};
	const auto string1{arena1.allocate<std::string>()};
	const auto string2{arena1.allocate<std::string>()};
	const auto number2{arena1.allocate<std::int32_t>()};
	ext::arena arena2{};
	arena2 = std::move(arena1);
	std::construct_at(number1, 5);
	std::construct_at(string1, "hello");
	std::construct_at(string2, "world");
	std::construct_at(number2, 10);
	ASSERT_EQ(*number1, 5);
	ASSERT_EQ(*string1, "hello");
	ASSERT_EQ(*string2, "world");
	ASSERT_EQ(*number2, 10);
	std::destroy_at(number2);
	std::destroy_at(string2);
	std::destroy_at(string1);
	std::destroy_at(number1);
	arena2.deallocate();
	ASSERT_EQ(arena2.allocate<std::int32_t>(), number1);
	ASSERT_EQ(arena2.allocate<std::string>(), string1);
	ASSERT_EQ(arena2.allocate<std::string>(), string2);
	ASSERT_EQ(arena2.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_should_allocate_and_deallocate)
{
	ext::arena arena{};
	const auto number1{arena.allocate<std::int32_t>()};
	ASSERT_TRUE(number1);
	ASSERT_FALSE(reinterpret_cast<std::size_t>(number1) % alignof(std::int32_t));
	const auto string1{arena.allocate<std::string>()};
	ASSERT_TRUE(string1);
	ASSERT_EQ(reinterpret_cast<const std::byte *>(string1) - reinterpret_cast<const std::byte *>(number1), sizeof(std::int32_t) + (ext::padding<std::string, std::int32_t>::size));
	ASSERT_FALSE((arena.allocate<std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>>)());
	const auto string2{arena.allocate<std::string>()};
	ASSERT_TRUE(string2);
	ASSERT_EQ(reinterpret_cast<const std::byte *>(string2) - reinterpret_cast<const std::byte *>(string1), sizeof(std::string) + (ext::padding<std::string, std::string>::size));
	const auto number2{arena.allocate<std::int32_t>()};
	ASSERT_TRUE(number2);
	ASSERT_EQ(reinterpret_cast<const std::byte *>(number2) - reinterpret_cast<const std::byte *>(string2), sizeof(std::string) + (ext::padding<std::int32_t, std::string>::size));
	std::construct_at(number1, 5);
	std::construct_at(string1, "hello");
	std::construct_at(string2, "world");
	std::construct_at(number2, 10);
	ASSERT_EQ(*number1, 5);
	ASSERT_EQ(*string1, "hello");
	ASSERT_EQ(*string2, "world");
	ASSERT_EQ(*number2, 10);
	std::destroy_at(number2);
	std::destroy_at(string2);
	std::destroy_at(string1);
	std::destroy_at(number1);
	arena.deallocate();
	ASSERT_EQ(arena.allocate<std::int32_t>(), number1);
	ASSERT_EQ(arena.allocate<std::string>(), string1);
	ASSERT_EQ(arena.allocate<std::string>(), string2);
	ASSERT_EQ(arena.allocate<std::int32_t>(), number2);
}

/* NOLINTEND(cert-err58-cpp,performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
