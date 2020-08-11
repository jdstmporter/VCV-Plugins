/*
 * widgets.hpp
 *
 *  Created on: 31 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_SHARED_SIMPLEWIDGETS_HPP_
#define SRC_SHARED_SIMPLEWIDGETS_HPP_

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
		switch(O) {
		case Orientation::Vertical:
			auto margin = math::Vec(3.5, 3.5);
			maxHandlePos = math::Vec(-1, -2).plus(margin);
			minHandlePos = math::Vec(-1, 87).plus(margin);
			setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/BefacoSlidePot.svg")));
			setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/BefacoSlidePotHandle.svg")));
			background->box.pos = margin;
			box.size = background->box.size.plus(margin.mult(2));
			break;
		case Orientation::Horizontal:
			auto margin = math::Vec(0,0);
			maxHandlePos = app::mm2px(math::Vec(16.578, 0.738).plus(margin));
			minHandlePos = app::mm2px(math::Vec(0.738, 0.738).plus(margin));
			setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotH.svg")));
			setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotHandleH.svg")));
			break;
		}

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









}}


#endif /* SRC_SHARED_SIMPLEWIDGETS_HPP_ */
