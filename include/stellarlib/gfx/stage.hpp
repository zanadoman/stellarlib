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

#ifndef STELLARLIB_GFX_STAGE_HPP
#define STELLARLIB_GFX_STAGE_HPP

#include <stellarlib/ecs/ecs.hpp>
#include <stellarlib/gfx/renderer.hpp>
#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_gpu.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

/**
 * @brief Graphics and rendering abstractions
 */
namespace stellarlib::gfx
{
/**
 * @brief Pipeline stage interface
 * @warning Intended for internal/professional use
 */
class [[nodiscard]] stage
{
public:
	/**
	 * @brief Validation pass info
	 */
	struct [[nodiscard]] validate_info final
	{
		/**
		 * @brief Host world of the entity
		 */
		ecs::world *world;

		/**
		 * @brief Host entity of the camera
		 */
		std::uint32_t entity;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 4> padding;
	};

	/**
	 * @brief Copy pass info
	 */
	struct [[nodiscard]] copy_info final
	{
		/**
		 * @brief Renderer context
		 */
		renderer *renderer;

		/**
		 * @brief Command buffer handle
		 */
		SDL_GPUCommandBuffer *cmdbuf;

		/**
		 * @brief Copy pass handle
		 */
		SDL_GPUCopyPass *cpypass;

		/**
		 * @brief Host world of the entity
		 */
		ecs::world *world;

		/**
		 * @brief Host entity of the camera
		 */
		std::uint32_t entity;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 4> padding;
	};

	/**
	 * @brief Render pass info
	 */
	struct [[nodiscard]] render_info final
	{
		/**
		 * @brief Renderer context
		 */
		renderer *renderer;

		/**
		 * @brief Command buffer handle
		 */
		SDL_GPUCommandBuffer *cmdbuf;

		/**
		 * @brief Swapchain texture handle
		 */
		SDL_GPUTexture *swapchain;

		/**
		 * @brief Swapchain texture size
		 */
		lin::uint2 size;

		/**
		 * @brief Host world of the entity
		 */
		ecs::world *world;

		/**
		 * @brief Host entity of the camera
		 */
		std::uint32_t entity;

		/**
		 * @brief Explicit padding
		 */
		[[maybe_unused]] std::array<std::byte, 4> padding;
	};

	/**
	 * @brief Destructor
	 */
	virtual ~stage() noexcept;

	/**
	 * @brief Returns a pointer to the internal device
	 * @return Pointer to the internal device
	 * @warning Intended for internal/professional use
	 */
	[[nodiscard]]
	explicit operator const SDL_GPUDevice *() const;

	/**
	 * @brief Validation pass
	 * @param info Validation pass info
	 * @return Result of the validation
	 */
	virtual constexpr auto validate(const validate_info &info)
		-> bool = 0;

	/**
	 * @brief Copy pass
	 * @param info Copy pass info
	 */
	virtual void copy(const copy_info &info);

	/**
	 * @brief Render pass
	 * @param info Render pass info
	 */
	virtual void render(const render_info &info);

protected:
	/**
	 * @brief Parameterized constructor
	 * @param device Device of the texture
	 */
	[[nodiscard]]
	explicit stage(std::shared_ptr<SDL_GPUDevice> device);

	/**
	 * @brief Copy constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	stage(const stage &other) noexcept;

	/**
	 * @brief Move constructor
	 * @param other Other instance
	 */
	[[nodiscard]]
	stage(stage &&other) noexcept;

	/**
	 * @brief Copy assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(const stage &other) noexcept
		-> stage &;

	/**
	 * @brief Move assignment operator
	 * @param other Other instance
	 * @return Current instance
	 */
	auto operator=(stage &&other) noexcept
		-> stage &;

private:
	std::shared_ptr<SDL_GPUDevice> _device{};
};
}

#endif
