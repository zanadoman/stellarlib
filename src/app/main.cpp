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

#include <stellarlib/app/main.hpp>

#include <stellarlib/app/context.hpp>
#include "context_impl.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#undef main
#include <SDL3/SDL_messagebox.h>

#include <cstdint>
#include <cstdlib>
#include <exception>

namespace stellarlib::app
{
void terminate_handler() noexcept
{
	constexpr auto report{[] (const char *what) noexcept -> void {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", what);
		const auto title{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};

		if (!SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title ? title : "stellarlib", what, nullptr)) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
		}
	}};

	if (const auto exception{std::current_exception()}) {
		try {
			std::rethrow_exception(exception);
		}
		catch (const std::exception &exception) {
			report(exception.what());
		}
		catch (...) {
			report("Unknown exception");
		}
	}
	else {
		report("Unknown error");
	}

	std::abort();
}
}

extern "C"
{
auto SDL_AppInit(void **appstate, const std::int32_t argc, char **argv)
	-> SDL_AppResult
{
	std::set_terminate(stellarlib::app::terminate_handler);

	if (const auto info{stellarlib::app::main({argv, argv + argc})}; info.scene) {
		*appstate = new stellarlib::app::context::impl{info};
		return SDL_APP_CONTINUE;
	}

	return SDL_APP_SUCCESS;
}

auto SDL_AppIterate(void *appstate)
	-> SDL_AppResult
{
	return static_cast<stellarlib::app::context::impl *>(appstate)->iterate();
}

auto SDL_AppEvent(void *appstate, SDL_Event *event)
	-> SDL_AppResult
{
	return static_cast<stellarlib::app::context::impl *>(appstate)->event(event);
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] const SDL_AppResult result)
{
	delete static_cast<const stellarlib::app::context::impl *>(appstate);
}
}
