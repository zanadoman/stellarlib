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

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <ranges>
#include <span>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

constexpr std::array<std::uint8_t, 24> BYTES{
	255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255,
	255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255
};

constexpr std::array<lin::uchar4, 6> PIXELS{
	lin::uchar4{255, 0, 0, 255}, lin::uchar4{0, 255, 0, 255}, lin::uchar4{0, 0, 255, 255},
	lin::uchar4{255, 0, 0, 255}, lin::uchar4{0, 255, 0, 255}, lin::uchar4{0, 0, 255, 255}
};

TEST(stellarlib_res_image, should_construct)
{
	const res::image image1{lin::uint2{3, 2}};
	ASSERT_TRUE(lin::all(image1.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{0, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{0, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{1, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{1, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{2, 0}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_TRUE(lin::all(image1[lin::uint2{2, 1}] == lin::uchar4{0, 0, 0, 0}));
	ASSERT_EQ(image1.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image1.bytes(), std::views::repeat(0, 24)));
	ASSERT_EQ(image1.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::all_of(image1.pixels(), [] [[nodiscard]] (const auto pixel) -> auto {
		return lin::all(pixel == lin::uchar4{0, 0, 0, 0});
	}));
	const res::image image2{lin::uint2{3, 2}, lin::uchar4{255, 255, 255, 255}};
	ASSERT_TRUE(lin::all(image2.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{0, 0}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{0, 1}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{1, 0}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{1, 1}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{2, 0}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_TRUE(lin::all(image2[lin::uint2{2, 1}] == lin::uchar4{255, 255, 255, 255}));
	ASSERT_EQ(image2.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image2.bytes(), std::views::repeat(255, 24)));
	ASSERT_EQ(image2.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::all_of(image2.pixels(), [] [[nodiscard]] (const auto pixel) -> auto {
		return lin::all(pixel == lin::uchar4{255, 255, 255, 255});
	}));
	const res::image image3{lin::uint2{3, 2}, std::span{BYTES}};
	ASSERT_TRUE(lin::all(image3.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{0, 0}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{0, 1}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{1, 0}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{1, 1}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{2, 0}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_TRUE(lin::all(image3[lin::uint2{2, 1}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_EQ(image3.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image3.bytes(), BYTES));
	ASSERT_EQ(image3.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::equal(image3.pixels(), PIXELS, [] [[nodiscard]] (const auto lhs, const auto rhs) -> auto {
		return lin::all(lhs == rhs);
	}));
	const res::image image4{lin::uint2{3, 2}, std::span{PIXELS}};
	ASSERT_TRUE(lin::all(image4.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{0, 0}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{0, 1}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{1, 0}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{1, 1}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{2, 0}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_TRUE(lin::all(image4[lin::uint2{2, 1}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_EQ(image4.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image4.bytes(), BYTES));
	ASSERT_EQ(image4.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::equal(image4.pixels(), PIXELS, [] [[nodiscard]] (const auto lhs, const auto rhs) -> auto {
		return lin::all(lhs == rhs);
	}));
	const res::image image5{ext::filesystem::base_directory_path() / "assets" / "tests" / "rgb.png"};
	ASSERT_TRUE(lin::all(image5.size() == lin::uint2{3, 2}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{0, 0}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{0, 1}] == lin::uchar4{255, 0, 0, 255}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{1, 0}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{1, 1}] == lin::uchar4{0, 255, 0, 255}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{2, 0}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_TRUE(lin::all(image5[lin::uint2{2, 1}] == lin::uchar4{0, 0, 255, 255}));
	ASSERT_EQ(image5.bytes().size(), 24);
	ASSERT_TRUE(std::ranges::equal(image5.bytes(), BYTES));
	ASSERT_EQ(image5.pixels().size(), 6);
	ASSERT_TRUE(std::ranges::equal(image5.pixels(), PIXELS, [] [[nodiscard]] (const auto lhs, const auto rhs) -> auto {
		return lin::all(lhs == rhs);
	}));
}

#pragma clang diagnostic pop
