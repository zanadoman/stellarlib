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

#include <stellarlib/res/image.hpp>

#include <stellarlib/ext/filesystem.hpp>
#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <ranges>
#include <stdexcept>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(performance-unnecessary-copy-initialization) */

constexpr std::array<std::uint8_t, 24> BYTES{
	255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
	255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255
};

constexpr std::array<lin::uchar4, 6> PIXELS{
	lin::uchar4{255, 0, 0, 255}, lin::uchar4{0, 255, 0, 255}, lin::uchar4{0, 0, 255, 255},
	lin::uchar4{255, 0, 0, 255}, lin::uchar4{0, 255, 0, 255}, lin::uchar4{0, 0, 255, 255}
};

namespace
{
constexpr void check_rgb(const res::image &image)
{
	ASSERT_TRUE(lin::all(image.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image[lin::uint2{0, 0}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image[lin::uint2{0, 1}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image[lin::uint2{1, 0}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image[lin::uint2{1, 1}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image[lin::uint2{2, 0}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_TRUE(lin::all(image[lin::uint2{2, 1}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_EQ(image.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image.bytes(), BYTES));
	ASSERT_EQ(image.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::equal(image.pixels(), PIXELS, [] [[nodiscard]] (const auto lhs, const auto rhs) -> auto {
		return lin::all(lhs == rhs);
	}));
}
}

static_assert(res::image::format == SDL_PIXELFORMAT_ABGR8888);

TEST(stellarlib_res_image, should_init_from_size)
{
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{-1, -1}}), std::invalid_argument);
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{0, 0}}), std::invalid_argument);
	const res::image image{lin::uint2{3, 2}};
	ASSERT_TRUE(lin::all(image.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image[lin::uint2{0, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image[lin::uint2{0, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image[lin::uint2{1, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image[lin::uint2{1, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image[lin::uint2{2, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image[lin::uint2{2, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_EQ(image.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image.bytes(), std::views::repeat(0, 24)));
	ASSERT_EQ(image.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::all_of(image.pixels(), [] [[nodiscard]] (const auto pixel) -> auto {
		return lin::all(pixel == lin::uchar4{0, 0, 0, 0});
	}));
}

TEST(stellarlib_res_image, should_init_from_bytes)
{
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{-1, -1}, BYTES}), std::invalid_argument);
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{0, 0}, BYTES}), std::invalid_argument);
	check_rgb(res::image{lin::uint2{3, 2}, BYTES});
}

TEST(stellarlib_res_image, should_init_from_pixels)
{
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{-1, -1}, PIXELS}), std::invalid_argument);
	ASSERT_THROW(static_cast<void>(res::image{lin::uint2{0, 0}, PIXELS}), std::invalid_argument);
	check_rgb(res::image{lin::uint2{3, 2}, PIXELS});
}

TEST(stellarlib_res_image, should_init_from_path)
{
	ASSERT_THROW(static_cast<void>(res::image{""}), std::runtime_error);
	check_rgb(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"});
}

TEST(stellarlib_res_image, should_copy_via_ctor)
{
	const res::image image1{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"};
	const auto image2{image1};
	check_rgb(image2);
	ASSERT_EQ(image2, image1);
}

TEST(stellarlib_res_image, should_skip_self_copy_via_assignment)
{
	res::image image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"};
	image = image;
	check_rgb(image);
	ASSERT_EQ(image, image);
}

TEST(stellarlib_res_image, should_copy_via_assignment)
{
	const res::image image1{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"};
	res::image image2{lin::uint2{1, 1}};
	image2 = image1;
	check_rgb(image2);
	ASSERT_EQ(image2, image1);
}

TEST(stellarlib_res_image, should_provide_handle)
{
	ASSERT_TRUE(static_cast<SDL_Surface *>(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"}));
}

TEST(stellarlib_res_image, should_apply_repeat)
{
	const res::image input{ext::filesystem::base_directory_path() / "assets" / "tests" / "asymmetric.png"};
	res::image output{input.size() * std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, input.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()) * 2.0F, res::image::address_mode::repeat, res::image::filter::nearest), std::nullopt);
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "repeat.png"});
}

TEST(stellarlib_res_image, should_apply_mirrored_repeat)
{
	const res::image input{ext::filesystem::base_directory_path() / "assets" / "tests" / "asymmetric.png"};
	res::image output{input.size() * std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, input.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()) * 2.0F, res::image::address_mode::mirrored_repeat, res::image::filter::nearest), std::nullopt);
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "mirrored_repeat.png"});
}

TEST(stellarlib_res_image, should_apply_clamp_to_edge)
{
	const res::image input{ext::filesystem::base_directory_path() / "assets" / "tests" / "asymmetric.png"};
	res::image output{input.size() * std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, input.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()) * 2.0F, res::image::address_mode::clamp_to_edge, res::image::filter::nearest), std::nullopt);
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "clamp_to_edge.png"});
}

TEST(stellarlib_res_image, should_apply_nearest_filtering)
{
	const res::image input{ext::filesystem::base_directory_path() / "assets" / "tests" / "symmetric.png"};
	res::image output{input.size() * std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, input.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()), res::image::address_mode::repeat, res::image::filter::nearest), std::nullopt);
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "nearest.png"});
}

TEST(stellarlib_res_image, should_apply_linear_filtering)
{
	const res::image input{ext::filesystem::base_directory_path() / "assets" / "tests" / "symmetric.png"};
	res::image output{input.size() * std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, input.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()), res::image::address_mode::repeat, res::image::filter::linear), std::nullopt);
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "linear.png"});
}

TEST(stellarlib_res_image, should_apply_alpha_blending)
{
	const res::image layer1{ext::filesystem::base_directory_path() / "assets" / "tests" / "layer1.png"};
	const res::image layer2{ext::filesystem::base_directory_path() / "assets" / "tests" / "layer2.png"};
	res::image output{(layer1.size() + layer2.size()) / std::uint32_t{2}};
	for (const auto x : std::views::iota(std::uint32_t{}, output.size().x())) {
		for (const auto y : std::views::iota(std::uint32_t{}, output.size().y())) {
			output.blend(lin::uint2{x, y}, layer1.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()), res::image::address_mode::repeat, res::image::filter::nearest), std::nullopt);
			output.blend(lin::uint2{x, y}, layer2.sample((lin::float2{x, y} + 0.5F) / lin::cast<float>(output.size()), res::image::address_mode::repeat, res::image::filter::nearest), {{
				.src_color_blend_factor = res::image::blend_factor::src_alpha,
				.dst_color_blend_factor = res::image::blend_factor::one_minus_src_alpha,
				.color_blend_op = res::image::blend_op::add,
				.src_alpha_blend_factor = res::image::blend_factor::one,
				.dst_alpha_blend_factor = res::image::blend_factor::one_minus_src_alpha,
				.alpha_blend_op = res::image::blend_op::add
			}});
		}
	}
	ASSERT_EQ(output, res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "alpha_blending.png"});
}

TEST(stellarlib_res_image, should_evaluate_equality)
{
	ASSERT_EQ((res::image{lin::uint2{3, 2}}), (res::image{lin::uint2{3, 2}}));
	ASSERT_EQ((res::image{lin::uint2{3, 2}, BYTES}), (res::image{lin::uint2{3, 2}, PIXELS}));
	ASSERT_EQ((res::image{lin::uint2{3, 2}, PIXELS}), (res::image{lin::uint2{3, 2}, BYTES}));
	ASSERT_EQ((res::image{lin::uint2{3, 2}, BYTES}), res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"});
	ASSERT_EQ(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"}, (res::image{lin::uint2{3, 2}, BYTES}));
	ASSERT_EQ((res::image{lin::uint2{3, 2}, PIXELS}), res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"});
	ASSERT_EQ(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"}, (res::image{lin::uint2{3, 2}, PIXELS}));
	ASSERT_NE((res::image{lin::uint2{3, 2}}), (res::image{lin::uint2{2, 3}}));
	ASSERT_NE((res::image{lin::uint2{2, 3}}), (res::image{lin::uint2{3, 2}}));
	ASSERT_NE((res::image{lin::uint2{3, 2}, BYTES}), (res::image{lin::uint2{3, 2}}));
	ASSERT_NE((res::image{lin::uint2{3, 2}}), (res::image{lin::uint2{3, 2}, BYTES}));
	ASSERT_NE((res::image{lin::uint2{3, 2}, PIXELS}), (res::image{lin::uint2{3, 2}}));
	ASSERT_NE((res::image{lin::uint2{3, 2}}), (res::image{lin::uint2{3, 2}, PIXELS}));
	ASSERT_NE(res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"}, (res::image{lin::uint2{3, 2}}));
	ASSERT_NE((res::image{lin::uint2{3, 2}}), res::image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"});
}

TEST(stellarlib_res_image, should_save)
{
	const res::image image{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"};
	std::filesystem::remove_all(ext::filesystem::base_directory_path() / "tmp");
	ASSERT_THROW(image.save(ext::filesystem::base_directory_path() / "tmp" / "rgb.png"), std::runtime_error);
	std::filesystem::create_directory(ext::filesystem::base_directory_path() / "tmp");
	image.save(ext::filesystem::base_directory_path() / "tmp" / "rgb.png");
	ASSERT_EQ(res::image{ext::filesystem::base_directory_path() / "tmp" / "rgb.png"}, image);
}

TEST(stellarlib_res_image, should_clear)
{
	res::image image{lin::uint2{3, 2}};
	image.clear(BYTES);
	check_rgb(image);
	image.clear(PIXELS);
	check_rgb(image);
}

/* NOLINTEND(performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
