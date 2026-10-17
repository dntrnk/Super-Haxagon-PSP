// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Platform.hpp"

#include "glib2d_helper.h"
#include "intraFont.h"
#include "controls.h"
#include "pspaalib.h"

#include "CreatePSP.hpp"
#include "Driver/Tools/Configuration.hpp"
#include "Driver/Tools/Random.hpp"
#include "Driver/Font.hpp"
#include "Driver/Music.hpp"
#include "Driver/Screen.hpp"
#include "Driver/Sound.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <sys/stat.h>

namespace SuperHaxagon {
	std::unique_ptr<Music> createMusic(const std::string& path);
	std::unique_ptr<Screen> createScreen();
	std::unique_ptr<Sound> createSound(const std::string& path);
	std::unique_ptr<Font> createFont(const Platform& platform, const std::string& path, int size);

	struct Platform::PlatformImpl {
		PlatformImpl() : screen(createScreen()) {}

		~PlatformImpl() {
			sceKernelExitGame();
		}

		void gameInit(const Platform& platform) {
			initializePlatform(sdmc, romfs, backslash);

			g2dInit();
			intraFontInit();
			controls_init();
			AalibInit();

			time = std::chrono::high_resolution_clock::now();

			loaded = true;

			platform.loadSound("awesome");
			platform.loadSound("begin");
			platform.loadSound("excellent");
			platform.loadSound("go");
			platform.loadSound("haxagon");
			platform.loadSound("level");
			platform.loadSound("over");
			platform.loadSound("select");
			platform.loadSound("wonderful");
			platform.loadSound("heptagon");
			platform.loadSound("hexagon");
			platform.loadSound("line");
			platform.loadSound("octagon");
			platform.loadSound("pentagon");
			platform.loadSound("square");
			platform.loadSound("triangle");
		}

		bool loop(const Platform& platform) {
			if (!loaded) return false;

			if (software) {
				// We aren't locked to the framerate of a hardware device,
				// so attempt to make our own frame cap of 60fps
				auto now = std::chrono::high_resolution_clock::now();
				auto us = std::chrono::duration_cast<std::chrono::microseconds>(now - time).count();
				auto timeToSleep = 16666 - us;
				if (timeToSleep < 0) timeToSleep = 0;
				std::this_thread::sleep_for(std::chrono::microseconds(timeToSleep));
			}

			auto now = std::chrono::high_resolution_clock::now();
			auto us = std::chrono::duration_cast<std::chrono::microseconds>(now - time).count();
			frameTime = static_cast<float>(us) /  1000000.0f;
			time = now;

			controls_read();

			buttons.select = controls_held(PSP_CTRL_CROSS);
			buttons.back = controls_held(PSP_CTRL_CIRCLE);
			buttons.left = controls_held(PSP_CTRL_LEFT) | controls_held(PSP_CTRL_LTRIGGER) | (controls_AnalogX() < -75);
			buttons.right = controls_held(PSP_CTRL_RIGHT) | controls_held(PSP_CTRL_RTRIGGER) | (controls_AnalogX() > 75);
			buttons.select = controls_held(PSP_CTRL_CROSS);
			buttons.quit = false;

			return true;
		}

		std::string getPath(const std::string& partial, const Location location) const {
			auto path = partial;

			if (backslash) std::replace(path.begin(), path.end(), '/', '\\');

			switch (location) {
				case Location::ROM:
					return romfs + path;
				case Location::USER:
					return sdmc + path;
			}

			return "";
		}

		std::chrono::system_clock::time_point time;
		std::unique_ptr<Screen> screen;
		Buttons buttons{};

		float frameTime = 0.0f;
		bool loaded = false;
		bool software = false;
		bool backslash = false;
		std::string sdmc;
		std::string romfs;
	};

	Platform::Platform() : _impl(std::make_unique<PlatformImpl>()) {
		_impl->gameInit(*this);
	}

	Platform::~Platform() = default;

	void initializePlatform(std::string& sdmc, std::string& romfs, bool& backslash) {
		sdmc = "./sdmc";
		romfs = "./romfs";
		backslash = false;

		mkdir(sdmc.c_str(), 0755);
	}

	bool Platform::loop() {
		return _impl->loop(*this);
	}

	float Platform::getDilation() const {
		return _impl->frameTime / (1.0f/60.0f);
	}

	std::string Platform::getPath(const std::string& partial, const Location location) const {
		return _impl->getPath(partial, location);
	}

	std::unique_ptr<Font> Platform::loadFont(const int size) const {
		if (!_impl->loaded) return nullptr;
		return createFont(*this, getPath("/bump-it-up.ttf", Location::ROM), size);
	}

	std::unique_ptr<Sound> Platform::loadSound(const std::string& base) const {
		return createSound(getPath(base, Location::ROM) + ".wav");
	}

	std::unique_ptr<Music> Platform::loadMusic(const std::string& base, const Location location) const {
		return createMusic(getPath(base, location) + ".at3");
	}

	Screen& Platform::getScreen() {
		return *_impl->screen;
	}

	std::string Platform::getButtonName(ButtonName buttonName) {
		return ::SuperHaxagon::getButtonName(buttonName);
	}

	Buttons Platform::getPressed() const {
		return _impl->buttons;
	}

	void Platform::message(const Dbg dbg, const std::string& where, const std::string& message) const {
		if (dbg == Dbg::INFO) {
			std::cout << "[psp:info] " + where + ": " + message << std::endl;
		} else if (dbg == Dbg::WARN) {
			std::cout << "[psp:warn] " + where + ": " + message << std::endl;
		} else if (dbg == Dbg::FATAL) {
			std::cerr << "[psp:fatal] " + where + ": " + message << std::endl;
		}
	}

	Supports Platform::supports() {
		return Supports::SHADOWS;
	}

	void Platform::shutdown() {}
}
