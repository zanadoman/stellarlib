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

#ifndef STELLARLIB_APP_METADATA_HPP
#define STELLARLIB_APP_METADATA_HPP

#include <stellarlib/app/subsystem.hpp>

#include <string>

namespace stellarlib::app
{
class metadata final : public internal::subsystem<metadata>
{
friend internal::subsystem<metadata>;

public:
	struct info final
	{
		std::string name;
		std::string version;
		std::string identifier;
		std::string creator;
		std::string copyright;
		std::string url;
		std::string type;
	};

	[[nodiscard]]
	constexpr metadata(const metadata &) noexcept = delete;

	[[nodiscard]]
	constexpr metadata(metadata &&) noexcept = delete;

	constexpr auto operator=(const metadata &) noexcept
		-> metadata & = delete;

	constexpr auto operator=(metadata &&) noexcept
		-> metadata & = delete;

	~metadata();

	[[nodiscard]]
	auto name() const
		-> std::string;

	[[nodiscard]]
	auto version() const
		-> std::string;

	[[nodiscard]]
	auto identifier() const
		-> std::string;

	[[nodiscard]]
	auto creator() const
		-> std::string;

	[[nodiscard]]
	auto copyright() const
		-> std::string;

	[[nodiscard]]
	auto url() const
		-> std::string;

	[[nodiscard]]
	auto type() const
		-> std::string;

private:
	[[nodiscard]]
	explicit metadata(const info &info);
};
}

#endif
