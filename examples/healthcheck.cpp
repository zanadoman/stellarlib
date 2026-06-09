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
 * @example examples/healthcheck.cpp
 * @details Runtime healthcheck
 */

#include <stellarlib/ext/filesystem.hpp>
#include <stellarlib/stellarlib.hpp>

#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_log.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunreachable-code"

namespace
{
constexpr void check_metadata(const app::context &ctx)
{
	SDL_assert_always(ctx.metadata().name() == "healthcheck");
	SDL_assert_always(ctx.metadata().version() == "0.1.0");
	SDL_assert_always(ctx.metadata().identifier() == "org.stellarlib.healthcheck");
	SDL_assert_always(ctx.metadata().creator() == "Domán Zana");
	SDL_assert_always(ctx.metadata().copyright() == "Copyright (C) 2025-2026 Domán Zana");
	SDL_assert_always(ctx.metadata().url() == "https://zanadoman.github.io/stellarlib/examples_2healthcheck_8cpp-example.html");
	SDL_assert_always(ctx.metadata().type() == "application");
}

constexpr void check_clock(app::context &ctx)
{
	SDL_assert_always(0.0F < ctx.clock().now());
	SDL_assert_always(ctx.clock().target_frequency() == 60.0F);
	ctx.clock().set_target_frequency(120.0F);
	SDL_assert_always(ctx.clock().target_frequency() == 120.0F);
	SDL_assert_always(ctx.clock().frame() < ctx.clock().now());
	SDL_assert_always(ctx.clock().max_delta() == 0.05F);
	ctx.clock().set_max_delta(0.025F);
	SDL_assert_always(ctx.clock().max_delta() == 0.025F);
	SDL_assert_always(!lin::cast<bool>(ctx.clock().delta()));
}

constexpr void check_window(app::context &ctx)
{
	SDL_assert_always(ctx.window().title() == "org.stellarlib.healthcheck");
	ctx.window().set_title("healthcheck");
	SDL_assert_always(ctx.window().title() == "healthcheck");
	SDL_assert_always(!lin::all(ctx.window().resolution()));
	SDL_assert_always(!ctx.window().focused());
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(ctx.window().renderer()));
	SDL_assert_always(static_cast<std::shared_ptr<SDL_GPUDevice>>(ctx.window().renderer()).get() == static_cast<const SDL_GPUDevice *>(ctx.window().renderer()));
	ctx.window().renderer().set_vsync(!ctx.window().renderer().vsync());
	const res::image image{ext::filesystem::base_directory_path() / "assets" / "tests" / "linear.png"};
	const auto texture1{ctx.window().renderer().upload_image(image, false)};
	SDL_assert_always(static_cast<SDL_GPUTexture *>(texture1));
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(texture1) == static_cast<const SDL_GPUDevice *>(ctx.window().renderer()));
	SDL_assert_always(lin::all(texture1.size() == image.size()));
	SDL_assert_always(!texture1.mipmaps());
	SDL_assert_always(ctx.window().renderer().download_texture(texture1, false) == image);
	const auto texture2{ctx.window().renderer().upload_image(image, true)};
	SDL_assert_always(static_cast<SDL_GPUTexture *>(texture2));
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(texture2) == static_cast<const SDL_GPUDevice *>(ctx.window().renderer()));
	SDL_assert_always(lin::all(texture2.size() == image.size()));
	SDL_assert_always(texture2.mipmaps());
	SDL_assert_always(ctx.window().renderer().download_texture(texture2, true) == image);
}

class [[nodiscard]] blank final : public app::scene
{
public:
	[[nodiscard]]
	constexpr blank()
	{
		SDL_Log("%p: constructor", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr blank(const blank &) noexcept = delete;

	[[nodiscard]]
	constexpr blank(blank &&) noexcept = delete;

	constexpr auto operator=(const blank &) noexcept
		-> blank & = delete;

	constexpr auto operator=(blank &&) noexcept
		-> blank & = delete;

	constexpr ~blank() final
	{
		SDL_Log("%p: destructor", static_cast<const void *>(this));
	}

private:
	constexpr void begin([[maybe_unused]] app::context &ctx) final
	{
		SDL_Log("%p: begin", static_cast<const void *>(this));
	}

	[[nodiscard]]
	constexpr auto update([[maybe_unused]] app::context &ctx)
		-> std::optional<std::unique_ptr<app::scene>> final
	{
		SDL_Log("%p: update", static_cast<const void *>(this));
		return std::nullopt;
	}

	constexpr void end([[maybe_unused]] app::context &ctx) final
	{
		SDL_Log("%p: end", static_cast<const void *>(this));
	}
};
}

auto app::init(const std::vector<std::string> &args)
	-> std::optional<info>
{
	SDL_assert_always(!args.empty());
	SDL_assert_always(!args.front().empty());

	return {{
		.metadata = {
			.name = "healthcheck",
			.version = "0.1.0",
			.identifier = "org.stellarlib.healthcheck",
			.creator = "Domán Zana",
			.copyright = "Copyright (C) 2025-2026 Domán Zana",
			.url = "https://zanadoman.github.io/stellarlib/examples_2healthcheck_8cpp-example.html",
			.type = "application"
		},
		.clock = {
			.target_frequency = 60.0F,
			.max_delta = 0.05F
		},
		.window = {
			.title = "org.stellarlib.healthcheck",
			.icon = res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"},
			.renderer = {
				.vsync = false,
				.debug = true
			}
		},
		.entry = [] [[nodiscard]] (auto &ctx) -> auto {
			check_metadata(ctx);
			check_clock(ctx);
			check_window(ctx);
			return std::make_unique<blank>();
		}
	}};
}

#pragma clang diagnostic pop
