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

#include <stellarlib/gfx/stage.hpp>

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <utility>

namespace stellarlib::gfx
{
stage::~stage() noexcept = default;

stage::operator const SDL_GPUDevice *() const
{
	return _device.get();
}

void stage::copy([[maybe_unused]] const copy_info &info) {}

void stage::render([[maybe_unused]] const render_info &info) {}

stage::stage(std::shared_ptr<SDL_GPUDevice> device)
	: _device{std::move(device)}
{}

stage::stage(const stage &) noexcept = default;

stage::stage(stage &&) noexcept = default;

auto stage::operator=(const stage &) noexcept
	-> stage & = default;

auto stage::operator=(stage &&) noexcept
	-> stage & = default;
}
