#include "plugin.hpp"


struct Windy : Module {
	enum ParamIds {
		UPPER_PARAM,
		LOWER_PARAM,
		CHANGERINGING_PARAM,
		CHANGEWAVEFORM_PARAM,
		ATTACK_PARAM,
		DECAY_PARAM,
		BOOST_PARAM,
		PNORMAL_PARAM,
		PRING_PARAM,
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
		configParam(ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DECAY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
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

		addParam(createParam<RoundBlackKnob>(mm2px(Vec(6.388, 18.143)), module, Windy::UPPER_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(23.129, 18.224)), module, Windy::LOWER_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(35.806, 66.306)), module, Windy::CHANGERINGING_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.429, 66.52)), module, Windy::CHANGEWAVEFORM_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.101, 86.063)), module, Windy::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(36.706, 86.063)), module, Windy::DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.321, 89.578)), module, Windy::BOOST_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(21.531, 101.352)), module, Windy::PNORMAL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(36.867, 101.664)), module, Windy::PRING_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(47.027, 33.884)), module, Windy::OUTPUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 59.739)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 59.777)), module, Windy::NEVERRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 64.092)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 64.13)), module, Windy::ONCHANGERING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 68.445)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 68.483)), module, Windy::INTERNALRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 72.798)), module, Windy::SAWTOOTHLED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(44.292, 72.836)), module, Windy::ALWAYSRING_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(7.321, 83.249)), module, Windy::ENVELOPELED_LIGHT));
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");