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

#ifndef STELLARLIB_APP_CLOCK_HPP
#define STELLARLIB_APP_CLOCK_HPP

#include <stellarlib/app/lifecycle.hpp>

/**
 * @brief Application runtime
 */
namespace stellarlib::app
{
/**
 * @brief Clock subsystem
 */
class clock final
{
friend internal::lifecycle<class context>;

public:
	/**
	 * @brief Clock initialization descriptor
	 */
	struct info final
	{
		/**
		 * @brief Target scene update frequency in hertz [0.0F, +INF) (0.0F means as fast as possible)
		 */
		float target_frequency;

		/**
		 * @brief Maximum allowed delta time in seconds [0.0F, +INF)
		 */
		float max_delta;
	};

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr clock(const clock &) noexcept = delete;

	/**
	 * @brief Deleted move constructor
	 */
	[[nodiscard]]
	constexpr clock(clock &&) noexcept = delete;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const clock &) noexcept
		-> clock & = delete;

	/**
	 * @brief Deleted move assignment operator
	 */
	constexpr auto operator=(clock &&) noexcept
		-> clock & = delete;

	/**
	 * @brief Destructor
	 */
	~clock();

	/**
	 * @brief Returns the elapsed time in seconds since context initialization
	 * @return Elapsed time in seconds since context initialization
	 */
	[[nodiscard]]
	auto now() const
		-> float;

	/**
	 * @brief Returns the target scene update frequency in hertz [0.0F, +INF) (0.0F means as fast as possible)
	 * @return Target scene update frequency in hertz [0.0F, +INF) (0.0F means as fast as possible)
	 */
	[[nodiscard]]
	auto target_frequency() const
		-> float;

	/**
	 * @brief Sets the target scene update frequency in hertz [0.0F, +INF) (0.0F means as fast as possible)
	 * @param target_frequency Target scene update frequency in hertz [0.0F, +INF) (0.0F means as fast as possible)
	 */
	void set_target_frequency(float target_frequency);

	/**
	 * @brief Returns the elapsed time in seconds between context initialization and the last scene update
	 * @return Elapsed time in seconds between context initialization and the last scene update
	 */
	[[nodiscard]]
	auto frame() const
		-> float;

	/**
	 * @brief Returns the maximum allowed delta time in seconds [0.0F, +INF)
	 * @return Maximum allowed delta time in seconds [0.0F, +INF)
	 */
	[[nodiscard]]
	auto max_delta() const
		-> float;

	/**
	 * @brief Sets the maximum allowed delta time in seconds [0.0F, +INF)
	 * @param max_delta Maximum allowed delta time in seconds [0.0F, +INF)
	 */
	void set_max_delta(float max_delta);

	/**
	 * @brief Returns the elapsed time in seconds between the previous and current scene updates
	 * @return Elapsed time in seconds between the previous and current scene updates
	 */
	[[nodiscard]]
	auto delta() const
		-> float;

private:
	float _delta{};
	float _frame{now()};
	float _max_delta{};

	[[nodiscard]]
	explicit clock(const info &info);

	void iterate();
};
}

#endif
