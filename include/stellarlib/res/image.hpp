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

#ifndef STELLARLIB_RES_IMAGE_HPP
#define STELLARLIB_RES_IMAGE_HPP

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>

namespace stellarlib::res
{
class image final
{
public:
	[[nodiscard]]
	explicit image(const std::filesystem::path &path);

	[[nodiscard]]
	explicit image(const lin::ushort2 &size);

	[[nodiscard]]
	image(const image &other);

	[[nodiscard]]
	image(image &&other) noexcept;

	auto operator=(const image &other)
		-> image &;

	auto operator=(image &&other) noexcept
		-> image &;

	~image();

	[[nodiscard]]
	auto size() const
		-> lin::ushort2;

	[[nodiscard]]
	auto operator[](const lin::ushort2 &coord) const
		-> const lin::uchar4 &;

	[[nodiscard]]
	auto operator[](const lin::ushort2 &coord)
		-> lin::uchar4 &;

	[[nodiscard]]
	auto data() const
		-> std::span<const std::uint8_t>;

	[[nodiscard]]
	auto data()
		-> std::span<std::uint8_t>;

	[[nodiscard]]
	auto sample(lin::float2 coord)
		-> lin::float4;

	void paint(const lin::ushort2 &coord, lin::float4 color);

	void save(const std::filesystem::path &path) const;

private:
	std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> _handle;
};
}

#endif
