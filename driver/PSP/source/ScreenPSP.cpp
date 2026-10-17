// SPDX-FileCopyrightText: 2025 AJ Walter
// SPDX-FileCopyrightText: 2026 dntrnk
// SPDX-License-Identifier: GPL-3.0-or-later OR MIT

#include "Driver/Screen.hpp"

#include "glib2d_helper.h"

namespace SuperHaxagon {
	struct Screen::ScreenImpl {
		explicit ScreenImpl() {
			loaded = true;
		}

		~ScreenImpl() = default;

		Vec2f getScreenDim() const {
			return {480, 272};
		}

		void screenFinalize() const {
			g2dHelperFlip();
		}

		void drawPoly(const Color& color, const std::vector<Vec2f>& points) const {
			if (points.size() < 3) return;

			const g2dColor c = G2D_RGBA(color.r, color.g, color.b, color.a);

			const size_t tri_count = points.size() - 2;
			const size_t vert_count = tri_count * 3;

			std::vector<float> xy(vert_count * 2);

			const float x0 = points[0].x;
			const float y0 = points[0].y;

			size_t k = 0;
			for (size_t i = 1; i < points.size() - 1; i++) {
				xy[k++] = x0;
				xy[k++] = y0;

				xy[k++] = points[i].x;
				xy[k++] = points[i].y;

				xy[k++] = points[i + 1].x;
				xy[k++] = points[i + 1].y;
			}

			g2dHelperDrawTriangles(xy.data(), static_cast<int>(vert_count), c);
		}

		void clear(const Color& color) const {
			g2dHelperClear(G2D_RGB(color.r, color.g, color.b));
		}

		bool loaded = false;
	};

	Screen::Screen(std::unique_ptr<ScreenImpl> data) : _impl(std::move(data)) {}

	Screen::~Screen() = default;

	Vec2f Screen::getScreenDim() const {
		return _impl->getScreenDim();
	}

	void Screen::screenBegin() const {}

	void Screen::screenSwitch() const {}

	void Screen::screenFinalize() const {
		_impl->screenFinalize();
	}

	void Screen::drawPoly(const Color& color, const std::vector<Vec2f>& points) const {
		_impl->drawPoly(color, points);
	}

	void Screen::clear(const Color& color) const {
		_impl->clear(color);
	}

	std::unique_ptr<Screen> createScreen() {
		return std::make_unique<Screen>(std::make_unique<Screen::ScreenImpl>());
	}
}
