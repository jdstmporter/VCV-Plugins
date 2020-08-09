#include "plugin.hpp"
#include "customWidgets.hpp"
#include "MultiNoiseGenerator.hpp"
#include <vector>
#include <initializer_list>

#define BLOCK 64

enum LightIds {
			SINELED_LIGHT,
			SQUARELED_LIGHT,
			TRIANGLELED_LIGHT,
			SAWTOOTHLED_LIGHT,
			NUM_LIGHTS
	};

struct WaveformButton : public BefacoActionButton {
	rack::Module *module;
	unsigned currentWaveform;
	bool ready;

	static constexpr unsigned lightCodes[NUM_LIGHTS] = {SINELED_LIGHT,SQUARELED_LIGHT,TRIANGLELED_LIGHT,SAWTOOTHLED_LIGHT};

	WaveformButton(rack::Module *m) : BefacoActionButton() , module(m), currentWaveform(0), ready(true) {};
	virtual ~WaveformButton() = default;

	wind::WaveForm waveform() const { return static_cast<wind::WaveForm>(currentWaveform); }

	virtual void didFire(const rack::event::Change &e) {
		bool value = this->paramQuantity->getValue()>0;
		if(!value) ready=true;
		else if(ready) {
			ready=false;
			currentWaveform=(currentWaveform+1) % NUM_LIGHTS;
			updateWaveFormDisplay();
		}
	}

		void updateWaveFormDisplay() {
			for(unsigned i=0;i<NUM_LIGHTS;i++) {
				auto idx = lightCodes[i];
				auto b = idx==lightCodes[currentWaveform] ? 1.f : 0.f;
				module->lights[idx].setBrightness(b);
			}
		}

		void resetWaveForm() {
			currentWaveform=0;
			updateWaveFormDisplay();
		}


};

struct Windy : Module {
	enum ParamIds {
		BOOST_PARAM,
		RINGING_PARAM,
		UPPER_PARAM,
		LOWER_PARAM,
		CHANGEWAVEFORM_PARAM,
		PNORMAL_PARAM,
		PRING_PARAM,
		ATTACK_PARAM,
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



	unsigned offset;
	wind::ParameterSet parameters;
	wind::MultiNoiseGenerator generator;
	float buffer[BLOCK];
	WaveformButton *wbutton;

	Windy() : offset(0), parameters(), generator()  {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(UPPER_PARAM, 0.f, 1.f, 1.f, "");
		configParam(CHANGEWAVEFORM_PARAM, 0.f, 3.f, 0.f, "");
		configParam(RINGING_PARAM, 0.f, 3.f, 0.f, "");
		configParam(PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PRING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ATTACK_PARAM, -10.f, 10.f, 10.f, "");
		configParam(DECAY_PARAM, -10.f, 10.f, -1.f, "");

		for(auto i=0;i<BLOCK;i++) buffer[i]=0;
		wbutton = new WaveformButton(this);
	}
	virtual ~Windy() {
		delete wbutton;
	}


	void process(const ProcessArgs& args) override {

			if(offset==BLOCK) {
				if(wbutton) {
					parameters=wind::ParameterSet(this,args.sampleRate,wbutton->waveform());
				}
				else {
					parameters=wind::ParameterSet(this,args.sampleRate,wind::WaveForm::SINE);
				}
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

		INFO("BOOST");
		addParam(createParam<OnOffSwitch2V>(mm2px(Vec(-52.455, -113.376)), module, Windy::BOOST_PARAM));
		INFO("RINGER");
		addParam(createParam<FourSwitch>(mm2px(Vec(-38.852, -77.536)), module, Windy::RINGING_PARAM));
		INFO("UPPER");
		addParam(createParam<SlidePotV>(mm2px(Vec(8.504, 23.435)), module, Windy::UPPER_PARAM));
		INFO("LOWER");
		addParam(createParam<SlidePotV>(mm2px(Vec(26.304, 23.5)), module, Windy::LOWER_PARAM));
		INFO("CHANGE");

		module->wbutton->box.pos=mm2px(Vec(8.429, 66.269));
		module->wbutton->paramQuantity=module->paramQuantities[Windy::CHANGEWAVEFORM_PARAM];
		module->wbutton->box.pos = module->wbutton->box.pos.minus(module->wbutton->box.size.div(2));

		//addParam(createParamCentered<BefacoActionButton>(mm2px(Vec(8.429, 66.269)), module, Windy::CHANGEWAVEFORM_PARAM));
		INFO("Normal");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.317, 86.12)), module, Windy::PNORMAL_PARAM));
		INFO("RING");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(37.363, 86.12)), module, Windy::PRING_PARAM));
		INFO("ATTACK");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.785, 109.687)), module, Windy::ATTACK_PARAM));
		INFO("DECAY");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(33.682, 109.687)), module, Windy::DECAY_PARAM));
		INFO("OUT");
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45.705, 39.364)), module, Windy::OUTPUT_OUTPUT));

		INFO("LEDs");
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 59.739)), module, SINELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 64.092)), module, SQUARELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 68.445)), module, TRIANGLELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 72.798)), module, SAWTOOTHLED_LIGHT));
		INFO("BUILT");
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
