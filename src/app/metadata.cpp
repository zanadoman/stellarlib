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

#include <stellarlib/app/metadata.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>

#include <stdexcept>
#include <string>

namespace stellarlib::app
{
metadata::~metadata() = default;

auto metadata::name() const
	-> std::string
{
	const auto name{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING)};
	return static_cast<bool>(name) ? name : "";
}

auto metadata::version() const
	-> std::string
{
	const auto version{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING)};
	return static_cast<bool>(version) ? version : "";
}

auto metadata::identifier() const
	-> std::string
{
	const auto identifier{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING)};
	return static_cast<bool>(identifier) ? identifier : "";
}

auto metadata::creator() const
	-> std::string
{
	const auto creator{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING)};
	return static_cast<bool>(creator) ? creator : "";
}

auto metadata::copyright() const
	-> std::string
{
	const auto copyright{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING)};
	return static_cast<bool>(copyright) ? copyright : "";
}

auto metadata::url() const
	-> std::string
{
	const auto url{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING)};
	return static_cast<bool>(url) ? url : "";
}

auto metadata::type() const
	-> std::string
{
	const auto type{SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING)};
	return static_cast<bool>(type) ? type : "";
}

metadata::metadata(const info &info)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, info.name.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, info.version.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, info.identifier.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, info.creator.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, info.copyright.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, info.url.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, info.type.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}
}
}
