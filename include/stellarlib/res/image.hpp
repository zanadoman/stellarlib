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

#include <SDL3/SDL_surface.h>

#include <cstdint>
#include <filesystem>
#include <memory>

namespace stellarlib::res
{
class image final
{
public:
	[[nodiscard]]
	explicit image(const std::filesystem::path &path);

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
	auto width()
		-> std::uint32_t;

	[[nodiscard]]
	auto height()
		-> std::uint32_t;

private:
	std::unique_ptr<SDL_Surface, void (*)(SDL_Surface *)> _handle;
};
}

#endif
