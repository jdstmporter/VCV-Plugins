#include "plugin.hpp"
#include "hilbertXform.hpp"

#define BLOCKSIZE 64
#define NSTAGES 64

#define limit(x) clamp(x,-5.0f,5.0f)

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

	hilbert::HilbertTransform hilbert;
	float * buffer;
	float * outX;
	float * outY;
	unsigned offset;

	Hilbert() : hilbert(NSTAGES,BLOCKSIZE), offset(0) {
		buffer = new float[BLOCKSIZE];
		outX = new float[BLOCKSIZE];
		outY = new float[BLOCKSIZE];
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	virtual ~Hilbert() {
		delete [] buffer;
		delete [] outX;
		delete [] outY;
	}

	void process(const ProcessArgs& args) override {
		float input = inputs[Input].getVoltage();
		input = clamp(input,-5.0f,5.0f);
		buffer[offset++]=input;
		if(offset==BLOCKSIZE) {
			hilbert.process(buffer,outX,outY);
			offset=0;
		}
		float oReal = clamp(outX[offset],-10.0f,10.0f);
		float oImag = clamp(outY[offset],-10.0f,10.0f);
		float oModulus = hypot(oReal,oImag);
		float oPhase = (oReal == 0 && oImag == 0) ? 0 : atan2(oImag,oReal);

		outputs[Real].setVoltage(oReal);
		outputs[Imaginary].setVoltage(oImag);
		outputs[Modulus].setVoltage(clamp(oModulus,-10.0f,10.0f));
		outputs[Phase].setVoltage(clamp(oPhase,-5.0f,5.0f));
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
