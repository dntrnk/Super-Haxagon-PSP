#ifndef DATA_PSP_HPP
#define DATA_PSP_HPP

// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Platform.hpp"

#include <string>

namespace SuperHaxagon {
	void initializePlatform(std::string& sdmc, std::string& romfs, bool& backslash);

	std::string getButtonName(ButtonName buttonName);
}

#endif // DATA_PSP_HPP