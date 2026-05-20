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

#include <stellarlib/res/image.hpp>

#include <stellarlib/lin/lin.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_surface.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <optional>
#include <span>
#include <stdexcept>
#include <utility>

namespace stellarlib::res
{
image::image(const lin::uint2 size)
{
	if (!lin::all(size) || lin::any(lin::cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) < size)) {
		SDL_InvalidParamError("size");
		throw std::invalid_argument{SDL_GetError()};
	}

	_handle = SDL_CreateSurface(lin::cast<std::int32_t>(size.x()), lin::cast<std::int32_t>(size.y()), format);

	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

image::image(const std::filesystem::path &path)
	: _handle{SDL_LoadPNG(path.string().c_str())}
{
	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}

	if (_handle->format == format) {
		return;
	}

	SDL_DestroySurface(std::exchange(_handle, SDL_ConvertSurface(_handle, format)));

	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

image::image(const image &other)
	: _handle{SDL_DuplicateSurface(other._handle)}
{
	if (!static_cast<bool>(_handle)) {
		throw std::runtime_error{SDL_GetError()};
	}
}

image::image(image &&other)
	: _handle{std::exchange(other._handle, {})}
{}

auto image::operator=(const image &other)
	-> image &
{
	if (std::addressof(other) == this) {
		return *this;
	}

	if (lin::all(other.size() == size())) {
		std::ranges::copy(other.bytes(), bytes().begin());
	}
	else {
		std::destroy_at(this);
		std::construct_at(this, other);
	}

	return *this;
}

auto image::operator=(image &&other)
	-> image &
{
	if (std::addressof(other) != this) {
		std::destroy_at(this);
		std::construct_at(this, std::move(other));
	}

	return *this;
}

image::~image()
{
	SDL_DestroySurface(_handle);
}

image::operator SDL_Surface *() const
{
	return _handle;
}

auto image::size() const
	-> lin::uint2
{
	return lin::uint2{_handle->w, _handle->h};
}

auto image::operator[](const lin::uint2 coord) const
	-> const lin::uchar4 &
{
	return *(static_cast<const lin::uchar4 *>(_handle->pixels) + lin::mad(coord.y(), lin::cast<std::uint32_t>(_handle->w), coord.x()));
}

auto image::operator[](const lin::uint2 coord)
	-> lin::uchar4 &
{
	return *(static_cast<lin::uchar4 *>(_handle->pixels) + lin::mad(coord.y(), lin::cast<std::uint32_t>(_handle->w), coord.x()));
}

auto image::bytes() const
	-> std::span<const std::uint8_t>
{
	return {static_cast<const std::uint8_t *>(_handle->pixels), lin::cast<std::size_t>(_handle->pitch * _handle->h)};
}

auto image::bytes()
	-> std::span<std::uint8_t>
{
	return {static_cast<std::uint8_t *>(_handle->pixels), lin::cast<std::size_t>(_handle->pitch * _handle->h)};
}

auto image::pixels() const
	-> std::span<const lin::uchar4>
{
	return {static_cast<const lin::uchar4 *>(_handle->pixels), lin::cast<std::size_t>(_handle->w * _handle->h)};
}

auto image::pixels()
	-> std::span<lin::uchar4>
{
	return {static_cast<lin::uchar4 *>(_handle->pixels), lin::cast<std::size_t>(_handle->w * _handle->h)};
}

auto image::sample(lin::float2 uv, const address_mode address_mode, const filter filter) const
	-> lin::float4
{
	switch (address_mode) {
	case address_mode::repeat: {
		uv = (uv - lin::floor(uv)) * lin::cast<float>(size());
		break;
	}
	case address_mode::mirrored_repeat: {
		uv *= 0.5F;
		uv = (1.0F - lin::abs((uv - lin::floor(uv)) * 2.0F - 1.0F)) * lin::cast<float>(size());
		break;
	}
	case address_mode::clamp_to_edge: {
		uv = lin::clamp(uv * lin::cast<float>(size()), 0.0F, lin::cast<float>(size() - std::uint32_t{1}));
		break;
	}
	default: {
		SDL_InvalidParamError("address_mode");
		throw std::invalid_argument{SDL_GetError()};
	}}

	switch (filter) {
	case filter::nearest: {
		return lin::float4{(*this)[lin::cast<std::uint32_t>(uv)]} / 255.0F;
	}
	case filter::linear: {
		uv -= 0.5F;
		const auto coord{lin::floor(uv)};
		const auto tl{lin::max(coord, 0.0F)};
		const auto br{lin::min(coord + 1.0F, lin::cast<float>(size() - std::uint32_t{1}))};
		const auto s{uv - coord};
		return lin::lerp(lin::lerp(lin::float4{(*this)[lin::cast<std::uint32_t>(tl)]}, lin::float4{(*this)[lin::cast<std::uint32_t>(lin::float2{br.x(), tl.y()})]}, s.x()), lin::lerp(lin::float4{(*this)[lin::cast<std::uint32_t>(lin::float2{tl.x(), br.y()})]}, lin::float4{(*this)[lin::cast<std::uint32_t>(br)]}, s.x()), s.y()) / 255.0F;
	}
	default: {
		SDL_InvalidParamError("filter");
		throw std::invalid_argument{SDL_GetError()};
	}}
}

void image::blend(const lin::uint2 coord, const lin::float4 &src, const std::optional<blend_state> &blend_state)
{
	if (!blend_state.has_value()) {
		(*this)[coord] = lin::cast<std::uint8_t>(lin::saturate(src) * 255.0F);
		return;
	}

	constexpr auto resolve_op{[] [[nodiscard]] (const auto blend_op, const auto &lhs, const auto &rhs) -> auto {
		switch (blend_op) {
		case blend_op::add: {
			return lhs + rhs;
		}
		case blend_op::subtract: {
			return lhs - rhs;
		}
		case blend_op::reverse_subtract: {
			return rhs - lhs;
		}
		case blend_op::min: {
			return lin::min(lhs, rhs);
		}
		case blend_op::max: {
			return lin::max(lhs, rhs);
		}
		default: {
			SDL_InvalidParamError("blend_op");
			throw std::invalid_argument{SDL_GetError()};
		}}
	}};

	constexpr auto resolve_factor{[] [[nodiscard]] (const auto blend_factor, const auto &src, const auto &dst) -> auto {
		switch (blend_factor) {
		case blend_factor::zero: {
			return lin::float4{0.0F, 0.0F, 0.0F, 0.0F};
		}
		case blend_factor::one: {
			return lin::float4{1.0F, 1.0F, 1.0F, 1.0F};
		}
		case blend_factor::src_color: {
			return src;
		}
		case blend_factor::one_minus_src_color: {
			return 1.0F - src;
		}
		case blend_factor::dst_color: {
			return dst;
		}
		case blend_factor::one_minus_dst_color: {
			return 1.0F - dst;
		}
		case blend_factor::src_alpha: {
			return src.aaaa();
		}
		case blend_factor::one_minus_src_alpha: {
			return 1.0F - src.aaaa();
		}
		case blend_factor::dst_alpha: {
			return dst.aaaa();
		}
		case blend_factor::one_minus_dst_alpha: {
			return 1.0F - dst.aaaa();
		}
		case blend_factor::src_alpha_saturate: {
			const auto f{lin::min(src.a(), 1.0F - dst.a())};
			return lin::float4{f, f, f, 1.0F};
		}
		default: {
			SDL_InvalidParamError("blend_factor");
			throw std::invalid_argument{SDL_GetError()};
		}}
	}};

	const auto dst{lin::float4{(*this)[coord]} / 255.0F};
	const auto color{resolve_op(blend_state->color_blend_op, src * resolve_factor(blend_state->src_color_blend_factor, src, dst), dst * resolve_factor(blend_state->dst_color_blend_factor, src, dst))};
	(*this)[coord] = lin::cast<std::uint8_t>(lin::saturate(lin::float4{color.r(), color.g(), color.b(), resolve_op(blend_state->alpha_blend_op, src * resolve_factor(blend_state->src_alpha_blend_factor, src, dst), dst * resolve_factor(blend_state->dst_alpha_blend_factor, src, dst)).a()}) * 255.0F);
}

auto image::operator==(const image &other) const
	-> bool
{
	return lin::all(other.size() == size()) && std::ranges::equal(other.bytes(), bytes());
}

void image::save(const std::filesystem::path &path) const
{
	if (!SDL_SavePNG(_handle, path.string().c_str())) {
		throw std::runtime_error{SDL_GetError()};
	}
}
}
