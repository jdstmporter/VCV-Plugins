/*
 * text.hpp
 *
 *  Created on: 12 Aug 2020
 *      Author: julianporter
 */

#ifndef SRC_WIDGETS_TEXT_HPP_
#define SRC_WIDGETS_TEXT_HPP_

#include <rack.hpp>
#include <nanovg.h>
#include <color.hpp>
#include <blendish.h>

namespace rack { namespace componentlibrary {

struct TextDisplayField : public LedDisplayTextField {

	NVGcolor background;
	NVGcolor highlight;
	float fontSize = NAN;

	TextDisplayField() : LedDisplayTextField() {
		background = nvgRGBAf(0,0,0,0);
		color = nvgRGBAf(0,0,0,1);
		highlight = nvgRGBAf(0,0,0,0.5);
	};
	virtual ~TextDisplayField() = default;

	void setFG(const NVGcolor &c) {
		color=c;
		highlight=nvgTransRGBAf(c,0.5f);
	}
	void setFG(const float r,const float g,const float b,const float a=1.f) { setFG(nvgRGBAf(r,g,b,a)); }

	void setBG(const NVGcolor &c) { background=c; }
	void setBG(const float r,const float g,const float b,const float a=1.f) { setBG(nvgRGBAf(r,g,b,a)); }

	void loadFont(const std::string &f) { font=APP->window->loadFont(f); }
	void setFontSize(const float s) { fontSize=s; }

	virtual void draw(const Widget::DrawArgs &args) override {
		NVGcontext *ctx=args.vg;
		nvgScissor(ctx, RECT_ARGS(args.clipBox));

			// Background
			nvgBeginPath(ctx);
			nvgRoundedRect(ctx, 0, 0, box.size.x, box.size.y, 5.0);
			nvgFillColor(ctx, background);
			nvgFill(ctx);

			// Text
			nvgScale(ctx,0.75,0.75);
			if (font->handle >= 0) {

				bndSetFont(font->handle);
				if(std::isfinite(fontSize)) nvgFontSize(ctx,fontSize);

				int begin = std::min(cursor, selection);
				int end = (this == APP->event->selectedWidget) ? std::max(cursor, selection) : -1;
				bndIconLabelCaret(ctx, textOffset.x, textOffset.y,
				                  box.size.x - 2 * textOffset.x, box.size.y - 2 * textOffset.y,
				                  -1, (NVGcolor)color, 12, text.c_str(), highlight, begin, end);

				bndSetFont(APP->window->uiFont->handle);

			}
			nvgResetTransform(ctx);
			nvgResetScissor(ctx);

	}


};

}}



#endif /* SRC_WIDGETS_TEXT_HPP_ */
