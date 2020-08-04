/*
 * ActionButton.hpp
 *
 *  Created on: 4 Aug 2020
 *      Author: julianporter
 */

#ifndef SRC_ETC_ACTIONBUTTON_HPP_
#define SRC_ETC_ACTIONBUTTON_HPP_

#include <rack.hpp>
#include <memory>
#include <vector>

struct ActionSwitchCallback {


	ActionSwitchCallback() = default;
	virtual ~ActionSwitchCallback() = default;
	virtual void operator()(const unsigned uid) = 0;
};

struct ActionSwitch : rack::SVGSwitch {
	
	unsigned uid = 0;
	ActionSwitchCallback *callback=nullptr;

	virtual void step();
};

struct ActionMultiSwitch : ActionSwitchCallback {
	std::vector<ActionSwitch*> switches;

	ActionMultiSwitch() : switches() {
	}

	void add(ActionSwitch *sw) {
		sw->uid=switches.size();
		sw->callback=this;
		switches.push_back(sw);
	}

	void operator()(const unsigned uid) {
		// set the value to uid
	}
};

#endif /* SRC_ETC_ACTIONBUTTON_HPP_ */
