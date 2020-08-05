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

struct ActionSwitch : rack::SvgSwitch {
	struct Callback {
		Callback() = default;
		virtual ~Callback() = default;
		virtual void operator()(const unsigned uid) = 0;
	};

	unsigned uid;
	Callback *callback;

	ActionSwitch()  : rack::SvgSwitch(), uid(0), callback(nullptr) {
		momentary=true;
	}
	virtual ~ActionSwitch() = default;

	void setCallback(const Callback *cb) { callback = cb; }
	virtual void onChange(const rack::event::Change &e) {
		rack::SvgSwitch::onChange(e);
		if(callback!=nullptr) (*callback)(uid);
	}
};

struct MultiSwitch : public ActionSwitch::Callback {
public:

	struct Unit {
		ActionSwitch *button;
		rack::ModuleLightWidget *light;

		Unit(ActionSwitch *b,rack::ModuleLightWidget *l) : button(b), light(l) {};
		virtual ~Unit() = default;
	};

private:
	static std::map<bool,std::vector<float>> onOff;

	std::vector<Unit> units;
	unsigned def;
	unsigned selected;

public:
	MultiSwitch() : ActionSwitch::Callback(), units(), def(0), selected(0) {};
	virtual ~MultiSwitch() = default;

	void add(const Unit &unit);
	void add(ActionSwitch *b,rack::ModuleLightWidget *l) { add(Unit(b,l)); }
	void setDefault(const unsigned d) { def = d; }
	unsigned getSelected() const { return selected; }
	void setSelected(const unsigned);
	void reset() { setSelected(def); }
	void operator()(const unsigned uid) { setSelected(uid); }
};

#endif /* SRC_ETC_ACTIONBUTTON_HPP_ */
