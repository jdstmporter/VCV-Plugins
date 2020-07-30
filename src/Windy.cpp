#include "plugin.hpp"

struct FourSwitch : app::SvgSwitch {
	FourSwitch() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_3.svg")));
	}
};

struct SlidePot : app::SvgSlider {
	SlidePot() {
		math::Vec margin = math::Vec(3.5, 3.5);
		maxHandlePos = math::Vec(-1, -2).plus(margin);
		minHandlePos = math::Vec(-1, 87).plus(margin);
		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePot.svg")));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotHandle.svg")));
		background->box.pos = margin;
		box.size = background->box.size.plus(margin.mult(2));
	}
};

struct SlidePotH : app::SvgSlider {
	SlidePotH() {
		horizontal = true;
		maxHandlePos = app::mm2px(math::Vec(16.578, 0.738).plus(math::Vec(0, 2)));
		minHandlePos = app::mm2px(math::Vec(0.738, 0.738).plus(math::Vec(0, 2)));
		setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotH.svg")));
		setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/SlidePotHandleH.svg")));
	}
};

struct LatchingPushButton : app::SvgSwitch {
	LatchingPushButton() {
		momentary = false;
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/BefacoPush_1.svg")));
	}
};

struct NKK2 : app::SvgSwitch {
	NKK2() {
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_0.svg")));
		addFrame(APP->window->loadSvg(asset::system("res/ComponentLibrary/NKK_2.svg")));
	}
};

struct Windy : Module {
	enum ParamIds {
		BOOST_PARAM,
		                RINGING_PARAM,
		                WAVEFORM_PARAM,
		                UPPER_PARAM,
		                LOWER_PARAM,
		                PNORMAL_PARAM,
		                PRATIO_PARAM,
		                PRING_PARAM,
		                DECAY_PARAM,
		                ATTACK_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Windy() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(UPPER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(WAVEFORM_PARAM, 0.f, 3.f, 0.f, "");
		configParam(RINGING_PARAM, 0.f, 3.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRATIO_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DECAY_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct WindyWidget : ModuleWidget {
	WindyWidget(Windy* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Windy.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<NKK2>(mm2px(Vec(47.542, 112.235)), module, Windy::BOOST_PARAM));
		addParam(createParamCentered<FourSwitch>(mm2px(Vec(35.852, 70.536)), module, Windy::RINGING_PARAM));
		addParam(createParamCentered<FourSwitch>(mm2px(Vec(8.5, 70.251)), module, Windy::WAVEFORM_PARAM));
		addParam(createParam<SlidePot>(mm2px(Vec(8.504, 23.435)), module, Windy::UPPER_PARAM));
		addParam(createParam<SlidePot>(mm2px(Vec(26.304, 23.5)), module, Windy::LOWER_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(9.317, 86.12)), module, Windy::PNORMAL_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(23.34, 86.12)), module, Windy::PRATIO_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(37.363, 86.12)), module, Windy::PRING_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(28.348, 109.441)), module, Windy::DECAY_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(11.785, 109.687)), module, Windy::ATTACK_PARAM));


		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45.705, 39.364)), module, Windy::OUTPUT_OUTPUT));

	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
