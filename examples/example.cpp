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
 * @example examples/example.cpp
 * @details Blank example
 */

#include <stellarlib/stellarlib.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h> /* IWYU pragma: keep */

#include <cstdint>
#include <cstdlib>
#include <string>

auto main([[maybe_unused]] const std::int32_t argc, [[maybe_unused]] char **argv)
	-> std::int32_t
{
	stellarlib::ecs::world world{};
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s\n", std::get<0>(world.operator[]<std::string>(world.spawn(std::string{"hello, world"}))).c_str());
	SDL_Quit();
	return EXIT_SUCCESS;
}
