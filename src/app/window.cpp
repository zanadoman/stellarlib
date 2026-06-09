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
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <algorithm>
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
	for (const auto fence : _fences) {
		SDL_ReleaseGPUFence(_device.get(), fence);
	}

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

auto window::resolution() const
	-> lin::uint2
{
	return _safe_area.s;
}

auto window::renderer() const
	-> const gfx::renderer &
{
	return *this;
}

auto window::renderer()
	-> gfx::renderer &
{
	return *this;
}

void window::blit(SDL_GPUCommandBuffer *cmdbuf, SDL_GPUTexture *src, const lin::uint2 size, SDL_GPUTexture *dst)
{
	const SDL_GPUBlitInfo info{
		.source = {
			.texture = src,
			.w = size.x(),
			.h = size.y()
		},
		.destination = {
			.texture = dst,
			.w = size.x(),
			.h = size.y()
		}
	};

	SDL_BlitGPUTexture(cmdbuf, std::addressof(info));
}

window::window(const info &info)
	: _handle{SDL_CreateWindow(info.title.c_str(), 0, 0, info.renderer.debug ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE)}
{
	if (!static_cast<bool>(_handle) || !SDL_SetWindowIcon(_handle, static_cast<SDL_Surface *>(info.icon)) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
		throw std::runtime_error{SDL_GetError()};
	}

	_handle_id = SDL_GetWindowID(_handle);

	if (!lin::cast<bool>(_handle_id)) {
		throw std::runtime_error{SDL_GetError()};
	}

	SDL_Rect bounds{};

	if (const auto display{SDL_GetDisplayForWindow(_handle)}; !lin::cast<bool>(display) || !SDL_GetDisplayUsableBounds(display, std::addressof(bounds)) || !SDL_SetWindowSize(_handle, bounds.w / 2, bounds.h / 2) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
		throw std::runtime_error{SDL_GetError()};
	}

	const auto props{SDL_CreateProperties()};

	const std::unique_ptr<const SDL_PropertiesID, void (*)(const SDL_PropertiesID *)> props_holder{std::addressof(props), [] (const auto props) -> void {
		SDL_DestroyProperties(*props);
	}};

	if (!props_holder || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, info.renderer.debug) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN, true)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_device = {SDL_CreateGPUDeviceWithProperties(props), SDL_DestroyGPUDevice};

	if (!_device || !SDL_ClaimWindowForGPUDevice(_device.get(), _handle)) {
		throw std::runtime_error{SDL_GetError()};
	}

	set_vsync(info.renderer.vsync);
}

window::operator SDL_GPUDevice *() const
{
	return _device.get();
}

window::operator std::shared_ptr<SDL_GPUDevice>() const
{
	return _device;
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
	else if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_IMMEDIATE)) {
		mode = SDL_GPU_PRESENTMODE_IMMEDIATE;
	}
	else if (SDL_WindowSupportsGPUPresentMode(_device.get(), _handle, SDL_GPU_PRESENTMODE_MAILBOX)) {
		mode = SDL_GPU_PRESENTMODE_MAILBOX;
	}

	if (mode == -1) {
		return;
	}

	if (!SDL_SetGPUSwapchainParameters(_device.get(), _handle, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, static_cast<SDL_GPUPresentMode>(mode))) {
		throw std::runtime_error{SDL_GetError()};
	}

	_vsync = vsync;
}

auto window::upload_image(const res::image &image, const bool mipmaps)
	-> gfx::texture
{
	extend_transbuf(lin::cast<std::uint32_t>(image.bytes().size()));
	std::memcpy(map_transbuf().get(), image.bytes().data(), image.bytes().size());
	auto cmdbuf{acquire_cmdbuf()};
	const auto cpypass{SDL_BeginGPUCopyPass(cmdbuf.get())};
	const auto transtex{create_transtex(SDL_GPU_TEXTUREUSAGE_SAMPLER, image.size())};
	const auto [transfer, region]{prepare_transfer(transtex.get(), image.size())};
	SDL_UploadToGPUTexture(cpypass, std::addressof(transfer), std::addressof(region), false);
	SDL_EndGPUCopyPass(cpypass);
	gfx::texture texture{_device, image.size(), mipmaps};
	blit(cmdbuf.get(), transtex.get(), texture.size(), static_cast<SDL_GPUTexture *>(texture));

	if (texture.mipmaps()) {
		SDL_GenerateMipmapsForGPUTexture(cmdbuf.get(), static_cast<SDL_GPUTexture *>(texture));
	}

	submit_cmdbuf(std::move(cmdbuf));
	return texture;
}

auto window::download_texture(const gfx::texture &texture, const bool idle)
	-> res::image
{
	if (static_cast<const SDL_GPUDevice *>(texture) != _device.get()) {
		SDL_InvalidParamError("texture");
		throw std::invalid_argument{SDL_GetError()};
	}

	auto cmdbuf(acquire_cmdbuf());
	const auto transtex{create_transtex(SDL_GPU_TEXTUREUSAGE_COLOR_TARGET, texture.size())};
	blit(cmdbuf.get(), static_cast<SDL_GPUTexture *>(texture), texture.size(), transtex.get());
	const auto cpypass{SDL_BeginGPUCopyPass(cmdbuf.get())};
	res::image image{texture.size()};
	extend_transbuf(lin::cast<std::uint32_t>(image.bytes().size()));
	const auto [transfer, region]{prepare_transfer(transtex.get(), texture.size())};
	SDL_DownloadFromGPUTexture(cpypass, std::addressof(region), std::addressof(transfer));
	SDL_EndGPUCopyPass(cpypass);

	if (idle && lin::cast<bool>(_fences.size())) {
		if (!SDL_WaitForGPUFences(_device.get(), true, _fences.data(), lin::cast<std::uint32_t>(_fences.size()))) {
			throw std::runtime_error{SDL_GetError()};
		}

		for (auto &fence : _fences) {
			SDL_ReleaseGPUFence(_device.get(), std::exchange(fence, {}));
		}

		_fences.clear();
	}

	submit_cmdbuf(std::move(cmdbuf));
	wait_fence();
	std::memcpy(image.bytes().data(), map_transbuf().get(), image.bytes().size());
	return image;
}

void window::iterate()
{
	SDL_GPUColorTargetInfo swapchain{
		.load_op = SDL_GPU_LOADOP_CLEAR
	};

	const std::unique_ptr<SDL_GPUCommandBuffer, std::function<void (SDL_GPUCommandBuffer *)>> cmdbuf{SDL_AcquireGPUCommandBuffer(_device.get()), [&] (const auto cmdbuf) -> void {
		if (swapchain.texture) {
			const auto fences{std::ranges::remove_if(_fences, [cpt = _device.get()] [[nodiscard]] (const auto fence) -> auto {
				return SDL_QueryGPUFence(cpt, fence);
			})};

			for (auto &fence : _fences) {
				SDL_ReleaseGPUFence(_device.get(), std::exchange(fence, {}));
			}

			_fences.erase(fences.begin(), _fences.end());
			_fences.emplace_back(SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf));

			if (!_fences.back()) {
				_fences.pop_back();
				throw std::runtime_error{SDL_GetError()};
			}
		}
		else if (!SDL_CancelGPUCommandBuffer(cmdbuf)) {
			throw std::runtime_error{SDL_GetError()};
		}
	}};

	if (!cmdbuf || !SDL_AcquireGPUSwapchainTexture(cmdbuf.get(), _handle, std::addressof(swapchain.texture), nullptr, nullptr)) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (static_cast<bool>(swapchain.texture)) {
		SDL_EndGPURenderPass(SDL_BeginGPURenderPass(cmdbuf.get(), std::addressof(swapchain), 1, nullptr));
	}

	wait_fence();
}

void window::event(const SDL_Event &event)
{
	if (event.window.windowID != _handle_id || event.type != SDL_EVENT_WINDOW_SAFE_AREA_CHANGED && event.type != SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
		return;
	}

	SDL_Rect area{};
	lin::int2 size{};

	if (!SDL_GetWindowSafeArea(_handle, std::addressof(area)) || !SDL_GetWindowSizeInPixels(_handle, std::addressof(size.x()), std::addressof(size.y()))) {
		throw std::runtime_error{SDL_GetError()};
	}

	const auto inset{lin::max(area.x, size.x() - area.w - area.x)};

	_safe_area = {
		.p = lin::uint2{inset, area.y},
		.s = lin::uint2{size.x() - inset * 2, area.h}
	};
}

void window::extend_transbuf(const std::uint32_t size)
{
	if (size <= _transbuf_size) {
		return;
	}

	const SDL_GPUTransferBufferCreateInfo info{
		.size = size
	};

	const auto transbuf{SDL_CreateGPUTransferBuffer(_device.get(), std::addressof(info))};

	if (!static_cast<bool>(transbuf)) {
		throw std::runtime_error{SDL_GetError()};
	}

	SDL_ReleaseGPUTransferBuffer(_device.get(), std::exchange(_transbuf, transbuf));
	_transbuf_size = info.size;
}

auto window::map_transbuf()
	-> std::unique_ptr<void, std::function<void (void *)>>
{
	std::unique_ptr<void, std::function<void (void *)>> transmem{SDL_MapGPUTransferBuffer(_device.get(), _transbuf, true), [this] (const auto) -> void {
		SDL_UnmapGPUTransferBuffer(_device.get(), _transbuf);
	}};

	if (!transmem) {
		throw std::runtime_error{SDL_GetError()};
	}

	return transmem;
}

auto window::acquire_cmdbuf()
	-> std::unique_ptr<SDL_GPUCommandBuffer, void (*)(SDL_GPUCommandBuffer *)>
{
	std::unique_ptr<SDL_GPUCommandBuffer, void (*)(SDL_GPUCommandBuffer *)> cmdbuf{SDL_AcquireGPUCommandBuffer(_device.get()), [] (const auto cmdbuf) -> void {
		if (!SDL_CancelGPUCommandBuffer(cmdbuf)) {
			throw std::runtime_error{SDL_GetError()};
		}
	}};

	if (!cmdbuf) {
		throw std::runtime_error{SDL_GetError()};
	}

	return cmdbuf;
}

auto window::create_transtex(const SDL_GPUTextureUsageFlags usage, const lin::uint2 size)
	-> std::unique_ptr<SDL_GPUTexture, std::function<void (SDL_GPUTexture *)>>
{
	const SDL_GPUTextureCreateInfo info{
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = usage,
		.width = size.x(),
		.height = size.y(),
		.layer_count_or_depth = 1,
		.num_levels = 1
	};

	std::unique_ptr<SDL_GPUTexture, std::function<void (SDL_GPUTexture *)>> transtex{SDL_CreateGPUTexture(_device.get(), std::addressof(info)), [cpt = _device.get()] (const auto transtex) -> void {
		SDL_ReleaseGPUTexture(cpt, transtex);
	}};

	if (!transtex) {
		throw std::runtime_error{SDL_GetError()};
	}

	return transtex;
}

auto window::prepare_transfer(SDL_GPUTexture *texture, const lin::uint2 size)
	-> std::pair<SDL_GPUTextureTransferInfo, SDL_GPUTextureRegion>
{
	return {
		{
			.transfer_buffer = _transbuf
		},
		{
			.texture = texture,
			.w = size.x(),
			.h = size.y(),
			.d = 1
		}
	};
}

void window::wait_fence()
{
	if (static_cast<bool>(_fence) && !SDL_WaitForGPUFences(_device.get(), false, std::addressof(_fence), 1)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

void window::submit_cmdbuf(std::unique_ptr<SDL_GPUCommandBuffer, void (*)(SDL_GPUCommandBuffer *)> cmdbuf)
{
	wait_fence();
	SDL_ReleaseGPUFence(_device.get(), std::exchange(_fence, SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf.release())));

	if (!static_cast<bool>(_fence)) {
		throw std::runtime_error{SDL_GetError()};
	}
}
}
