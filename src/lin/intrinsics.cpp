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

#include <stellarlib/lin/intrinsics.hpp>

#include <bit>
#include <cstdint>
#include <cmath>

namespace stellarlib::lin
{
auto acos(const float x) noexcept
	-> float
{
	return std::acosf(x);
}

auto asin(const float x) noexcept
	-> float
{
	return std::asinf(x);
}

void asuint(const double value, std::uint32_t &lowbits, std::uint32_t &highbits) noexcept
{
    lowbits = static_cast<std::uint32_t>(std::bit_cast<std::uint64_t>(value));
    highbits = static_cast<std::uint32_t>(std::bit_cast<std::uint64_t>(value) >> std::uint8_t{32});
}

auto atan(const float x) noexcept
	-> float
{
	return std::atanf(x);
}

auto atan2(const float y, const float x) noexcept
	-> float
{
	return std::atan2f(y, x);
}

auto ceil(const float x) noexcept
	-> float
{
	return std::ceilf(x);
}

auto cos(const float x) noexcept
	-> float
{
	return std::cosf(x);
}

auto cosh(const float x) noexcept
	-> float
{
	return std::coshf(x);
}

auto exp(const float x) noexcept
	-> float
{
	return std::expf(x);
}

auto exp2(const float x) noexcept
	-> float
{
	return std::exp2f(x);
}

auto floor(const float x) noexcept
	-> float
{
	return std::floorf(x);
}

auto fmod(const float x, const float y) noexcept
	-> float
{
	return std::fmodf(x, y);
}
}
