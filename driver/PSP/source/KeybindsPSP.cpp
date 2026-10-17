// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "CreatePSP.hpp"

#include "controls.h"

namespace SuperHaxagon {
	std::string getButtonName(ButtonName buttonName) {
		switch (buttonName) {
			case ButtonName::BACK: return "CIRCLE";
			case ButtonName::SELECT: return "CROSS";
			case ButtonName::LEFT: return "LEFT | L | STICK L";
			case ButtonName::RIGHT: return "RIGHT | R | STICK R";
			default: return "?";
		}
	}
}