#include "plugin.hpp"


struct Windy : Module {
	enum ParamIds {
		UPPER_PARAM,
		LOWER_PARAM,
		CHANGERINGING_PARAM,
		CHANGEWAVEFORM_PARAM,
		PNORMAL_PARAM,
		ATTACK_PARAM,
		BOOST_PARAM,
		PRING_PARAM,
		DECAY_PARAM,
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
		SINELED_LIGHT,
		NEVERRING_LIGHT,
		SQUARELED_LIGHT,
		ONCHANGERING_LIGHT,
		TRIANGLELED_LIGHT,
		INTERNALRING_LIGHT,
		SAWTOOTHLED_LIGHT,
		ALWAYSRING_LIGHT,
		ENVELOPELED_LIGHT,
		NUM_LIGHTS
	};

	Windy() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(UPPER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(CHANGERINGING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(CHANGEWAVEFORM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
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

		addParam(createParam<RoundBlackKnob>(mm2px(Vec(9.061, 12.851)), module, Windy::UPPER_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(45.899, 12.933)), module, Windy::LOWER_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(35.806, 59.956)), module, Windy::CHANGERINGING_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.429, 60.17)), module, Windy::CHANGEWAVEFORM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(46.855, 77.067)), module, Windy::PNORMAL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(21.423, 77.294)), module, Windy::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.429, 86.774)), module, Windy::BOOST_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(46.855, 92.971)), module, Windy::PRING_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(21.423, 93.169)), module, Windy::DECAY_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.48, 114.771)), module, Windy::OUTPUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 53.389)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 53.427)), module, Windy::NEVERRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 57.742)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 57.78)), module, Windy::ONCHANGERING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 62.095)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 62.133)), module, Windy::INTERNALRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 66.448)), module, Windy::SAWTOOTHLED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 66.486)), module, Windy::ALWAYSRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(8.429, 80.445)), module, Windy::ENVELOPELED_LIGHT));
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");