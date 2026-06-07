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

#include <stellarlib/ext/type_map.hpp>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

using namespace stellarlib;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#pragma clang diagnostic ignored "-Wself-move"

/* NOLINTBEGIN(performance-unnecessary-copy-initialization) */

namespace
{
class [[nodiscard]] base
{
public:
	virtual constexpr ~base() = default;

	[[nodiscard]]
	virtual constexpr auto value() const
		-> std::uint64_t = 0;

protected:
	[[nodiscard]]
	constexpr base() = default;

	[[nodiscard]]
	constexpr base(const base &) = default;

	[[nodiscard]]
	constexpr base(base &&) = default;

	constexpr auto operator=(const base &)
		-> base & = default;

	constexpr auto operator=(base &&)
		-> base & = default;
};

class [[nodiscard]] derived1 final : public base
{
public:
	[[nodiscard]]
	explicit constexpr derived1(const std::uint64_t value)
		: _value{value}
	{}

	[[nodiscard]]
	constexpr derived1(const derived1 &) noexcept = delete;

	[[nodiscard]]
	constexpr derived1(derived1 &&) = default;

	constexpr auto operator=(const derived1 &) noexcept
		-> derived1 & = delete;

	constexpr auto operator=(derived1 &&)
		-> derived1 & = default;

	constexpr ~derived1() final = default;

	[[nodiscard]]
	constexpr auto value() const
		-> std::uint64_t final
	{
		return _value;
	}

private:
	std::uint64_t _value{};
};

class [[nodiscard]] derived2 final : public base
{
public:
	[[nodiscard]]
	explicit constexpr derived2(const std::uint64_t value)
		: _value{value}
	{}

	[[nodiscard]]
	constexpr derived2(const derived2 &) noexcept = delete;

	[[nodiscard]]
	constexpr derived2(derived2 &&) = default;

	constexpr auto operator=(const derived2 &) noexcept
		-> derived2 & = delete;

	constexpr auto operator=(derived2 &&)
		-> derived2 & = default;

	constexpr ~derived2() final = default;

	[[nodiscard]]
	constexpr auto value() const
		-> std::uint64_t final
	{
		return _value * 2;
	}

private:
	std::uint64_t _value{};
};

template <typename Base>
constexpr void check_elems(const ext::type_map<Base> &map)
{
	ASSERT_TRUE(map.template contains<derived1>());
	ASSERT_TRUE(map.template contains<derived2>());
	ASSERT_EQ(map.template at<derived1>().value(), 5);
	ASSERT_EQ(map.template at<derived2>().value(), 20);
	ASSERT_EQ(map.view().size(), 2);
	if constexpr (std::is_same_v<Base, void>) {
		ASSERT_EQ(map.view().front(), std::addressof(map.template at<derived1>()));
		ASSERT_EQ(map.view().back(), std::addressof(map.template at<derived2>()));
	}
	else {
		ASSERT_EQ(map.view().front().value(), 5);
		ASSERT_EQ(map.view().back().value(), 20);
	}
}
}

TEST(stellarlib_ext_type_map, should_init_from_elems)
{
	check_elems(ext::type_map<void>{derived1{5}, derived2{10}});
	check_elems(ext::type_map<base>{derived1{5}, derived2{10}});
}

TEST(stellarlib_ext_type_map, should_insert_emplace_and_erase_elems)
{
	ext::type_map<void> map1{};
	ASSERT_FALSE(map1.contains<derived1>());
	ASSERT_FALSE(map1.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map1.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map1.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map1.view().empty());
	map1.insert(derived1{0});
	map1.insert(derived1{5});
	ASSERT_TRUE(map1.contains<derived1>());
	ASSERT_FALSE(map1.contains<derived2>());
	ASSERT_EQ(map1.at<derived1>().value(), 5);
	ASSERT_THROW(static_cast<void>(map1.at<derived2>()), std::out_of_range);
	ASSERT_EQ(map1.view().size(), 1);
	ASSERT_EQ(map1.view().front(), std::addressof(map1.at<derived1>()));
	map1.emplace<derived2>(std::uint64_t{0});
	map1.emplace<derived2>(std::uint64_t{10});
	check_elems(map1);
	map1.erase<derived2>();
	map1.erase<derived2>();
	ASSERT_TRUE(map1.contains<derived1>());
	ASSERT_FALSE(map1.contains<derived2>());
	ASSERT_EQ(map1.at<derived1>().value(), 5);
	ASSERT_THROW(static_cast<void>(map1.at<derived2>()), std::out_of_range);
	ASSERT_EQ(map1.view().size(), 1);
	ASSERT_EQ(map1.view().front(), std::addressof(map1.at<derived1>()));
	map1.erase<derived1>();
	map1.erase<derived1>();
	ASSERT_FALSE(map1.contains<derived1>());
	ASSERT_FALSE(map1.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map1.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map1.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map1.view().empty());
	ext::type_map<base> map2{};
	ASSERT_FALSE(map2.contains<derived1>());
	ASSERT_FALSE(map2.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map2.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map2.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map2.view().empty());
	map2.insert(derived1{0});
	map2.insert(derived1{5});
	ASSERT_TRUE(map2.contains<derived1>());
	ASSERT_FALSE(map2.contains<derived2>());
	ASSERT_EQ(map2.at<derived1>().value(), 5);
	ASSERT_THROW(static_cast<void>(map2.at<derived2>()), std::out_of_range);
	ASSERT_EQ(map2.view().size(), 1);
	ASSERT_EQ(map2.view().front().value(), map2.at<derived1>().value());
	map2.emplace<derived2>(std::uint64_t{0});
	map2.emplace<derived2>(std::uint64_t{10});
	check_elems(map2);
	map2.erase<derived2>();
	map2.erase<derived2>();
	ASSERT_TRUE(map2.contains<derived1>());
	ASSERT_FALSE(map2.contains<derived2>());
	ASSERT_EQ(map2.at<derived1>().value(), 5);
	ASSERT_THROW(static_cast<void>(map2.at<derived2>()), std::out_of_range);
	ASSERT_EQ(map2.view().size(), 1);
	ASSERT_EQ(map2.view().front().value(), map2.at<derived1>().value());
	map2.erase<derived1>();
	map2.erase<derived1>();
	ASSERT_FALSE(map2.contains<derived1>());
	ASSERT_FALSE(map2.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map2.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map2.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map2.view().empty());
}

TEST(stellarlib_ext_type_map, should_clear_elems)
{
	ext::type_map<void> map1{derived1{5}, derived2{10}};
	map1.clear();
	ASSERT_FALSE(map1.contains<derived1>());
	ASSERT_FALSE(map1.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map1.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map1.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map1.view().empty());
	ext::type_map<base> map2{derived1{5}, derived2{10}};
	map2.clear();
	ASSERT_FALSE(map2.contains<derived1>());
	ASSERT_FALSE(map2.contains<derived2>());
	ASSERT_THROW(static_cast<void>(map2.at<derived1>()), std::out_of_range);
	ASSERT_THROW(static_cast<void>(map2.at<derived2>()), std::out_of_range);
	ASSERT_TRUE(map2.view().empty());
}

/* NOLINTEND(performance-unnecessary-copy-initialization) */

#pragma clang diagnostic pop
