#include "plugin.hpp"

struct FourSwitch : app::SvgSwitch {
	FourSwitch() {
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_1.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_2.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance,"res/CKSSFour_3.svg")));
	}
};

struct Windy : Module {
	enum ParamIds {
		BOOST_PARAM,
		LOWERDIAL_PARAM,
		UPPERDIAL_PARAM,
		WAVEFORM_PARAM,
		RINGINGMODE_PARAM,
		NORMAL_PARAM,
		RINGING_PARAM,
		RATIO_PARAM,
		ATTACK_PARAM,
		DECAY_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		LOWERCV_INPUT,
		UPPERCV_INPUT,
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
		configParam(LOWERDIAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(UPPERDIAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(WAVEFORM_PARAM, 0.f, 3.f, 0.f, "");
		configParam(RINGINGMODE_PARAM, 0.f, 3.f, 0.f, "");
		configParam(NORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RINGING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RATIO_PARAM, 0.f, 1.f, 0.f, "");
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


		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(21.13, 28.891)), module, Windy::LOWERDIAL_PARAM));
		addParam(createParamCentered<RoundLargeBlackKnob>(mm2px(Vec(21.13, 46.467)), module, Windy::UPPERDIAL_PARAM));
		addParam(createParamCentered<CKSS>(mm2px(Vec(12, 60)), module, Windy::BOOST_PARAM));
		addParam(createParamCentered<FourSwitch>(mm2px(Vec(11.15, 75.461)), module, Windy::WAVEFORM_PARAM));
		addParam(createParamCentered<FourSwitch>(mm2px(Vec(30.805, 76.595)), module, Windy::RINGINGMODE_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(38.063, 90.391)), module, Windy::NORMAL_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(10.016, 90.58)), module, Windy::RINGING_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(23.246, 90.58)), module, Windy::RATIO_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(21.734, 103.242)), module, Windy::ATTACK_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(9.071, 104.187)), module, Windy::DECAY_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(37.836, 28.891)), module, Windy::LOWERCV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(37.836, 46.467)), module, Windy::UPPERCV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(37.767, 61.098)), module, Windy::OUTPUT_OUTPUT));
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
