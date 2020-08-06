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

namespace rack { namespace componentlibrary {

enum class Orientation {
	Horizontal, Vertical
};

template<Orientation O>
struct SlidePot : app::SvgSlider {

	static std::map<Orientation,std::string> backgrounds = {
			std::make_pair(Orientation::Horizontal,"res/SlidePotH.svg"),
			std::make_pair(Orientation::Vertical,"res/SlidePot.svg")
	};

	static std::map<Orientation,std::string> handles = {
			std::make_pair(Orientation::Horizontal,"res/SlidePotHandleH.svg"),
			std::make_pair(Orientation::Vertical,"res/SlidePotHandle.svg")
	};

	SlidePot() {
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
		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,backgrounds[O])));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,handles[O])));
	}
};

using SlidePotH = SlidePot<Orientation::Horizontal>;
using SlidePotV = SlidePot<Orientation::Vertical>;

struct LatchingPushButton : public app::SvgSwitch {
	LatchingPushButton();
};

template<Orientation O>
struct OnOffSwitch2 : public app::SvgSwitch {

	static std::map<Orientation,std::string> offs = {
				std::make_pair(Orientation::Horizontal,"res/NNK_0H.svg"),
				std::make_pair(Orientation::Vertical,"res/NNK_0.svg")
		};

		static std::map<Orientation,std::string> ons = {
				std::make_pair(Orientation::Horizontal,"res/NNK_2H.svg"),
				std::make_pair(Orientation::Vertical,"res/NNK2.svg")
		};

	OnOffSwitch2() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,offs[O])));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,ons[O])));
	}
};

using OnOffSwitch2V = OnOffSwitch2<Orientation::Vertical>;
using OnOffSwitch2H = OnOffSwitch2<Orientation::Horizontal>;


struct FourSwitch : public app::SvgSwitch {
	FourSwitch();
};

template <typename S, class = typename std::enable_if<std::is_base_of<SvgSwitch,S>::value>::type>
struct ActionButton : S {

	ActionButton()  : S() {
		momentary=true;
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
