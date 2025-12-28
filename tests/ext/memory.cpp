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

#include <stellarlib/ext/memory.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <string>

using namespace stellarlib::ext;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(cert-err58-cpp,cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes,performance-unnecessary-copy-initialization) */

TEST(stellarlib_ext_memory, should_acquire_and_release_trivial_arena)
{
	const arena_allocator<std::int32_t, uint32_t> allocator{};
	std::int32_t *arena{};
	std::uint32_t size{10};
	allocator.allocate(arena, size);
	std::uninitialized_default_construct_n(arena, size);
	ASSERT_EQ(size, 15);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, should_acquire_and_release_non_trivial_arena)
{
	const arena_allocator<std::string, uint32_t> allocator{};
	std::string *arena{};
	std::uint32_t size{10};
	allocator.allocate(arena, size);
	std::uninitialized_default_construct_n(arena, size);
	ASSERT_EQ(size, 15);
	allocator.deallocate(arena);
}

TEST(stellarlib_ext_memory, should_resize_arena)
{
}

/* NOLINTEND(cert-err58-cpp,cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes,performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
