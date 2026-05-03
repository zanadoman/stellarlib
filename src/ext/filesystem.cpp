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

#include <stellarlib/ext/filesystem.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

namespace stellarlib::ext::filesystem
{
auto base_directory_path()
	-> const std::filesystem::path &
{
	static const auto cache{[] [[nodiscard]] -> auto {
		if (const auto path{SDL_GetBasePath()}) {
			return std::filesystem::path{path};
		}

		throw std::runtime_error{SDL_GetError()};
	}()};

	return cache;
}

auto data_directory_path(const std::string &organization, const std::string &application)
	-> std::filesystem::path
{
	if (const std::unique_ptr<char, void (*)(void *)> path{SDL_GetPrefPath(organization.c_str(), application.c_str()), SDL_free}) {
		return path.get();
	}

	throw std::runtime_error{SDL_GetError()};
}
}
