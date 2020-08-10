#include "plugin.hpp"
#include "customWidgets.hpp"
#include "MultiNoiseGenerator.hpp"
#include <vector>
#include <initializer_list>

#define BLOCK 64



struct Windy;

struct SwitchCallback {
	SwitchCallback() = default;
	virtual ~SwitchCallback() = default;

	virtual void step() = 0;
};

template <typename T, class = typename std::enable_if<std::is_base_of<rack::app::Switch,T>::value>::type>
struct ActionSwitch : T {
	SwitchCallback *callback;

	ActionSwitch()  : T(), callback(nullptr) {
		this->momentary=true;
	}
	virtual ~ActionSwitch() = default;

	void setCallback(rack::ParamWidget *cb) { callback = cb; }

	virtual void onChange(const rack::event::Change &e) override {
		rack::SvgSwitch::onChange(e);
		if(callback != nullptr && this->paramQuantity != nullptr) {
			if(this->paramQuantity->isMin()) callback->step();
		}
	}
};



struct Windy : Module, SwitchCallback {
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
	enum LightIds {
				SINELED_LIGHT,
				SQUARELED_LIGHT,
				TRIANGLELED_LIGHT,
				SAWTOOTHLED_LIGHT,
				NUM_LIGHTS
		};



	unsigned offset;
	wind::ParameterSet parameters;
	wind::ParameterSet oldParameters;
	wind::MultiNoiseGenerator generator;
	float buffer[BLOCK];
	unsigned wave;

	Windy() : offset(0), parameters(), oldParameters(), generator(), wave(NUM_LIGHTS-1)  {
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

	}

	void updateLights() {
		for(unsigned i=0;i<NUM_LIGHTS;i++) {
			auto b = (i==wave) ? 1.f : 0.f;
			lights[i].setBrightness(b);
		}
	}

	void resetLights() {
		wave=0;
		updateLights();
	}

	virtual void step() override {
		wave=(wave+1)%NUM_LIGHTS;
		updateLights();
		INFO("Current waveform is %d",wave);
	};


	void process(const ProcessArgs& args) override {

			if(offset==BLOCK) {
				auto w = static_cast<wind::WaveForm>(wave);
				oldParameters=parameters;
				parameters=wind::ParameterSet(this,args.sampleRate,w);
				generator.Render(buffer,BLOCK,parameters);
				offset=0;
				parameters.dump(oldParameters);
			}
			float output=clamp(buffer[offset++],-5.0f,5.0f);
			outputs[OUTPUT_OUTPUT].setVoltage(output);
	}
};


struct WindyWidget : ModuleWidget {
	using Button = ActionSwitch<BefacoPush>;


	WindyWidget(Windy* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Windy.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		INFO("BOOST");
		addParam(createParamCentered<OnOffSwitch2V>(mm2px(Vec(50.271, 109.687	)), module, Windy::BOOST_PARAM));
		INFO("RINGER");
		addParam(createParam<FourSwitch>(mm2px(Vec(34.316, 61.302)), module, Windy::RINGING_PARAM));
		INFO("UPPER");
		addParam(createParam<SlidePotV>(mm2px(Vec(8.504, 18.143)), module, Windy::UPPER_PARAM));
		INFO("LOWER");
		addParam(createParam<SlidePotV>(mm2px(Vec(26.304, 18.224)), module, Windy::LOWER_PARAM));
		INFO("CHANGE");


		auto wbutton=(Button *)createParamCentered<Button>(mm2px(Vec(8.429, 66.269)), module, Windy::CHANGEWAVEFORM_PARAM);
		wbutton->callback=module;
		addParam(wbutton);


		INFO("Normal");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.317, 86.12)), module, Windy::PNORMAL_PARAM));
		INFO("RING");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(29.426, 86.12)), module, Windy::PRING_PARAM));
		INFO("ATTACK");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.785, 109.687)), module, Windy::ATTACK_PARAM));
		INFO("DECAY");
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(28.39, 109.687)), module, Windy::DECAY_PARAM));
		INFO("OUT");
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45.705, 39.364)), module, Windy::OUTPUT_OUTPUT));

		INFO("LEDs");
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 59.739)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 64.092)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 68.445)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(16.914, 72.798)), module, Windy::SAWTOOTHLED_LIGHT));
		INFO("BUILT");
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
