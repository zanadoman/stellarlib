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
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
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
	SDL_ReleaseGPUTexture(_device.get(), _transtex);
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

auto window::focused() const
	-> bool
{
	return _focused;
}

auto window::renderer() const
	-> const gfx::renderer<window> &
{
	return *this;
}

auto window::renderer()
	-> gfx::renderer<window> &
{
	return *this;
}

void window::blit(SDL_GPUCommandBuffer &cmdbuf, SDL_GPUTexture &src, const lin::uint2 size, SDL_GPUTexture &dst)
{
	const SDL_GPUBlitInfo descriptor{
		.source = {
			.texture = std::addressof(src),
			.w = size.x(),
			.h = size.y()
		},
		.destination = {
			.texture = std::addressof(dst),
			.w = size.x(),
			.h = size.y()
		}
	};

	SDL_BlitGPUTexture(std::addressof(cmdbuf), std::addressof(descriptor));
}

window::window(const info &info)
{
	if (!SDL_SetHintWithPriority(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight", SDL_HINT_OVERRIDE)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_handle = SDL_CreateWindow(info.title.c_str(), 0, 0, info.renderer.validation ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);

	if (!static_cast<bool>(_handle) || !SDL_SetWindowIcon(_handle, std::addressof(static_cast<SDL_Surface &>(info.icon))) && !static_cast<bool>(std::strstr(SDL_GetError(), "not supported"))) {
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

	if (!props_holder || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, info.renderer.validation) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN, false) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true) || !SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN, true)) {
		throw std::runtime_error{SDL_GetError()};
	}

	_device = {SDL_CreateGPUDeviceWithProperties(props), SDL_DestroyGPUDevice};

	if (!_device || !SDL_ClaimWindowForGPUDevice(_device.get(), _handle)) {
		throw std::runtime_error{SDL_GetError()};
	}

	set_min_aspect(info.renderer.min_aspect);
	set_max_aspect(info.renderer.max_aspect);
	set_max_resolution(info.renderer.max_resolution);
	set_filter(info.renderer.filter);
	set_presentation(info.renderer.presentation);
	set_vsync(info.renderer.vsync);
	submit_cmdbuf(acquire_cmdbuf());
}

window::operator SDL_GPUDevice &() const
{
	return *_device;
}

window::operator std::shared_ptr<SDL_GPUDevice>() const
{
	return _device;
}

auto window::min_aspect() const
	-> std::optional<float>
{
	return _min_aspect;
}

void window::set_min_aspect(const std::optional<float> min_aspect)
{
	if (_max_aspect && _max_aspect < min_aspect) {
		SDL_InvalidParamError("min_aspect");
		throw std::invalid_argument{SDL_GetError()};
	}

	_min_aspect = min_aspect;
}

auto window::max_aspect() const
	-> std::optional<float>
{
	return _max_aspect;
}

void window::set_max_aspect(const std::optional<float> max_aspect)
{
	if (max_aspect && max_aspect < _min_aspect) {
		SDL_InvalidParamError("max_aspect");
		throw std::invalid_argument{SDL_GetError()};
	}

	_max_aspect = max_aspect;
}

auto window::max_resolution() const
	-> const std::optional<lin::uint2> &
{
	return _max_resolution;
}

void window::set_max_resolution(const std::optional<lin::uint2> &max_resolution)
{
	if (max_resolution && !lin::all(*max_resolution)) {
		SDL_InvalidParamError("max_resolution");
		throw std::invalid_argument{SDL_GetError()};
	}

	_max_resolution = max_resolution;
}

auto window::filter() const
	-> res::image::filter
{
	return _filter;
}

void window::set_filter(const res::image::filter filter)
{
	switch (filter) {
	case res::image::filter::nearest:
	case res::image::filter::linear: {
		_filter = filter;
		break;
	}
	default: {
		SDL_InvalidParamError("filter");
		throw std::invalid_argument{SDL_GetError()};
	}}
}

auto window::presentation() const
	-> enum presentation
{
	return _presentation;
}

void window::set_presentation(const enum presentation presentation)
{
	switch (presentation) {
	case presentation::letterbox:
	case presentation::stretch: {
		_presentation = presentation;
		break;
	}
	default: {
		SDL_InvalidParamError("presentation");
		throw std::invalid_argument{SDL_GetError()};
	}}
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
	wait_fence();
	std::memcpy(map_transbuf().get(), image.bytes().data(), image.bytes().size());
	auto cmdbuf{acquire_cmdbuf()};
	const auto cpypass{SDL_BeginGPUCopyPass(cmdbuf.get())};
	extend_transtex(image.size());
	const auto [transfer, region]{prepare_transfer(*_transtex, image.size())};
	SDL_UploadToGPUTexture(cpypass, std::addressof(transfer), std::addressof(region), false);
	SDL_EndGPUCopyPass(cpypass);
	gfx::texture texture{_device, image.size(), mipmaps};
	blit(*cmdbuf, *_transtex, texture.size(), static_cast<SDL_GPUTexture &>(texture));

	if (texture.mipmaps()) {
		SDL_GenerateMipmapsForGPUTexture(cmdbuf.get(), std::addressof(static_cast<SDL_GPUTexture &>(texture)));
	}

	submit_cmdbuf(std::move(cmdbuf));
	return texture;
}

void window::blit_texture(const blit_info &info, const bool idle)
{
	if (std::addressof(static_cast<const SDL_GPUDevice &>(info.src_texture)) != _device.get() || lin::any(info.src_area.p < 0.0F) || lin::any(info.src_area.s < 0.0F) || std::addressof(static_cast<const SDL_GPUDevice &>(info.dst_texture)) != _device.get() || lin::any(info.dst_area.p < 0.0F) || lin::any(info.dst_area.s < 0.0F)) {
		SDL_InvalidParamError("info");
		throw std::invalid_argument{SDL_GetError()};
	}

	SDL_GPUBlitInfo descriptor{
		.source = {
			.texture = std::addressof(static_cast<SDL_GPUTexture &>(info.src_texture)),
			.x = lin::cast<std::uint32_t>(lin::cast<float>(info.src_texture.size().x()) * info.src_area.p.x()),
			.y = lin::cast<std::uint32_t>(lin::cast<float>(info.src_texture.size().y()) * info.src_area.p.y()),
			.w = lin::cast<std::uint32_t>(lin::cast<float>(info.src_texture.size().x()) * info.src_area.s.x()),
			.h = lin::cast<std::uint32_t>(lin::cast<float>(info.src_texture.size().y()) * info.src_area.s.y())
		},
		.destination = {
			.texture = std::addressof(static_cast<SDL_GPUTexture &>(info.dst_texture)),
			.x = lin::cast<std::uint32_t>(lin::cast<float>(info.dst_texture.size().x()) * info.dst_area.p.x()),
			.y = lin::cast<std::uint32_t>(lin::cast<float>(info.dst_texture.size().y()) * info.dst_area.p.y()),
			.w = lin::cast<std::uint32_t>(lin::cast<float>(info.dst_texture.size().x()) * info.dst_area.s.x()),
			.h = lin::cast<std::uint32_t>(lin::cast<float>(info.dst_texture.size().y()) * info.dst_area.s.y())
		}
	};

	if (info.src_texture.size().x() < descriptor.source.x + descriptor.source.w || info.src_texture.size().y() < descriptor.source.y + descriptor.source.h || info.dst_texture.size().x() < descriptor.destination.x + descriptor.destination.w || info.dst_texture.size().y() < descriptor.destination.y + descriptor.destination.h) {
		SDL_InvalidParamError("info");
		throw std::invalid_argument{SDL_GetError()};
	}

	if (!lin::cast<bool>(descriptor.source.w) || !lin::cast<bool>(descriptor.source.h) || !lin::cast<bool>(descriptor.destination.w) || !lin::cast<bool>(descriptor.destination.h)) {
		return;
	}

	switch (info.filter) {
	case res::image::filter::nearest: {
		descriptor.filter = SDL_GPU_FILTER_NEAREST;
		break;
	}
	case res::image::filter::linear: {
		descriptor.filter = SDL_GPU_FILTER_LINEAR;
		break;
	}
	default: {
		SDL_InvalidParamError("info");
		throw std::invalid_argument{SDL_GetError()};
	}}

	auto cmdbuf{acquire_cmdbuf()};
	SDL_BlitGPUTexture(cmdbuf.get(), std::addressof(descriptor));
	wait_fence();

	if (idle) {
		wait_fences();
	}

	submit_cmdbuf(std::move(cmdbuf));
}

auto window::download_texture(const gfx::texture &texture, const bool idle)
	-> res::image
{
	if (std::addressof(static_cast<const SDL_GPUDevice &>(texture)) != _device.get()) {
		SDL_InvalidParamError("texture");
		throw std::invalid_argument{SDL_GetError()};
	}

	auto cmdbuf(acquire_cmdbuf());
	extend_transtex(texture.size());
	blit(*cmdbuf, static_cast<SDL_GPUTexture &>(texture), texture.size(), *_transtex);
	const auto cpypass{SDL_BeginGPUCopyPass(cmdbuf.get())};
	res::image image{texture.size()};
	extend_transbuf(lin::cast<std::uint32_t>(image.bytes().size()));
	const auto [transfer, region]{prepare_transfer(*_transtex, texture.size())};
	SDL_DownloadFromGPUTexture(cpypass, std::addressof(region), std::addressof(transfer));
	SDL_EndGPUCopyPass(cpypass);
	wait_fence();

	if (idle) {
		wait_fences();
	}

	submit_cmdbuf(std::move(cmdbuf));
	wait_fence();
	std::memcpy(image.bytes().data(), map_transbuf().get(), image.bytes().size());
	return image;
}

auto window::framebuffer() const
	-> const std::optional<gfx::texture> &
{
	return _framebuffer;
}

auto window::framebuffer()
	-> std::optional<gfx::texture> &
{
	return _framebuffer;
}

void window::iterate()
{
	SDL_GPUColorTargetInfo target{
		.load_op = SDL_GPU_LOADOP_CLEAR
	};

	const std::unique_ptr<SDL_GPUCommandBuffer, std::function<void (SDL_GPUCommandBuffer *)>> cmdbuf{SDL_AcquireGPUCommandBuffer(_device.get()), [&] (const auto cmdbuf) -> void {
		if (target.texture) {
			wait_fence();

			const auto fences{std::ranges::remove_if(_fences, [cpt = _device.get()] [[nodiscard]] (const auto fence) -> auto {
				return SDL_QueryGPUFence(cpt, fence);
			})};

			for (auto &fence : _fences) {
				SDL_ReleaseGPUFence(_device.get(), fence);
				fence = nullptr;
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

	if (!cmdbuf || !SDL_AcquireGPUSwapchainTexture(cmdbuf.get(), _handle, std::addressof(target.texture), nullptr, nullptr)) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (!static_cast<bool>(target.texture)) {
		_framebuffer.reset();
		return;
	}

	SDL_EndGPURenderPass(SDL_BeginGPURenderPass(cmdbuf.get(), std::addressof(target), 1, nullptr));
	auto size{_safe_area.s};

	if (_min_aspect || _max_aspect || _max_resolution) {
		if (_max_resolution) {
			size = lin::min(size, *_max_resolution);
		}

		auto src_aspect{lin::cast<float>(size.x()) / lin::cast<float>(size.y())};
		const auto dst_aspect{src_aspect};

		if (src_aspect < _min_aspect) {
			src_aspect = *_min_aspect;
		}
		else if (_max_aspect && _max_aspect < src_aspect) {
			src_aspect = *_max_aspect;
		}

		if (src_aspect < dst_aspect) {
			size.x() = lin::cast<std::uint32_t>(lin::cast<float>(size.y()) * src_aspect);
		}
		else if (dst_aspect < src_aspect) {
			size.y() = lin::cast<std::uint32_t>(lin::cast<float>(size.x()) / src_aspect);
		}
	}

	if (!lin::all(size)) {
		_framebuffer.reset();
		return;
	}

	if (!_framebuffer) {
		_framebuffer = {_device, size, false};
	}

	SDL_GPUBlitInfo descriptor{
		.source = {
			.texture = std::addressof(static_cast<SDL_GPUTexture &>(*_framebuffer)),
			.w = _framebuffer->size().x(),
			.h = _framebuffer->size().y()
		},
		.destination = {
			.texture = target.texture,
			.x = _safe_area.p.x(),
			.y = _safe_area.p.y(),
			.w = _safe_area.s.x(),
			.h = _safe_area.s.y()
		}
	};

	if (_presentation == presentation::letterbox) {
		const auto src_aspect{lin::cast<float>(_framebuffer->size().x()) / lin::cast<float>(_framebuffer->size().y())};
		const auto dst_aspect{lin::cast<float>(_safe_area.s.x()) / lin::cast<float>(_safe_area.s.y())};

		if (src_aspect < dst_aspect) {
			const auto width{lin::cast<std::uint32_t>(lin::cast<float>(_framebuffer->size().x()) * lin::cast<float>(descriptor.destination.h) / lin::cast<float>(_framebuffer->size().y()))};
			descriptor.destination.x += (descriptor.destination.w - width) / 2;
			descriptor.destination.w = width;
		}
		else if (dst_aspect < src_aspect) {
			const auto height{lin::cast<std::uint32_t>(lin::cast<float>(_framebuffer->size().y()) * lin::cast<float>(descriptor.destination.w) / lin::cast<float>(_framebuffer->size().x()))};
			descriptor.destination.y += (descriptor.destination.h - height) / 2;
			descriptor.destination.h = height;
		}
	}

	if (!lin::cast<bool>(descriptor.destination.w) || !lin::cast<bool>(descriptor.destination.h)) {
		_framebuffer.reset();
		return;
	}

	switch (_filter) {
	case res::image::filter::nearest: {
		descriptor.filter = SDL_GPU_FILTER_NEAREST;
		break;
	}
	case res::image::filter::linear: {
		descriptor.filter = SDL_GPU_FILTER_LINEAR;
		break;
	}}

	SDL_BlitGPUTexture(cmdbuf.get(), std::addressof(descriptor));

	if (!lin::all(_framebuffer->size() == size)) {
		_framebuffer = {_device, size, false};
	}

	target.texture = std::addressof(static_cast<SDL_GPUTexture &>(*_framebuffer));
	SDL_EndGPURenderPass(SDL_BeginGPURenderPass(cmdbuf.get(), std::addressof(target), 1, nullptr));
}

void window::event(const SDL_Event &event)
{
	if (event.window.windowID != _handle_id) {
		return;
	}

	switch (event.type) {
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED: {
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

		break;
	}
	case SDL_EVENT_WINDOW_FOCUS_GAINED: {
		_focused = true;
		break;
	}
	case SDL_EVENT_WINDOW_FOCUS_LOST: {
		_focused = false;
		break;
	}}
}

void window::extend_transbuf(const std::uint32_t size)
{
	if (size <= _transbuf_size) {
		return;
	}

	const SDL_GPUTransferBufferCreateInfo descriptor{
		.usage = static_cast<SDL_GPUTransferBufferUsage>(SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD | SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD),
		.size = size
	};

	std::unique_ptr<SDL_GPUTransferBuffer, std::function<void (SDL_GPUTransferBuffer *)>> transbuf{SDL_CreateGPUTransferBuffer(_device.get(), std::addressof(descriptor)), [cpt = _device.get()] (const auto transbuf) -> void {
		SDL_ReleaseGPUTransferBuffer(cpt, transbuf);
	}};

	if (!transbuf) {
		throw std::runtime_error{SDL_GetError()};
	}

	SDL_ReleaseGPUTransferBuffer(_device.get(), _transbuf);
	_transbuf = transbuf.release();
	_transbuf_size = descriptor.size;
}

auto window::map_transbuf() const
	-> std::unique_ptr<void, std::function<void (void *)>>
{
	std::unique_ptr<void, std::function<void (const void *)>> transmem{SDL_MapGPUTransferBuffer(_device.get(), _transbuf, false), [this] (const auto) -> void {
		SDL_UnmapGPUTransferBuffer(_device.get(), _transbuf);
	}};

	if (!transmem) {
		throw std::runtime_error{SDL_GetError()};
	}

	return transmem;
}

auto window::acquire_cmdbuf() const
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

void window::extend_transtex(const lin::uint2 size)
{
	if (lin::all(size <= _transtex_size)) {
		return;
	}

	const SDL_GPUTextureCreateInfo descriptor{
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		.width = size.x(),
		.height = size.y(),
		.layer_count_or_depth = 1,
		.num_levels = 1
	};

	std::unique_ptr<SDL_GPUTexture, std::function<void (SDL_GPUTexture *)>> transtex{SDL_CreateGPUTexture(_device.get(), std::addressof(descriptor)), [cpt = _device.get()] (const auto transtex) -> void {
		SDL_ReleaseGPUTexture(cpt, transtex);
	}};

	if (!transtex) {
		throw std::runtime_error{SDL_GetError()};
	}

	SDL_ReleaseGPUTexture(_device.get(), _transtex);
	_transtex = transtex.release();
	_transtex_size = size;
}

auto window::prepare_transfer(SDL_GPUTexture &texture, const lin::uint2 size) const
	-> std::pair<SDL_GPUTextureTransferInfo, SDL_GPUTextureRegion>
{
	return {
		{
			.transfer_buffer = _transbuf
		},
		{
			.texture = std::addressof(texture),
			.w = size.x(),
			.h = size.y(),
			.d = 1
		}
	};
}

void window::wait_fence()
{
	if (!SDL_WaitForGPUFences(_device.get(), false, std::addressof(_fence), 1)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

void window::wait_fences()
{
	if (!SDL_WaitForGPUFences(_device.get(), true, _fences.data(), lin::cast<std::uint32_t>(_fences.size()))) {
		throw std::runtime_error{SDL_GetError()};
	}

	for (auto &fence : _fences) {
		SDL_ReleaseGPUFence(_device.get(), fence);
		fence = nullptr;
	}

	_fences.clear();
}

void window::submit_cmdbuf(std::unique_ptr<SDL_GPUCommandBuffer, void (*)(SDL_GPUCommandBuffer *)> cmdbuf)
{
	SDL_ReleaseGPUFence(_device.get(), _fence);
	_fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf.release());

	if (!static_cast<bool>(_fence)) {
		throw std::runtime_error{SDL_GetError()};
	}
}
}
