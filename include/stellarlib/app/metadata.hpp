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

#ifndef STELLARLIB_APP_METADATA_HPP
#define STELLARLIB_APP_METADATA_HPP

#include <stellarlib/app/lifecycle.hpp>

#include <string>

/**
 * @brief Application runtime
 */
namespace stellarlib::app
{
/**
 * @brief Metadata subsystem
 */
class metadata final
{
friend internal::lifecycle<class context>;

public:
	/**
	 * @brief Metadata initialization descriptor
	 */
	struct info final
	{
		/**
		 * @brief Name of the application
		 */
		std::string name;

		/**
		 * @brief Version of the application
		 */
		std::string version;

		/**
		 * @brief Reverse-domain identifier of the application
		 */
		std::string identifier;

		/**
		 * @brief Creator of the application
		 */
		std::string creator;

		/**
		 * @brief Copyright notice of the application
		 */
		std::string copyright;

		/**
		 * @brief URL of the application's website
		 */
		std::string url;

		/**
		 * @brief Type of the application (e.g., "application", "game")
		 */
		std::string type;
	};

	/**
	 * @brief Deleted copy constructor
	 */
	[[nodiscard]]
	constexpr metadata(const metadata &) noexcept = delete;

	/**
	 * @brief Deleted move constructor
	 */
	[[nodiscard]]
	constexpr metadata(metadata &&) noexcept = delete;

	/**
	 * @brief Deleted copy assignment operator
	 */
	constexpr auto operator=(const metadata &) noexcept
		-> metadata & = delete;

	/**
	 * @brief Deleted move assignment operator
	 */
	constexpr auto operator=(metadata &&) noexcept
		-> metadata & = delete;

	/**
	 * @brief Destructor
	 */
	~metadata();

	/**
	 * @brief Returns the name of the application
	 * @return Name of the application
	 */
	[[nodiscard]]
	auto name() const
		-> std::string;

	/**
	 * @brief Returns the version of the application
	 * @return Version of the application
	 */
	[[nodiscard]]
	auto version() const
		-> std::string;

	/**
	 * @brief Returns the reverse-domain identifier of the application
	 * @return Reverse-domain identifier of the application
	 */
	[[nodiscard]]
	auto identifier() const
		-> std::string;

	/**
	 * @brief Returns the creator of the application
	 * @return Creator of the application
	 */
	[[nodiscard]]
	auto creator() const
		-> std::string;

	/**
	 * @brief Returns the copyright notice of the application
	 * @return Copyright notice of the application
	 */
	[[nodiscard]]
	auto copyright() const
		-> std::string;

	/**
	 * @brief Returns the URL of the application's website
	 * @return URL of the application's website
	 */
	[[nodiscard]]
	auto url() const
		-> std::string;

	/**
	 * @brief Returns the type of the application (e.g., "application", "game")
	 * @return Type of the application (e.g., "application", "game")
	 */
	[[nodiscard]]
	auto type() const
		-> std::string;

private:
	[[nodiscard]]
	explicit metadata(const info &info);
};
}

#endif
