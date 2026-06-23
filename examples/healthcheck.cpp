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
constexpr void check_metadata(const app::metadata &metadata)
{
	SDL_assert_always(metadata.name() == "healthcheck");
	SDL_assert_always(metadata.version() == "0.1.0");
	SDL_assert_always(metadata.identifier() == "org.stellarlib.healthcheck");
	SDL_assert_always(metadata.creator() == "Domán Zana");
	SDL_assert_always(metadata.copyright() == "Copyright (C) 2025-2026 Domán Zana");
	SDL_assert_always(metadata.url() == "https://zanadoman.github.io/stellarlib/examples_2healthcheck_8cpp-example.html");
	SDL_assert_always(metadata.type() == "application");
}

constexpr void check_clock(app::clock &clock)
{
	SDL_assert_always(0.0F < clock.now());
	SDL_assert_always(clock.target_frequency() == 60.0F);
	clock.set_target_frequency(120.0F);
	SDL_assert_always(clock.target_frequency() == 120.0F);
	SDL_assert_always(clock.frame() < clock.now());
	SDL_assert_always(clock.max_delta() == 0.05F);
	clock.set_max_delta(0.025F);
	SDL_assert_always(clock.max_delta() == 0.025F);
	SDL_assert_always(!lin::cast<bool>(clock.delta()));
}

constexpr void check_window(app::window &window)
{
	SDL_assert_always(window.title() == "org.stellarlib.healthcheck");
	window.set_title("healthcheck");
	SDL_assert_always(window.title() == "healthcheck");
	SDL_assert_always(!lin::all(window.resolution()));
	SDL_assert_always(!window.focused());
}

constexpr void check_renderer(gfx::renderer &renderer)
{
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(renderer));
	SDL_assert_always(static_cast<std::shared_ptr<SDL_GPUDevice>>(renderer).get() == static_cast<const SDL_GPUDevice *>(renderer));
	SDL_assert_always(!renderer.min_aspect());
	renderer.set_min_aspect(16.0F / 10.0F);
	SDL_assert_always(renderer.min_aspect() == 16.0F / 10.0F);
	SDL_assert_always(!renderer.max_aspect());
	renderer.set_max_aspect(32.0F / 9.0F);
	SDL_assert_always(renderer.max_aspect() == 32.0F / 9.0F);
	SDL_assert_always(renderer.max_resolution());
	SDL_assert_always(lin::all(*renderer.max_resolution() == (lin::uint2{1920, 1080})));
	renderer.set_max_resolution(std::nullopt);
	SDL_assert_always(!renderer.max_resolution());
	SDL_assert_always(renderer.filter() == res::image::filter::nearest);
	renderer.set_filter(res::image::filter::linear);
	SDL_assert_always(renderer.filter() == res::image::filter::linear);
	SDL_assert_always(renderer.presentation() == gfx::renderer::presentation::stretch);
	renderer.set_presentation(gfx::renderer::presentation::letterbox);
	SDL_assert_always(renderer.presentation() == gfx::renderer::presentation::letterbox);
	renderer.set_vsync(!renderer.vsync());
	const res::image image{ext::filesystem::base_directory_path() / "assets" / "tests" / "linear.png"};
	const auto texture1{renderer.upload_image(image, false)};
	SDL_assert_always(static_cast<SDL_GPUTexture *>(texture1));
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(texture1) == static_cast<const SDL_GPUDevice *>(renderer));
	SDL_assert_always(lin::all(texture1.size() == image.size()));
	SDL_assert_always(!texture1.mipmaps());
	SDL_assert_always(texture1.levels() == 1);
	gfx::texture texture2{static_cast<std::shared_ptr<SDL_GPUDevice>>(renderer), texture1.size(), texture1.mipmaps()};
	renderer.blit_texture(
		{
			.src_texture = texture1,
			.src_area = {
				.s = lin::float2{1.0F, 1.0F}
			},
			.dst_texture = texture2,
			.dst_area = {
				.s = lin::float2{1.0F, 1.0F}
			}
		},
		true
	);
	SDL_assert_always(renderer.download_texture(texture2, false) == image);
	const auto texture3{renderer.upload_image(image, true)};
	SDL_assert_always(static_cast<SDL_GPUTexture *>(texture3));
	SDL_assert_always(static_cast<const SDL_GPUDevice *>(texture3) == static_cast<const SDL_GPUDevice *>(renderer));
	SDL_assert_always(lin::all(texture3.size() == image.size()));
	SDL_assert_always(texture3.mipmaps());
	SDL_assert_always(texture3.levels() == 3);
	gfx::texture texture4{static_cast<std::shared_ptr<SDL_GPUDevice>>(renderer), texture3.size(), texture3.mipmaps()};
	renderer.blit_texture(
		{
			.src_texture = texture3,
			.src_area = {
				.s = lin::float2{1.0F, 1.0F}
			},
			.dst_texture = texture4,
			.dst_area = {
				.s = lin::float2{1.0F, 1.0F}
			}
		},
		true
	);
	SDL_assert_always(renderer.download_texture(texture4, true) == image);
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
	constexpr void begin(app::context &ctx) final
	{
		SDL_Log("%p: begin", static_cast<const void *>(this));
		_texture = std::make_unique<gfx::texture>(ctx.window().renderer().upload_image(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "alpha_blending.png"}, false));
	}

	[[nodiscard]]
	constexpr auto update(app::context &ctx)
		-> std::optional<std::unique_ptr<app::scene>> final
	{
		SDL_Log("%p: update", static_cast<const void *>(this));

		if (auto &framebuffer{ctx.window().renderer().framebuffer()}) {
			ctx.window().renderer().blit_texture(
				{
					.src_texture = *_texture,
					.src_area = {
						.s = lin::float2{1.0F, 1.0F}
					},
					.dst_texture = *framebuffer,
					.dst_area = {
						.s = lin::float2{1.0F, 1.0F}
					}
				},
				false
			);
		}

		return std::nullopt;
	}

	constexpr void end([[maybe_unused]] app::context &ctx) final
	{
		SDL_Log("%p: end", static_cast<const void *>(this));
	}

	std::unique_ptr<gfx::texture> _texture{};
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
				.max_resolution = lin::uint2{1920, 1080},
				.presentation = gfx::renderer::presentation::stretch,
				.vsync = false,
				.validation = true
			}
		},
		.entry = [] [[nodiscard]] (auto &ctx) -> auto {
			check_metadata(ctx.metadata());
			check_clock(ctx.clock());
			check_window(ctx.window());
			check_renderer(ctx.window().renderer());
			return std::make_unique<blank>();
		}
	}};
}

#pragma clang diagnostic pop
