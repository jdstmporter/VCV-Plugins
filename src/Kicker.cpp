#include "plugin.hpp"
#include <cmath>

struct Kicker : Module {
	enum ParamIds {
		LEARNINGRATE_PARAM,
		RECTIFY_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		INPUT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUTPUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	enum class Mode {
		HalfWave = 2,
		FullWave = 1,
		Square = 0
	};


		float output;
		Mode mode;

	Kicker() : output(0.0), mode(Mode::HalfWave) {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(LEARNINGRATE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RECTIFY_PARAM,0.f,2.f,0.f, "");
	}





	float rectify(const float value) const {
		switch(mode) {
		case Mode::HalfWave:
		default:
			return std::max(value,0.f);
			break;
		case Mode::FullWave:
			return fabs(value);
			break;
		case Mode::Square:
			return value*value;
		}
	}

	void process(const ProcessArgs& args) override {
		auto input=clamp(inputs[INPUT_INPUT].getVoltage(),-5.f,5.f);
		auto rho=params[LEARNINGRATE_PARAM].getValue();
		mode = static_cast<Mode>((unsigned)params[RECTIFY_PARAM].getValue());

		auto rectified = rectify(input);
		auto filtered = rho*rectified + (1.f-rho)*output;
		output=clamp(filtered,-5.f,5.f);

		outputs[OUTPUT_OUTPUT].setVoltage(output);
	}


};


struct KickerWidget : ModuleWidget {
	KickerWidget(Kicker* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Kicker.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.62, 35.908)), module, Kicker::LEARNINGRATE_PARAM));
		addParam(createParamCentered<NKK>(mm2px(Vec(4.7813988, 56.923214)), module, Kicker::RECTIFY_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 22.112)), module, Kicker::INPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.053, 74.008)), module, Kicker::OUTPUT_OUTPUT));
	}
};


Model* modelKicker = createModel<Kicker, KickerWidget>("Kicker");
