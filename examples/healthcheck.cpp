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

#include <SDL3/SDL_gpu.h>

#include <cassert>
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
	assert(ctx.metadata().name() == "healthcheck");
	assert(ctx.metadata().version() == "0.1.0");
	assert(ctx.metadata().identifier() == "org.stellarlib.healthcheck");
	assert(ctx.metadata().creator() == "Domán Zana");
	assert(ctx.metadata().copyright() == "Copyright (C) 2025-2026 Domán Zana");
	assert(ctx.metadata().url() == "https://zanadoman.github.io/stellarlib/examples_2healthcheck_8cpp-example.html");
	assert(ctx.metadata().type() == "application");
}

constexpr void check_clock(app::context &ctx)
{
	assert(0.0F < ctx.clock().now());
	assert(ctx.clock().target_frequency() == 60.0F);
	ctx.clock().set_target_frequency(120.0F);
	assert(ctx.clock().target_frequency() == 120.0F);
	assert(ctx.clock().frame() < ctx.clock().now());
	assert(ctx.clock().max_delta() == 0.05F);
	ctx.clock().set_max_delta(0.025F);
	assert(ctx.clock().max_delta() == 0.025F);
	assert(!lin::cast<bool>(ctx.clock().delta()));
}

constexpr void check_window(app::context &ctx)
{
	assert(ctx.window().title() == "org.stellarlib.healthcheck");
	ctx.window().set_title("healthcheck");
	assert(ctx.window().title() == "healthcheck");
	ctx.window().set_vsync(!ctx.window().vsync());
	const res::image image{ext::filesystem::base_directory_path() / "assets" / "tests" / "linear.png"};
	const auto texture1{ctx.window().upload_image(image, false)};
	assert(static_cast<SDL_GPUTexture *>(texture1));
	assert(lin::all(texture1.size() == image.size()));
	assert(!texture1.mipmaps());
	assert(ctx.window().download_texture(texture1) == image);
	const auto texture2{ctx.window().upload_image(image, true)};
	assert(static_cast<SDL_GPUTexture *>(texture2));
	assert(lin::all(texture2.size() == image.size()));
	assert(!texture2.mipmaps());
	assert(ctx.window().download_texture(texture2) == image);
}
}

auto app::init(const std::vector<std::string> &args)
	-> std::optional<info>
{
	assert(!args.empty());
	assert(!args.front().empty());

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
			.debug = true
		},
		.entry = [] [[nodiscard]] (auto &ctx) -> auto {
			check_metadata(ctx);
			check_clock(ctx);
			check_window(ctx);
			return nullptr;
		}
	}};
}

#pragma clang diagnostic pop
