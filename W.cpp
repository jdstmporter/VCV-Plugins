#include "plugin.hpp"


struct Windy : Module {
	enum ParamIds {
		UPPER_PARAM,
		LOWER_PARAM,
		CHANGEWAVEFORM_PARAM,
		RINGING_PARAM,
		PNORMAL_PARAM,
		PRING_PARAM,
		ATTACK_PARAM,
		DECAY_PARAM,
		BOOST_PARAM,
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
		SQUARELED_LIGHT,
		TRIANGLELED_LIGHT,
		SAWTOOTHLED_LIGHT,
		NUM_LIGHTS
	};

	Windy() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(UPPER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(CHANGEWAVEFORM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RINGING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ATTACK_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DECAY_PARAM, 0.f, 1.f, 0.f, "");
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
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

		addParam(createParam<RoundBlackKnob>(mm2px(Vec(8.504, 18.143)), module, Windy::UPPER_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(26.304, 18.224)), module, Windy::LOWER_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.429, 66.269)), module, Windy::CHANGEWAVEFORM_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(34.316, 61.302)), module, Windy::RINGING_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.317, 86.12)), module, Windy::PNORMAL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(29.426, 86.12)), module, Windy::PRING_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.785, 109.687)), module, Windy::ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(28.39, 109.687)), module, Windy::DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(50.271, 109.687)), module, Windy::BOOST_PARAM));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45.705, 39.364)), module, Windy::OUTPUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 59.739)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 64.092)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 68.445)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 72.798)), module, Windy::SAWTOOTHLED_LIGHT));
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");