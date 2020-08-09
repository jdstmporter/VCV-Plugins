/*
 * ActionButton.hpp
 *
 *  Created on: 4 Aug 2020
 *      Author: julianporter
 */

#ifndef SRC_ETC_ACTIONBUTTON_HPP_
#define SRC_ETC_ACTIONBUTTON_HPP_

#include <rack.hpp>
#include <event.hpp>
#include <vector>
#include <map>
#include <type_traits>

template <typename T, class = typename std::enable_if<std::is_base_of<rack::app::Switch,T>::value>::type>
struct ActionSwitch : T {
private:
	rack::ParamWidget *callback;
public:
	ActionSwitch()  : rack::SvgSwitch(), callback(nullptr) {
		this->momentary=true;
	}
	virtual ~ActionSwitch() = default;

	void setCallback(rack::ParamWidget *cb) { callback = cb; }

	virtual void onChange(const rack::event::Change &e) {
		rack::SvgSwitch::onChange(e);
		if(callback != nullptr && paramQuantity != nullptr) {
			if(paramQuantity->isMin()) callback->step();
		}
	}
};

template<unsigned N>
struct RadioButtons : public rack::ParamWidget {
public:
	rack::widget::FramebufferWidget* fb;
	ActionSwitch<rack::BefacoPush> *sw;
	unsigned it;

public:
	RadioButtons() : rack::ParamWidget(), it(0) {
		fb = new rack::widget::FramebufferWidget;
		addChild(fb);

		for(unsigned i=0;i<N;i++) {
			auto light=new rack::MediumLight<rack::RedLight>();
			fb->addChild(light);
		}
		sw = new rack::BefacoPush();
		sw->callback=this;
		fb->addChild(sw);


	};
	virtual ~RadioButtons() = default;

	unsigned selected() const { return it; }

	virtual void onChange(const rack::event::Change &e) {
		it=(it+1)%N;
	};

	virtual void reset() {};
};

#endif /* SRC_ETC_ACTIONBUTTON_HPP_ */
