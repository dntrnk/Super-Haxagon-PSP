// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Sound.hpp"

#include "pspaalib.h"

#include <string>

namespace SuperHaxagon {
	static bool wavChannelUsed[32] = {false};

	static int acquireWavChannel() {
		for (int i = 0; i < 32; i++) {
			if (!wavChannelUsed[i]) {
				wavChannelUsed[i] = true;
				return PSPAALIB_CHANNEL_WAV_1 + i;
			}
		}
		return -1;
	}

	static void releaseWavChannel(int channel) {
		if (channel < PSPAALIB_CHANNEL_WAV_1 || channel > PSPAALIB_CHANNEL_WAV_32) return;
		wavChannelUsed[channel - PSPAALIB_CHANNEL_WAV_1] = false;
	}

	struct Sound::SoundImpl {
		SoundImpl(const std::string& path) {
			_channel = acquireWavChannel();
			if (_channel < 0) {
				loadFailed = true;
				return;
			}

			int res = AalibLoad(const_cast<char*>(path.c_str()), _channel, true);
			if (res != PSPAALIB_SUCCESS) {
				loadFailed = true;
				releaseWavChannel(_channel);
				_channel = -1;
				return;
			}

			AalibSetAutoloop(_channel, false);
			AalibEnable(_channel, PSPAALIB_EFFECT_VOLUME_MANUAL);
			AalibSetVolume(_channel, AalibVolume{1.0f, 1.0f});
		}

		~SoundImpl() {
			if (_channel < 0) return;
			AalibStop(_channel);
			AalibWaitThreadFinished(_channel);
			AalibUnload(_channel);
			releaseWavChannel(_channel);
		}

		void play() {
			if (_channel < 0) return;
			if (AalibGetStatus(_channel) == PSPAALIB_STATUS_STOPPED) {
				AalibRewind(_channel);
				AalibPlay(_channel);
			} else {
				AalibStop(_channel);
				AalibWaitThreadFinished(_channel);
				AalibRewind(_channel);
				AalibPlay(_channel);
			}
		}

		int _channel = -1;
		bool loadFailed = false;
	};

	Sound::Sound(std::unique_ptr<SoundImpl> impl) : _impl(std::move(impl)) {}
	Sound::~Sound() = default;

	void Sound::play() const {
		_impl->play();
	}

	std::unique_ptr<Sound> createSound(const std::string& path) {
		auto data = std::make_unique<Sound::SoundImpl>(path);
		if (data->loadFailed) return nullptr;
		return std::make_unique<Sound>(std::move(data));
	}
}