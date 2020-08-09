/*
 * widgets.cpp
 *
 *  Created on: 31 Jul 2020
 *      Author: julianporter
 */


#include "plugin.hpp"
#include "customWidgets.hpp"

namespace rack { namespace componentlibrary {





FourSwitch::FourSwitch()  {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_3.svg")));
	}




LatchingPushButton::LatchingPushButton()  {
		momentary = false;
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_1.svg")));
	}




}}
