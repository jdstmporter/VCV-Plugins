/*
 * ActionButton.cpp
 *
 *  Created on: 4 Aug 2020
 *      Author: julianporter
 */

#include "ActionButton.hpp"
#include <event.hpp>


void ActionSwitch::step() {
	if(momentaryPressed) (*callback)(uid);
	Switch::step();

}
