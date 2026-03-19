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

#include <stellarlib/ecs/query.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <ranges>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(cert-err58-cpp,performance-unnecessary-copy-initialization) */

TEST(stellarlib_ecs_query, should_execute_callback)
{
	auto executed{false};
	{
		ecs::internal::query query{std::views::iota(std::uint32_t{}, std::numeric_limits<std::uint32_t>::max()), [&executed] noexcept -> void {
			executed = true;
		}};
		ASSERT_EQ(query.size(), std::views::iota(std::uint32_t{}, std::numeric_limits<std::uint32_t>::max()).size());
		ASSERT_TRUE(std::ranges::equal(query, std::views::iota(std::uint32_t{}, std::numeric_limits<std::uint32_t>::max())));
		ASSERT_FALSE(executed);
	}
	ASSERT_TRUE(executed);
}

/* NOLINTEND(cert-err58-cpp,performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
