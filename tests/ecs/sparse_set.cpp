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

#include <stellarlib/ecs/sparse_set.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ranges>

using namespace stellarlib::ecs;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(cert-err58-cpp,cppcoreguidelines-avoid-do-while,cppcoreguidelines-macro-usage,performance-unnecessary-copy-initialization) */

#define ASSERT_CONTAINS_EQ(set, key, value) do \
{                                              \
	ASSERT_TRUE((set).contains(key));          \
	ASSERT_EQ((set)[key], value);              \
	ASSERT_TRUE((set).at(key));                \
	ASSERT_EQ(*(set).at(key), value);          \
}                                              \
while (false)

#define ASSERT_NOT_CONTAINS(set, key) do \
{                                        \
	ASSERT_FALSE((set).contains(key));   \
	ASSERT_FALSE((set).at(key));         \
}                                        \
while (false)

constexpr std::array<std::size_t, 5>                                KEYS{2, 1, 0, 3, 4};
static const std::array<std::shared_ptr<std::int32_t>, KEYS.size()> VALUES{
	std::make_shared<std::int32_t>(0),
	std::make_shared<std::int32_t>(5),
	std::make_shared<std::int32_t>(10),
	std::make_shared<std::int32_t>(15),
	std::make_shared<std::int32_t>(25)
};

namespace
{
void check_values_mut(sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_CONTAINS_EQ(set, KEYS[0], VALUES[0]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	ASSERT_CONTAINS_EQ(set, KEYS[2], VALUES[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	ASSERT_CONTAINS_EQ(set, KEYS[4], VALUES[4]);
}

void check_values_const(const sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_CONTAINS_EQ(set, KEYS[0], VALUES[0]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	ASSERT_CONTAINS_EQ(set, KEYS[2], VALUES[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	ASSERT_CONTAINS_EQ(set, KEYS[4], VALUES[4]);
}

void check_iters_mut(sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_TRUE(std::ranges::equal(set.keys(), KEYS));
	ASSERT_TRUE(std::ranges::equal(set.values(), VALUES));
	ASSERT_TRUE(std::ranges::equal(set.zip(), std::ranges::views::zip(KEYS, VALUES)));
}

void check_iters_const(const sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_TRUE(std::ranges::equal(set.keys(), KEYS));
	ASSERT_TRUE(std::ranges::equal(set.values(), VALUES));
	ASSERT_TRUE(std::ranges::equal(set.zip(), std::ranges::views::zip(KEYS, VALUES)));
}

void erase_forward(sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_CONTAINS_EQ(set, KEYS[0], VALUES[0]);
	set.erase(KEYS[0]);
	ASSERT_NOT_CONTAINS(set, KEYS[0]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	set.erase(KEYS[1]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	ASSERT_CONTAINS_EQ(set, KEYS[2], VALUES[2]);
	set.erase(KEYS[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	set.erase(KEYS[3]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	ASSERT_CONTAINS_EQ(set, KEYS[4], VALUES[4]);
	set.erase(KEYS[4]);
	ASSERT_NOT_CONTAINS(set, KEYS[4]);
}

void erase_backward(sparse_set<std::shared_ptr<std::int32_t>> &set)
{
	ASSERT_CONTAINS_EQ(set, KEYS[4], VALUES[4]);
	set.erase(KEYS[4]);
	ASSERT_NOT_CONTAINS(set, KEYS[4]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	set.erase(KEYS[3]);
	ASSERT_NOT_CONTAINS(set, KEYS[3]);
	ASSERT_CONTAINS_EQ(set, KEYS[2], VALUES[2]);
	set.erase(KEYS[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[2]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	set.erase(KEYS[1]);
	ASSERT_NOT_CONTAINS(set, KEYS[1]);
	ASSERT_CONTAINS_EQ(set, KEYS[0], VALUES[0]);
	set.erase(KEYS[0]);
	ASSERT_NOT_CONTAINS(set, KEYS[0]);
}
}

/* NOLINTEND(cert-err58-cpp,cppcoreguidelines-avoid-do-while,cppcoreguidelines-macro-usage,performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
