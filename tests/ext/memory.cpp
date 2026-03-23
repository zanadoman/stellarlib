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
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

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
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 2);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 3);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 5);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 7);
	allocator.reallocate(arena, 0, ++capacity);
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
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 2);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 4);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 8);
	allocator.reallocate(arena, 0, ++capacity);
	ASSERT_EQ(capacity, 16);
	allocator.reallocate(arena, 0, ++capacity);
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

static_assert(std::is_same_v<ext::arena::value_type, std::allocator<void>::value_type>);
static_assert(std::is_same_v<ext::arena::size_type, std::allocator<void>::size_type>);
static_assert(std::is_same_v<ext::arena::difference_type, std::allocator<void>::difference_type>);
static_assert(std::is_same_v<ext::arena::propagate_on_container_move_assignment, std::allocator<void>::propagate_on_container_move_assignment>);

TEST(stellarlib_ext_memory, arena_should_move_via_ctor)
{
	ext::arena arena1{0};
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
	ext::arena arena{0};
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
	ext::arena arena1{0};
	const auto number1{arena1.allocate<std::int32_t>()};
	const auto string1{arena1.allocate<std::string>()};
	const auto string2{arena1.allocate<std::string>()};
	const auto number2{arena1.allocate<std::int32_t>()};
	ext::arena arena2{0};
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

TEST(stellarlib_ext_memory, arena_should_have_capacity)
{
	ASSERT_TRUE(ext::arena{0}.capacity());
}

TEST(stellarlib_ext_memory, arena_should_allocate_and_deallocate)
{
	ext::arena arena{0};
	const auto number1{arena.allocate<std::int32_t>()};
	ASSERT_TRUE(number1);
	ASSERT_FALSE(std::bit_cast<std::size_t>(number1) % alignof(std::int32_t));
	const auto string1{arena.allocate<std::string>()};
	ASSERT_TRUE(string1);
	ASSERT_EQ(std::bit_cast<const std::byte *>(string1) - std::bit_cast<const std::byte *>(number1), sizeof(std::int32_t) + (ext::padding<std::string, std::int32_t>::size));
	ASSERT_FALSE((arena.allocate<std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>>)());
	const auto string2{arena.allocate<std::string>()};
	ASSERT_TRUE(string2);
	ASSERT_EQ(std::bit_cast<const std::byte *>(string2) - std::bit_cast<const std::byte *>(string1), sizeof(std::string) + (ext::padding<std::string, std::string>::size));
	const auto number2{arena.allocate<std::int32_t>()};
	ASSERT_TRUE(number2);
	ASSERT_EQ(std::bit_cast<const std::byte *>(number2) - std::bit_cast<const std::byte *>(string2), sizeof(std::string) + (ext::padding<std::int32_t, std::string>::size));
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

TEST(stellarlib_ext_memory, arena_should_evaluate_equality)
{
	const ext::arena arena1{0};
	const ext::arena arena2{0};
	ASSERT_EQ(arena1, arena1);
	ASSERT_EQ(arena2, arena2);
	ASSERT_NE(arena1, arena2);
	ASSERT_NE(arena2, arena1);
}

static_assert(std::is_same_v<ext::arena_allocator::value_type, std::allocator<void>::value_type>);
static_assert(std::is_same_v<ext::arena_allocator::size_type, std::allocator<void>::size_type>);
static_assert(std::is_same_v<ext::arena_allocator::difference_type, std::allocator<void>::difference_type>);
static_assert(std::is_same_v<ext::arena_allocator::propagate_on_container_move_assignment, std::allocator<void>::propagate_on_container_move_assignment>);

TEST(stellarlib_ext_memory, arena_allocator_should_move_via_ctor)
{
	ext::arena_allocator allocator1{};
	const auto number1{allocator1.allocate<std::int32_t>()};
	const auto string1{allocator1.allocate<std::string>()};
	const auto string2{allocator1.allocate<std::string>()};
	const auto number2{allocator1.allocate<std::int32_t>()};
	auto allocator2{std::move(allocator1)};
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
	allocator2.deallocate();
	ASSERT_EQ(allocator2.allocate<std::int32_t>(), number1);
	ASSERT_EQ(allocator2.allocate<std::string>(), string1);
	ASSERT_EQ(allocator2.allocate<std::string>(), string2);
	ASSERT_EQ(allocator2.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_allocator_should_skip_self_move_via_assignment)
{
	ext::arena_allocator allocator{};
	const auto number1{allocator.allocate<std::int32_t>()};
	const auto string1{allocator.allocate<std::string>()};
	const auto string2{allocator.allocate<std::string>()};
	const auto number2{allocator.allocate<std::int32_t>()};
	allocator = std::move(allocator);
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
	allocator.deallocate();
	ASSERT_EQ(allocator.allocate<std::int32_t>(), number1);
	ASSERT_EQ(allocator.allocate<std::string>(), string1);
	ASSERT_EQ(allocator.allocate<std::string>(), string2);
	ASSERT_EQ(allocator.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_allocator_should_move_via_assignment)
{
	ext::arena_allocator allocator1{};
	const auto number1{allocator1.allocate<std::int32_t>()};
	const auto string1{allocator1.allocate<std::string>()};
	const auto string2{allocator1.allocate<std::string>()};
	const auto number2{allocator1.allocate<std::int32_t>()};
	ext::arena_allocator allocator2{};
	allocator2 = std::move(allocator1);
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
	allocator2.deallocate();
	ASSERT_EQ(allocator2.allocate<std::int32_t>(), number1);
	ASSERT_EQ(allocator2.allocate<std::string>(), string1);
	ASSERT_EQ(allocator2.allocate<std::string>(), string2);
	ASSERT_EQ(allocator2.allocate<std::int32_t>(), number2);
}

TEST(stellarlib_ext_memory, arena_allocator_should_allocate_and_deallocate)
{
	ext::arena_allocator allocator{};
	auto number1{allocator.allocate<std::int32_t>()};
	ASSERT_TRUE(number1);
	ASSERT_FALSE(std::bit_cast<std::size_t>(number1) % alignof(std::int32_t));
	auto string1{allocator.allocate<std::string>()};
	ASSERT_TRUE(string1);
	ASSERT_EQ(std::bit_cast<const std::byte *>(string1) - std::bit_cast<const std::byte *>(number1), sizeof(std::int32_t) + (ext::padding<std::string, std::int32_t>::size));
	auto array{allocator.allocate<std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>>()};
	ASSERT_TRUE(array);
	ASSERT_FALSE(std::bit_cast<std::size_t>(array) % alignof(std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>));
	auto string2{allocator.allocate<std::string>()};
	ASSERT_TRUE(string2);
	ASSERT_FALSE(std::bit_cast<std::size_t>(string2) % alignof(std::string));
	auto number2{allocator.allocate<std::int32_t>()};
	ASSERT_TRUE(number2);
	ASSERT_EQ(std::bit_cast<const std::byte *>(number2) - std::bit_cast<const std::byte *>(string2), sizeof(std::string) + (ext::padding<std::int32_t, std::string>::size));
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
	allocator.deallocate();
	std::vector<void *> ptrs{};
	for (const auto i : std::views::iota(std::uint16_t{}, std::numeric_limits<std::uint16_t>::max())) {
		if (static_cast<bool>(i % 2)) {
			ptrs.emplace_back(allocator.allocate<std::int32_t>());
			ASSERT_TRUE(ptrs.back());
			std::construct_at(static_cast<std::int32_t *>(ptrs.back()), i);
		}
		else {
			ptrs.emplace_back(allocator.allocate<std::string>());
			ASSERT_TRUE(ptrs.back());
			std::construct_at(static_cast<std::string *>(ptrs.back()), std::to_string(i));
		}
	}
	for (const auto i : std::views::iota(std::uint16_t{}, std::numeric_limits<std::uint16_t>::max())) {
		if (static_cast<bool>(i % 2)) {
			ASSERT_EQ(*static_cast<const std::int32_t *>(ptrs[i]), i);
			std::destroy_at(static_cast<std::int32_t *>(ptrs[i]));
		}
		else {
			ASSERT_EQ(*static_cast<const std::string *>(ptrs[i]), std::to_string(i));
			std::destroy_at(static_cast<std::string *>(ptrs[i]));
		}
	}
	allocator.deallocate();
	number1 = allocator.allocate<std::int32_t>();
	ASSERT_TRUE(number1);
	ASSERT_FALSE(std::bit_cast<std::size_t>(number1) % alignof(std::int32_t));
	string1 = allocator.allocate<std::string>();
	ASSERT_TRUE(string1);
	ASSERT_EQ(std::bit_cast<const std::byte *>(string1) - std::bit_cast<const std::byte *>(number1), sizeof(std::int32_t) + (ext::padding<std::string, std::int32_t>::size));
	array = allocator.allocate<std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>>();
	ASSERT_TRUE(array);
	ASSERT_EQ(std::bit_cast<const std::byte *>(array) - std::bit_cast<const std::byte *>(string1), sizeof(std::string) + (ext::padding<std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>, std::string>::size));
	string2 = allocator.allocate<std::string>();
	ASSERT_TRUE(string2);
	ASSERT_EQ(std::bit_cast<const std::byte *>(string2) - std::bit_cast<const std::byte *>(array), sizeof(std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>) + (ext::padding<std::string, std::array<std::byte, std::numeric_limits<std::uint16_t>::max()>>::size));
	number2 = allocator.allocate<std::int32_t>();
	ASSERT_TRUE(number2);
	ASSERT_EQ(std::bit_cast<const std::byte *>(number2) - std::bit_cast<const std::byte *>(string2), sizeof(std::string) + (ext::padding<std::int32_t, std::string>::size));
}

TEST(stellarlib_ext_memory, arena_allocator_should_evaluate_equality)
{
	const ext::arena_allocator allocator1{};
	const ext::arena_allocator allocator2{};
	ASSERT_EQ(allocator1, allocator1);
	ASSERT_EQ(allocator2, allocator2);
	ASSERT_NE(allocator1, allocator2);
	ASSERT_NE(allocator2, allocator1);
}

#pragma clang diagnostic pop
