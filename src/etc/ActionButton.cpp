/*
 * ActionButton.cpp
 *
 *  Created on: 4 Aug 2020
 *      Author: julianporter
 */

#include "ActionButton.hpp"
#include <algorithm>

static std::map<bool,std::vector<float>> MultiSwitch::onOff = {
		std::make_pair(false , std::vector<float>(1,0.f)),
		std::make_pair(true, std::vector<float>(1,1.f))
};


void MultiSwitch::add(const Unit &unit) {
	unit.button->uid=units.size();
	unit.button->callback=this;
	units.push_back(unit);
}

void MultiSwitch::setSelected(const unsigned s) {
	selected=s;
	std::for_each(units.begin(),units.end(),[s,&onOff](Unit &unit) {
		std::vector<float> v=onOff[unit.button->uid == s];
		unit.light->setBrightnesses(v);
	});
}


