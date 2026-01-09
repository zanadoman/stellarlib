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

#include <cstdint>
#include <cstdlib>

#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_error.h>

auto main([[maybe_unused]] const std::int32_t argc, [[maybe_unused]] char **argv)
	-> std::int32_t
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		/* NOLINTNEXTLINE */
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s\n", SDL_GetError());
		std::abort();
    }

	SDL_Window *window{};
	SDL_Renderer *renderer{};

    if (!SDL_CreateWindowAndRenderer("Android", 1920, 1080, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		/* NOLINTNEXTLINE */
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s\n", SDL_GetError());
		std::abort();
    }

    if (!SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255)) {
		/* NOLINTNEXTLINE */
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s\n", SDL_GetError());
		std::abort();
    }

    while (!SDL_HasEvent(SDL_EVENT_QUIT)) {
		if (!SDL_RenderClear(renderer)) {
			/* NOLINTNEXTLINE */
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s\n", SDL_GetError());
			std::abort();
		}


		if (!SDL_RenderPresent(renderer)) {
			/* NOLINTNEXTLINE */
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s\n", SDL_GetError());
			std::abort();
		}

        SDL_PumpEvents();
		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
