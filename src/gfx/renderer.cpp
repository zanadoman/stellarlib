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

#include <stellarlib/gfx/renderer.hpp>

#include <stellarlib/lin/lin.hpp>
#include <stellarlib/res/res.hpp>

#include <optional>

namespace stellarlib::gfx
{
renderer::~renderer() noexcept = default;

void renderer::set_min_aspect([[maybe_unused]] const std::optional<float> min_aspect) {}

void renderer::set_max_aspect([[maybe_unused]] const std::optional<float> max_aspect) {}

void renderer::set_max_resolution([[maybe_unused]] const std::optional<lin::uint2> &max_resolution) {}

void renderer::set_filter([[maybe_unused]] const res::image::filter filter) {}

void renderer::set_presentation([[maybe_unused]] const enum presentation presentation) {}

void renderer::set_vsync([[maybe_unused]] const bool vsync) {}

void renderer::blit_texture([[maybe_unused]] const blit_info &info, [[maybe_unused]] const bool idle) {}

renderer::renderer() noexcept = default;

renderer::renderer(const renderer &) noexcept = default;

renderer::renderer(renderer &&) noexcept = default;

auto renderer::operator=(const renderer &) noexcept
	-> renderer & = default;

auto renderer::operator=(renderer &&) noexcept
	-> renderer & = default;
}
