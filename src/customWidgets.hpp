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

struct SwitchCallback {
	SwitchCallback() = default;
	virtual ~SwitchCallback() = default;

	virtual void step() = 0;
	virtual void reset() = 0;
};

template <typename T, class = typename std::enable_if<std::is_base_of<rack::app::Switch,T>::value>::type>
struct ActionSwitch : T {
	SwitchCallback *callback;

	ActionSwitch()  : T(), callback(nullptr) {
		this->momentary=true;
	}
	virtual ~ActionSwitch() = default;

	void setCallback(rack::ParamWidget *cb) { callback = cb; }

	virtual void onChange(const rack::event::Change &e) override {
		rack::SvgSwitch::onChange(e);
		if(callback != nullptr && this->paramQuantity != nullptr) {
			if(this->paramQuantity->isMin()) callback->step();
		}
	}
};

template<unsigned N>
struct MultiSwitch : public SwitchCallback {
	Module *module;
	unsigned offset;
	unsigned value;

	MultiSwitch(Module *m,const unsigned o=0) : SwitchCallback(), module(m), offset(o), value(N-1) {}
	virtual ~MultiSwitch() = default;


	void updateLights() {
		if(module==nullptr) return;
		for(unsigned i=0;i<N;i++) {
			auto b = (i==value) ? 1.f : 0.f;
			module->lights[i+offset].setBrightness(b);
		}
	}

	virtual void reset() override {
		value=N-1;
		updateLights();
	}

	virtual void step() override {
		value=(value+1)%N;
		updateLights();
		INFO("Current multiswitch value is %d",value);
	};

};

struct OnOffSwitch : public SwitchCallback {
	Module *module;
	unsigned offset;
	bool value;

	OnOffSwitch(Module *m,const unsigned o=0) : SwitchCallback(), module(m), offset(o), value(false) {}
	virtual ~OnOffSwitch() = default;



	virtual void reset() override {
		value=false;
		module->lights[offset].setBrightness(0.0);
	}


	virtual void step() override {
		INFO("Click");
		value = !value;
		auto b = value ? 1.f : 0.f;
		module->lights[offset].setBrightness(b);
	}
};


}}


#endif /* SRC_SHARED_CUSTOMWIDGETS_HPP_ */
