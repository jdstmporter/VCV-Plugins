/*
 * multiSwitch.hpp
 *
 *  Created on: 11 Aug 2020
 *      Author: julianporter
 */

#ifndef SRC_WIDGETS_MULTISWITCH_HPP_
#define SRC_WIDGETS_MULTISWITCH_HPP_

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

template <typename T, class = typename std::enable_if<std::is_base_of<rack::app::Switch,T>::value>::type>
struct ActionSwitch : T {
	ParamWidget *callback;

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

struct TMultiSwitch : public ParamWidget {
protected:
	Module *module;
	unsigned offset;

	virtual void stepValue() = 0;
	virtual void updateLights() = 0;

	bool dummy() const { return paramQuantity == nullptr || module==nullptr; }

public:

	TMultiSwitch(Module *m,const unsigned o=0,const int id=0) : ParamWidget(), module(m), offset(o) {
		visible=false;
		if(module!=nullptr) paramQuantity = module->paramQuantities[id];
	}
	virtual ~TMultiSwitch() = default;

	virtual void reset() override {
		if(dummy()) return;
		paramQuantity->reset();
		updateLights();
	}

	virtual void step() override {
			if(dummy()) return;
			stepValue();
			updateLights();
			event::Change eChange;
			onChange(eChange);
		};

};


template<typename T, class = typename std::is_convertible<float,T>::type>
struct MultiSwitchBase : public TMultiSwitch {
protected:

public:
	MultiSwitchBase(Module *m,const unsigned o=0,const int id=0) : TMultiSwitch(m,o,id) {};
	virtual ~MultiSwitchBase() = default;

	virtual T value() const {
		if(dummy()) return (T)0.f;
		return (T)paramQuantity->getValue();
	}
	virtual void setValue(const T n) {
		if(dummy()) return;
		paramQuantity->setValue(float(n));
	}


};

template<unsigned N>
struct MultiSwitch : public MultiSwitchBase<int> {
protected:

	virtual void stepValue() override {
		if(dummy()) return;
		setValue((value()+1)%N);
	}

	void updateLights() override {
		if(dummy()) return;
		auto v=(unsigned)value();
		for(unsigned i=0;i<N;i++) {
			auto b = (i==v) ? 1.f : 0.f;
			module->lights[i+offset].setBrightness(b);
		}
	}

public:
	MultiSwitch(Module *m,const unsigned o=0,const int id=0) : MultiSwitchBase(m,o,id) {
		if(paramQuantity!=nullptr) {
			paramQuantity->minValue=0.f;
			paramQuantity->maxValue=(float)(N-1);
			paramQuantity->defaultValue = (float)(N-1);
		}
	}
	virtual ~MultiSwitch() = default;

};

struct OnOffSwitch : public MultiSwitchBase<bool> {
protected:
	virtual void stepValue() override {
		if(dummy()) return;
		setValue(!value());
	}

	void updateLights() override {
		if(dummy()) return;
		auto b = value() ? 1.f : 0.f;
		module->lights[offset].setBrightness(b);
	}

public:
	OnOffSwitch(Module *m,const unsigned o=0,const int id=0) : MultiSwitchBase(m,o,id) {
		if(paramQuantity!=nullptr) {
			paramQuantity->minValue=0.f;
			paramQuantity->maxValue=1.f;
			paramQuantity->defaultValue = 0.f;
		}
	}
	virtual ~OnOffSwitch() = default;

};

}}


#endif /* SRC_WIDGETS_MULTISWITCH_HPP_ */
