#include "plugin.hpp"
#include "customWidgets.hpp"
#include "MultiNoiseGenerator.hpp"

#define BLOCK 64

struct Windy : Module {
	enum ParamIds {
		BOOST_PARAM,
		                RINGING_PARAM,
		                WAVEFORM_PARAM,
		                UPPER_PARAM,
		                LOWER_PARAM,
		                PNORMAL_PARAM,
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

	unsigned offset;
	wind::ParameterSet parameters;
	wind::MultiNoiseGenerator generator;
	float buffer[BLOCK];

	Windy() : offset(0), parameters(), generator() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(UPPER_PARAM, 0.f, 1.f, 1.f, "");
		configParam(WAVEFORM_PARAM, 0.f, 3.f, 3.f, "");
		configParam(RINGING_PARAM, 0.f, 3.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ATTACK_PARAM, -10.f, 10.f, 10.f, "");
		configParam(DECAY_PARAM, -10.f, 10.f, -1.f, "");

		for(auto i=0;i<BLOCK;i++) buffer[i]=0;
	}

	void process(const ProcessArgs& args) override {
			if(offset==BLOCK) {
				parameters=wind::ParameterSet(this,args.sampleRate);
				generator.Render(buffer,BLOCK,parameters);
				offset=0;
			}
			float output=clamp(buffer[offset++],-5.0f,5.0f);
			outputs[OUTPUT_OUTPUT].setVoltage(output);
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
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(37.363, 86.12)), module, Windy::PRING_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(28.348, 109.441)), module, Windy::DECAY_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(11.785, 109.687)), module, Windy::ATTACK_PARAM));


		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45.705, 39.364)), module, Windy::OUTPUT_OUTPUT));

	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
