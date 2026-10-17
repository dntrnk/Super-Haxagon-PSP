// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "CreateSDL2.hpp"

#include "controls.h"

namespace SuperHaxagon {
	void setKey(Buttons& buttons, int key, bool pressing) {
		(void) buttons;
		(void) key;
		(void) pressing;
	}

	std::string getButtonName(ButtonName buttonName, bool showKbdControls, ControllerSettings settings) {
		(void) showKbdControls;
		(void) settings;

		switch (buttonName) {
			case ButtonName::BACK: return "CIRCLE";
			case ButtonName::SELECT: return "CROSS";
			case ButtonName::LEFT: return "LEFT | L | STICK L";
			case ButtonName::RIGHT: return "RIGHT | R | STICK R";
			default: return "?";
		}
	}
}