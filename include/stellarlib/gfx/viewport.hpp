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

#ifndef STELLARLIB_GFX_VIEWPORT_HPP
#define STELLARLIB_GFX_VIEWPORT_HPP

#include <stellarlib/ecs/ecs.hpp>
#include <stellarlib/gfx/renderer.hpp>
#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_gpu.h>

#include <cstdint>

namespace stellarlib::gfx
{
class [[nodiscard]] viewport final
{
public:
	struct [[nodiscard]] render_info final
	{
		renderer *renderer;

		SDL_GPUCommandBuffer *cmdbuf;

		SDL_GPUTexture *swapchain;

		lin::uint2 size;

		ecs::world *world;

		std::uint32_t entity;

		[[maybe_unused]] std::array<std::byte, 4> padding;
	};

	void render(const render_info &info);

private:

};
}

#endif
