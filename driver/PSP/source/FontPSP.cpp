// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Font.hpp"

#include "Driver/Platform.hpp"

#include "intraFont.h"
#include "glib2d.h"

#include <utility>

namespace SuperHaxagon {
	struct Font::FontImpl {
		FontImpl(const Platform& platform, std::string path, int size) :
			platform(platform),
			path(std::move(path)),
			size(size),
			requestedSize(1.08889) {
			font = intraFontLoadTTF(this->path.c_str(), INTRAFONT_CACHE_ASCII, static_cast<float>(size));
			if (!font) {
				platform.message(Dbg::WARN, "font", "failed to load " + this->path);
			}
		}

		~FontImpl() {
			if (font) intraFontUnload(font);
		}

		void setScale(float scale) {
			currentScale = scale;

			const float realScale = (scale - 1.0f) / 1.5f + 1.0f;
			requestedSize = realScale;
		}

		float getHeight() const {
			if (!font) return 0.0f;
			return font->glyph[1].height * requestedSize;
		}

		float getWidth(const std::string& str) const {
			if (!font || str.empty()) return 0.0f;
			intraFontSetStyle(font, requestedSize, 0xFFFFFFFF, 0, 0, INTRAFONT_STRING_UTF8);
			return intraFontMeasureText(font, str.c_str());
		}

		void draw(const Color& color, const Vec2f& position, const Alignment alignment, const std::string& text) const {
			if (!font || text.empty()) return;

			float x = position.x;
			if (alignment == Alignment::CENTER) {
				const float w = getWidth(text);
				x -= w / 2.0f;
			} else if (alignment == Alignment::RIGHT) {
				x -= getWidth(text);
			}

			const g2dColor c = G2D_RGBA(color.r, color.g, color.b, color.a);

			intraFontSetStyle(font, requestedSize, c, 0, 0, INTRAFONT_STRING_UTF8 | INTRAFONT_ALIGN_LEFT);
			intraFontPrint(font, x, position.y + intraFontTextHeight(font), text.c_str());
		}

		const Platform& platform;
		std::string path;
		int size;
		float currentScale = 1.0f;
		float requestedSize = 1.0f;
		intraFont* font = nullptr;
	};

	Font::Font(std::unique_ptr<Font::FontImpl> impl) : _impl(std::move(impl)) {}

	Font::~Font() = default;

	void Font::setScale(const float scale) {
		_impl->setScale(scale);
	}

	float Font::getHeight() const {
		return _impl->getHeight();
	}

	float Font::getWidth(const std::string& str) const {
		return _impl->getWidth(str);
	}

	void Font::draw(const Color& color, const Vec2f& position, const Alignment alignment, const std::string& text) const {
		_impl->draw(color, position, alignment, text);
	}

	std::unique_ptr<Font> createFont(const Platform& platform, const std::string& path, int size) {
		return std::make_unique<Font>(std::make_unique<Font::FontImpl>(platform, path, size));
	}
}
