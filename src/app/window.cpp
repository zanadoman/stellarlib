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

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <cstring>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

namespace stellarlib::app
{
window::~window()
{
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
	if (!SDL_SetGPUSwapchainParameters(_device.get(), _handle, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, vsync ? SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_MAILBOX) ? SDL_GPU_PRESENTMODE_MAILBOX : SDL_GPU_PRESENTMODE_VSYNC : SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_IMMEDIATE) ? SDL_GPU_PRESENTMODE_IMMEDIATE : SDL_GPU_PRESENTMODE_MAILBOX)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_vsync = vsync;
}

window::window(const info &info)
	: _handle{SDL_CreateWindow(info.title.c_str(), 0, 0, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE)}
{
	if (!static_cast<bool>(_handle) || !SDL_SetWindowIcon(_handle, static_cast<SDL_Surface *>(info.icon)) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
		throw std::runtime_error{SDL_GetError()};
	}

	const auto props{SDL_CreateProperties()};

	if (!lin::cast<bool>(props)) {
		throw std::runtime_error{SDL_GetError()};
	}

	const std::unique_ptr<const SDL_PropertiesID, void (*)(const SDL_PropertiesID *)> props_holder{std::addressof(props), [] (const auto props) -> void {
		SDL_DestroyProperties(*props);
	}};

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
}

void window::event(const SDL_Event &event) const
{
	if (event.type == SDL_EVENT_WINDOW_DESTROYED && event.window.windowID == SDL_GetWindowID(_handle)) {
		SDL_InvalidParamError("event");
		throw std::invalid_argument{SDL_GetError()};
	}
}
}
