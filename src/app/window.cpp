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

#include <stellarlib/app/window.hpp>

#include <stellarlib/gfx/gfx.hpp>
#include <stellarlib/lin/lin.hpp>
#include <stellarlib/res/res.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace stellarlib::app
{
window::~window()
{
	SDL_ReleaseGPUFence(_device.get(), _fence);
	SDL_ReleaseGPUTransferBuffer(_device.get(), _transbuf);
	SDL_ReleaseWindowFromGPUDevice(_device.get(), _handle);
	SDL_DestroyWindow(_handle);
}

auto window::title() const
	-> std::string
{
	return SDL_GetWindowTitle(_handle);
}

void window::set_title(const std::string &title)
{
	if (!SDL_SetWindowTitle(_handle, title.c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}
}

auto window::vsync() const
	-> bool
{
	return _vsync;
}

void window::set_vsync(const bool vsync)
{
	if (!SDL_WindowSupportsGPUSwapchainComposition(_device.get(), _handle, SDL_GPU_SWAPCHAINCOMPOSITION_SDR)) {
		return;
	}

	std::int8_t mode{-1};

	if (vsync) {
		if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_MAILBOX)) {
			mode = SDL_GPU_PRESENTMODE_MAILBOX;
		}
		else if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_VSYNC)) {
			mode = SDL_GPU_PRESENTMODE_VSYNC;
		}
	}
	else {
		if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_IMMEDIATE)) {
			mode = SDL_GPU_PRESENTMODE_IMMEDIATE;
		}
		else if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_MAILBOX)) {
			mode = SDL_GPU_PRESENTMODE_MAILBOX;
		}
	}

	if (mode == -1) {
		return;
	}

	if (!SDL_SetGPUSwapchainParameters(_device.get(), _handle, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, static_cast<SDL_GPUPresentMode>(mode))) {
		throw std::runtime_error{SDL_GetError()};
	}

	_vsync = vsync;
}

auto window::create_texture(const res::image &image, const bool mipmaps)
	-> gfx::texture
{
	SDL_GPUTextureCreateInfo info{
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		.width = image.size().x(),
		.height = image.size().y(),
		.layer_count_or_depth = 1,
		.num_levels = 1
	};

	if (mipmaps) {
		info.num_levels += lin::cast<std::uint32_t>(lin::log(lin::max(info.width, info.height)));
	}

	gfx::texture texture{_device, info};

	if (_transbuf_size < image.bytes().size()) {
		_transbuf_size = lin::cast<std::uint32_t>(image.bytes().size());

		const SDL_GPUTransferBufferCreateInfo transbuf{
			.size = _transbuf_size
		};

		SDL_ReleaseGPUTransferBuffer(_device.get(), std::exchange(_transbuf, SDL_CreateGPUTransferBuffer(_device.get(), std::addressof(transbuf))));

		if (!static_cast<bool>(_transbuf)) {
			throw std::runtime_error{SDL_GetError()};
		}
	}

	const std::unique_ptr<void, std::function<void (void *)>> transmem{SDL_MapGPUTransferBuffer(_device.get(), _transbuf, true), [this] (const auto) -> void {
		SDL_UnmapGPUTransferBuffer(_device.get(), _transbuf);
	}};

	if (!transmem) {
		throw std::runtime_error{SDL_GetError()};
	}

	std::memcpy(transmem.get(), image.bytes().data(), image.bytes().size());

	std::unique_ptr<SDL_GPUCommandBuffer, void (*)(SDL_GPUCommandBuffer *)> cmdbuf{SDL_AcquireGPUCommandBuffer(_device.get()), [] (const auto cmdbuf) -> void {
		if (!SDL_CancelGPUCommandBuffer(cmdbuf)) {
			throw std::runtime_error{SDL_GetError()};
		}
	}};

	const auto cpypass{SDL_BeginGPUCopyPass(cmdbuf.get())};

	const SDL_GPUTextureTransferInfo transfer{
		.transfer_buffer = _transbuf
	};

	const SDL_GPUTextureRegion region{
		.texture = static_cast<SDL_GPUTexture *>(texture),
		.w = info.width,
		.h = info.height,
		.d = 1
	};

	SDL_UploadToGPUTexture(cpypass, std::addressof(transfer), std::addressof(region), false);
	SDL_EndGPUCopyPass(cpypass);

	if (1 < info.num_levels) {
		SDL_GenerateMipmapsForGPUTexture(cmdbuf.get(), region.texture);
	}

	if (static_cast<bool>(_fence)) {
		if (!SDL_WaitForGPUFences(_device.get(), true, std::addressof(_fence), 1)) {
			throw std::runtime_error{SDL_GetError()};
		}

		SDL_ReleaseGPUFence(_device.get(), std::exchange(_fence, {}));
	}

	_fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf.release());

	if (!static_cast<bool>(_fence)) {
		throw std::runtime_error{SDL_GetError()};
	}

	return texture;
}

window::window(const info &info)
	: _handle{SDL_CreateWindow(info.title.c_str(), 0, 0, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE)}
{
	if (!static_cast<bool>(_handle) || !SDL_SetWindowIcon(_handle, static_cast<SDL_Surface *>(info.icon)) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
		throw std::runtime_error{SDL_GetError()};
	}

	SDL_Rect bounds{};

	if (const auto display{SDL_GetDisplayForWindow(_handle)}; !static_cast<bool>(display) || !SDL_GetDisplayUsableBounds(display, std::addressof(bounds)) || !SDL_SetWindowSize(_handle, bounds.w / 2, bounds.h / 2) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
		throw std::runtime_error{SDL_GetError()};
	}

	const auto props{SDL_CreateProperties()};

	const std::unique_ptr<const SDL_PropertiesID, void (*)(const SDL_PropertiesID *)> props_holder{std::addressof(props), [] (const auto props) -> void {
		SDL_DestroyProperties(*props);
	}};

	if (!props_holder) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, info.debug) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN, true)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_device = {SDL_CreateGPUDeviceWithProperties(props), SDL_DestroyGPUDevice};

	if (!_device || !SDL_ClaimWindowForGPUDevice(_device.get(), _handle)) {
		throw std::runtime_error{SDL_GetError()};
	}

	set_vsync(info.vsync);
}

void window::iterate()
{
	SDL_GPUColorTargetInfo swapchain{
		.load_op = SDL_GPU_LOADOP_CLEAR
	};

	const std::unique_ptr<SDL_GPUCommandBuffer, std::function<void (SDL_GPUCommandBuffer *)>> cmdbuf{SDL_AcquireGPUCommandBuffer(_device.get()), [&] (const auto cmdbuf) -> void {
		if (swapchain.texture ? !SDL_SubmitGPUCommandBuffer(cmdbuf) : !SDL_CancelGPUCommandBuffer(cmdbuf)) {
			throw std::runtime_error{SDL_GetError()};
		}
	}};

	if (!SDL_AcquireGPUSwapchainTexture(cmdbuf.get(), _handle, std::addressof(swapchain.texture), nullptr, nullptr)) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (static_cast<bool>(swapchain.texture)) {
		SDL_EndGPURenderPass(SDL_BeginGPURenderPass(cmdbuf.get(), std::addressof(swapchain), 1, nullptr));
	}

	if (static_cast<bool>(_fence)) {
		if (!SDL_WaitForGPUFences(_device.get(), true, std::addressof(_fence), 1)) {
			throw std::runtime_error{SDL_GetError()};
		}

		SDL_ReleaseGPUFence(_device.get(), std::exchange(_fence, {}));
	}
}

void window::event(const SDL_Event &event) const
{
	if (event.type == SDL_EVENT_WINDOW_DESTROYED && event.window.windowID == SDL_GetWindowID(_handle)) {
		SDL_InvalidParamError("event");
		throw std::invalid_argument{SDL_GetError()};
	}
}
}
