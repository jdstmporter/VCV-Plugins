/*
 * widgets.hpp
 *
 *  Created on: 31 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_SHARED_CUSTOMWIDGETS_HPP_
#define SRC_SHARED_CUSTOMWIDGETS_HPP_

#pragma once
#include <rack.hpp>
#include <app/SvgSlider.hpp>
#include <app/SvgPort.hpp>
#include <app/ModuleLightWidget.hpp>
#include <app/SvgSwitch.hpp>
#include <asset.hpp>
#include <map>
#include <type_traits>
#include <string>

namespace rack { namespace componentlibrary {

enum class Orientation {
	Horizontal, Vertical
};



template<Orientation O>
struct SlidePot : app::SvgSlider {

public:
	SlidePot() : app::SvgSlider() {

		math::Vec margin = (O==Orientation::Vertical) ? math::Vec(3.5, 3.5) : math::Vec(0,2);
		horizontal=(O==Orientation::Horizontal);

		switch(O) {
		case Orientation::Vertical:
			maxHandlePos = math::Vec(-1, -2).plus(margin);
			minHandlePos = math::Vec(-1, 87).plus(margin);
			background->box.pos = margin;
			box.size = background->box.size.plus(margin.mult(2));
			break;
		case Orientation::Horizontal:
			maxHandlePos = app::mm2px(math::Vec(16.578, 0.738).plus(margin));
			minHandlePos = app::mm2px(math::Vec(0.738, 0.738).plus(margin));
			break;
		}
		auto isH = O==Orientation::Horizontal;
		auto bg = isH ? "res/SlidePotH.svg" : "res/SlidePot.svg";
		auto handle = isH ? "res/SlidePotHandleH.svg" : "res/SlidePotHandle.svg";

		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,bg)));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,handle)));
	}
};

using SlidePotH = SlidePot<Orientation::Horizontal>;
using SlidePotV = SlidePot<Orientation::Vertical>;

struct LatchingPushButton : public app::SvgSwitch {
	LatchingPushButton();
};



template<Orientation O>
struct OnOffSwitch2 : public app::SvgSwitch {


public:
	OnOffSwitch2() : app::SvgSwitch() {

		auto isH = O==Orientation::Horizontal;
		auto off = isH ? "res/NKK_0H.svg" : "res/NKK_0.svg";
		auto on = isH ? "res/NKK_2H.svg" : "res/NKK_2.svg";

		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,off)));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,on)));
	};
};

using OnOffSwitch2V = OnOffSwitch2<Orientation::Vertical>;
using OnOffSwitch2H = OnOffSwitch2<Orientation::Horizontal>;


struct FourSwitch : public app::SvgSwitch {
	FourSwitch();
};

template <typename S, class = typename std::enable_if<std::is_base_of<app::Switch,S>::value>::type>
struct ActionButton : public S {

	ActionButton()  : S() {
		static_assert(std::is_base_of<app::Switch,S>::value,"ActionButton must derive from Switch");
		this->momentary=true;
	}
	virtual ~ActionButton() = default;

	virtual void onChange(const rack::event::Change &e) {
		S::onChange(e);
		didFire(e);
	}

	virtual void didFire(const rack::event::Change &) = 0;
};

using BefacoActionButton = ActionButton<BefacoPush>;
using LEDActionButton = ActionButton<LEDButton>;
using CKD6ActionButton = ActionButton<CKD6>;
using TL1105ActionButton = ActionButton<TL1105>;
}}


#endif /* SRC_SHARED_CUSTOMWIDGETS_HPP_ */
