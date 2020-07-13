#include "plugin.hpp"


struct Hilbert : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		Input,
		NUM_INPUTS
	};
	enum OutputIds {
		Real,
		Imaginary,
		Modulus,
		Phase,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Hilbert() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs& args) override {
	}
};


struct HilbertWidget : ModuleWidget {
	HilbertWidget(Hilbert* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Hilbert.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.62, 22.112)), module, Hilbert::Input));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 40.821)), module, Hilbert::Real));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 55.751)), module, Hilbert::Imaginary));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 76.729)), module, Hilbert::Modulus));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.62, 92.415)), module, Hilbert::Phase));
	}
};


Model* modelHilbert = createModel<Hilbert, HilbertWidget>("Hilbert");
