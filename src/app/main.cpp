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
#include <stellarlib/app/bridge.hpp>

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
class main final
{
public:
	[[nodiscard]]
	constexpr main() noexcept = delete;

	[[nodiscard]]
	constexpr main(const main &) noexcept = delete;

	[[nodiscard]]
	constexpr main(main &&) noexcept = delete;

	constexpr auto operator=(const main &) noexcept
		-> main & = delete;

	constexpr auto operator=(main &&) noexcept
		-> main & = delete;

	constexpr ~main() noexcept = delete;

	[[nodiscard]]
	static constexpr auto init(void **appstate, const std::int32_t argc, char **argv)
	{
		std::set_terminate([] [[noreturn]] () noexcept -> void {
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
		});

		if (const auto info{app::main({argv, argv + argc})}) {
			*appstate = new context{internal::bridge<main>::init<context>(*info)};
			return SDL_APP_CONTINUE;
		}

		return SDL_APP_SUCCESS;
	}

	[[nodiscard]]
	static constexpr auto iterate(void *appstate)
	{
		return internal::bridge<main>::iterate(*static_cast<stellarlib::app::context *>(appstate));
	}

	[[nodiscard]]
	static constexpr auto event(void *appstate, SDL_Event *event)
	{
		return internal::bridge<main>::event(*static_cast<stellarlib::app::context *>(appstate), event);
	}

	static constexpr void quit(void *appstate, [[maybe_unused]] const SDL_AppResult result)
	{
		delete static_cast<const stellarlib::app::context *>(appstate);
	}
};
}

extern "C"
{
auto SDL_AppInit(void **appstate, const std::int32_t argc, char **argv)
	-> SDL_AppResult
{
	return stellarlib::app::main::init(appstate, argc, argv);
}

auto SDL_AppIterate(void *appstate)
	-> SDL_AppResult
{
	return stellarlib::app::main::iterate(appstate);
}

auto SDL_AppEvent(void *appstate, SDL_Event *event)
	-> SDL_AppResult
{
	return stellarlib::app::main::event(appstate, event);
}

void SDL_AppQuit(void *appstate, const SDL_AppResult result)
{
	stellarlib::app::main::quit(appstate, result);
}
}
