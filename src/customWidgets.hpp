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

namespace rack { namespace componentlibrary {

struct SlidePot : app::SvgSlider {
	SlidePot();
};

struct SlidePotH : app::SvgSlider {
	SlidePotH();
};

struct LatchingPushButton : public app::SvgSwitch {
	LatchingPushButton();
};

struct NKK2 : public app::SvgSwitch {
	NKK2();
};

struct FourSwitch : public app::SvgSwitch {
	FourSwitch();
};

}}


#endif /* SRC_SHARED_CUSTOMWIDGETS_HPP_ */
