// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Music.hpp"

#include "pspaalib.h"

#include <chrono>
#include <string>

namespace SuperHaxagon {
	static constexpr int AT3_CHANNEL = PSPAALIB_CHANNEL_AT3_1;

	std::chrono::system_clock::time_point getNow() {
		return std::chrono::high_resolution_clock::now();
	}

	struct Music::MusicImpl {
		MusicImpl(const std::string& path) {
			AalibStop(AT3_CHANNEL);
    		AalibWaitThreadFinished(AT3_CHANNEL);
			
    		AalibUnload(AT3_CHANNEL);

			int res = AalibLoad(const_cast<char*>(path.c_str()), AT3_CHANNEL, true);
			if (res != PSPAALIB_SUCCESS) {
				loadFailed = true;
				return;
			}

			AalibSetAutoloop(AT3_CHANNEL, false);
			AalibEnable(AT3_CHANNEL, PSPAALIB_EFFECT_VOLUME_MANUAL);
			AalibSetVolume(AT3_CHANNEL, AalibVolume{1.0f, 1.0f});
		}

		~MusicImpl() {
			AalibStop(AT3_CHANNEL);
			AalibWaitThreadFinished(AT3_CHANNEL);
			AalibUnload(AT3_CHANNEL);
		}

		bool isDone() const {
			if (loadFailed) return true;
			return AalibGetStatus(AT3_CHANNEL) == PSPAALIB_STATUS_STOPPED;
		}

		void play() {
			if (loadFailed) return;

			if (isDone()) {
				paused = false;
				auto now = getNow();
				timeStarted = now;
				timePaused = now;

				AalibRewind(AT3_CHANNEL);
				AalibPlay(AT3_CHANNEL);
				return;
			}

			if (paused) {
				timeStarted += getNow() - timePaused;
				AalibPause(AT3_CHANNEL);
				paused = false;
			}
		}

		void pause() {
			if (loadFailed) return;
			if (paused) return;

			paused = true;
			timePaused = getNow();
			AalibPause(AT3_CHANNEL);
		}

		float getTime() {
			std::chrono::duration<float> duration = paused ? timePaused - timeStarted : getNow() - timeStarted;
			return duration.count();
		}

		void update() {
			if (loadFailed) return;
			if (loop && isDone()) {
				paused = false;
				auto now = getNow();
				timeStarted = now;
				timePaused = now;
				AalibRewind(AT3_CHANNEL);
				AalibPlay(AT3_CHANNEL);
			}
		}

		bool loadFailed = false;

		bool loop = false;
		bool paused = true;
		std::chrono::system_clock::time_point timeStarted;
		std::chrono::system_clock::time_point timePaused;
	};

	Music::Music(std::unique_ptr<Music::MusicImpl> impl) : _impl(std::move(impl)) {}
	Music::~Music() = default;

	void Music::update() const {
		_impl->update();
	}

	void Music::setLoop(const bool loop) const {
		_impl->loop = loop;
	}

	void Music::play() const {
		_impl->play();
	}

	void Music::pause() const {
		_impl->pause();
	}

	bool Music::isDone() const {
		return _impl->isDone();
	}

	float Music::getTime() const {
		return _impl->getTime();
	}

	std::unique_ptr<Music> createMusic(const std::string& path) {
		auto data = std::make_unique<Music::MusicImpl>(path);
		if (data->loadFailed) return nullptr;
		return std::make_unique<Music>(std::move(data));
	}
}