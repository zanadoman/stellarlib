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

#include <stellarlib/ecs/command_queue.hpp>

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <ranges>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

TEST(stellarlib_ecs_command_queue, should_enqueue_and_execute_commands)
{
	ecs::internal::command_queue commands{};
	std::size_t count{};
	const auto value1{std::make_shared<std::size_t>(1)};
	for (const auto i : std::views::iota(std::uint16_t{}, std::numeric_limits<std::uint16_t>::max())) {
		if (static_cast<bool>(i % 2)) {
			commands.enqueue([&] noexcept -> void {
				++count;
			});
		}
		else {
			commands.enqueue([&] noexcept -> void {
				count += *value1;
			});
		}
	}
	commands.execute();
	ASSERT_EQ(count, std::numeric_limits<std::uint16_t>::max());
	const auto value2{std::make_shared<std::size_t>(2)};
	for (const auto i : std::views::iota(std::uint16_t{}, std::numeric_limits<std::uint16_t>::max())) {
		if (static_cast<bool>(i % 2)) {
			commands.enqueue([&] noexcept -> void {
				count += *value2;
			});
		}
		else {
			commands.enqueue([&] noexcept -> void {
				count += 2;
			});
		}
	}
	commands.execute();
	ASSERT_EQ(count, std::numeric_limits<std::uint16_t>::max() * 3);
}

#pragma clang diagnostic pop
