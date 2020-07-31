/*
 * widgets.cpp
 *
 *  Created on: 31 Jul 2020
 *      Author: julianporter
 */


#include "plugin.hpp"
#include "customWidgets.hpp"

namespace rack { namespace componentlibrary {

NKK2::NKK2()   {
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_2.svg")));
	}

FourSwitch::FourSwitch()  {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_3.svg")));
	}


SlidePot::SlidePot()  {
		math::Vec margin = math::Vec(3.5, 3.5);
		maxHandlePos = math::Vec(-1, -2).plus(margin);
		minHandlePos = math::Vec(-1, 87).plus(margin);
		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePot.svg")));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotHandle.svg")));
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
	}


SlidePotH::SlidePotH()  {
		horizontal = true;
		maxHandlePos = app::mm2px(math::Vec(16.578, 0.738).plus(math::Vec(0, 2)));
		minHandlePos = app::mm2px(math::Vec(0.738, 0.738).plus(math::Vec(0, 2)));
		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotH.svg")));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotHandleH.svg")));
	}


LatchingPushButton::LatchingPushButton()  {
		momentary = false;
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_1.svg")));
	}




}}
