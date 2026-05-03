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

/**
 * @example examples/scenes.cpp
 * @details Scenes example
 */

#include <stellarlib/stellarlib.hpp>

#include <SDL3/SDL_log.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace stellarlib;

namespace
{
class scene2 final : public app::scene
{
public:
	[[nodiscard]]
	constexpr scene2() noexcept
	{
		SDL_Log("%p: constructor", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr scene2(const scene2 &) noexcept = delete;

	[[nodiscard]]
	constexpr scene2(scene2 &&) noexcept = delete;

	constexpr auto operator=(const scene2 &) noexcept
		-> scene2 & = delete;

	constexpr auto operator=(scene2 &&) noexcept
		-> scene2 & = delete;

	constexpr ~scene2() noexcept final
	{
		SDL_Log("%p: destructor", static_cast<const void *>(this));
	}

private:
	constexpr void begin([[maybe_unused]] app::context &ctx) noexcept final
	{
		SDL_Log("%p: begin", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr auto update([[maybe_unused]] app::context &ctx) noexcept
		-> std::optional<std::unique_ptr<app::scene>> final
	{
		SDL_Log("%p: update", static_cast<const void *>(this));
		return nullptr;
	}

	constexpr void end([[maybe_unused]] app::context &ctx) noexcept final
	{
		SDL_Log("%p: end", static_cast<const void *>(this));
	}
};

class scene1 final : public app::scene
{
public:
	[[nodiscard]]
	constexpr scene1() noexcept
	{
		SDL_Log("%p: constructor", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr scene1(const scene1 &) noexcept = delete;

	[[nodiscard]]
	constexpr scene1(scene1 &&) noexcept = delete;

	constexpr auto operator=(const scene1 &) noexcept
		-> scene1 & = delete;

	constexpr auto operator=(scene1 &&) noexcept
		-> scene1 & = delete;

	constexpr ~scene1() noexcept final
	{
		SDL_Log("%p: destructor", static_cast<const void *>(this));
	}

private:
	constexpr void begin([[maybe_unused]] app::context &ctx) noexcept final
	{
		SDL_Log("%p: begin", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr auto update([[maybe_unused]] app::context &ctx)
		-> std::optional<std::unique_ptr<app::scene>> final
	{
		SDL_Log("%p: update", static_cast<const void *>(this));
		return std::make_unique<scene2>();
	}

	constexpr void end([[maybe_unused]] app::context &ctx) noexcept final
	{
		SDL_Log("%p: end", static_cast<const void *>(this));
	}
};
}

auto app::main(const std::vector<std::string> &args)
	-> std::optional<app::info>
{
	SDL_Log("%s", args.front().c_str());

	return {{
		.metadata = {
			.name = "scenes",
			.version = "0.1.0",
			.identifier = "org.stellarlib.scenes",
			.creator = "Domán Zana",
			.copyright = "Copyright (C) 2025-2026 Domán Zana",
			.url = "https://zanadoman.github.io/stellarlib/examples_2scenes_8cpp-example.html",
			.type = "application"
		},
		.clock = {
			.target_frequency = 60.0F,
			.max_delta = 0.05F
		},
		.main = std::make_unique<scene1>()
	}};
}
