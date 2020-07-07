#include "plugin.hpp"
#include "Haar.hpp"
#include "Log.hpp"
#include <iostream>

struct Wavelet : Module {
	enum ParamIds {
		LEVEL1_PARAM,
		LEVEL2_PARAM,
		LEVEL3_PARAM,
		LEVEL4_PARAM,
		APPROX_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float buffer[16];
	float outs[16];
	float thresholds[5];
	haar::Haar wavelet;
	unsigned offset=0;

	Wavelet() : wavelet(4) {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(LEVEL1_PARAM, 0.f, 1.f, 0.f, "detail1");
		configParam(LEVEL2_PARAM, 0.f, 1.f, 0.f, "detail2");
		configParam(LEVEL3_PARAM, 0.f, 1.f, 0.f, "detail3");
		configParam(LEVEL4_PARAM, 0.f, 1.f, 0.f, "detail4");
		configParam(APPROX_PARAM, 0.f, 1.f, 0.f, "approximation");
		log() << "Configured parameters for Wavelet" << std::endl;

		for(auto i=0;i<16;i++) outs[i]=0;

		log() << "Configured arrays for Wavelet" << std::endl;
	}

	void process(const ProcessArgs& args) override {

			for(auto i=0;i<5;i++) thresholds[i]=params[i].getValue();
			auto t=clamp(thresholds[0]+thresholds[4],-10.0f,10.0f)/10.0f;
			float input = inputs[IN_INPUT].getVoltage();
			input = clamp(input,-5.0f,5.0f);


			log() << "Processing " << offset << " at " << args.sampleTime << std::endl;
			buffer[offset++]=input;
			if(offset==16) {
				wavelet.process(buffer,outs,thresholds);
				offset=0;
			}
			float output = clamp(outs[offset],-5.0f,5.0f);
			outputs[OUT_OUTPUT].setVoltage(output);

	}
};


struct WaveletWidget : ModuleWidget {
	WaveletWidget(Wavelet* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Wavelet.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.704, 32.317)), module, Wavelet::LEVEL1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.776, 32.317)), module, Wavelet::LEVEL2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.704, 48.523)), module, Wavelet::LEVEL3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.776, 48.523)), module, Wavelet::LEVEL4_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 78.808)), module, Wavelet::APPROX_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.52, 104.699)), module, Wavelet::IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.96, 104.699)), module, Wavelet::OUT_OUTPUT));
	}
};


Model* modelWavelet = createModel<Wavelet, WaveletWidget>("Wavelet");
