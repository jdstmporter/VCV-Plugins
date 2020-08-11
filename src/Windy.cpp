#include "plugin.hpp"
#include "MultiNoiseGenerator.hpp"
#include <vector>
#include <initializer_list>
#include "widgets/customWidgets.hpp"

#define BLOCK 64


struct Windy : Module {

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
		NEVERRING_LIGHT,
		ONCHANGERING_LIGHT,
		INTERNALRING_LIGHT,
		ALWAYSRING_LIGHT,
		ENVELOPELED_LIGHT,
		NUM_LIGHTS
	};



	unsigned offset;
	wind::ParameterSet parameters;
	wind::ParameterSet oldParameters;
	wind::MultiNoiseGenerator generator;
	float buffer[BLOCK];
	unsigned wave;
	MultiSwitch<4> *waveSwitch;
	MultiSwitch<4> *ringSwitch;
	OnOffSwitch *envelopeSwitch;
	LedDisplayTextField *led;

	Windy() : offset(0), parameters(), oldParameters(), generator(), wave(NUM_LIGHTS-1), led(nullptr)  {
		config(wind::NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(wind::BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::LOWER_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::UPPER_PARAM, 0.f, 1.f, 1.f, "");
		configParam(wind::WAVEFORM_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::RINGING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::PNORMAL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::PRING_PARAM, 0.f, 1.f, 0.f, "");
		configParam(wind::ATTACK_PARAM, -10.f, 10.f, 10.f, "");
		configParam(wind::DECAY_PARAM, -10.f, 10.f, -1.f, "");

		configParam(wind::WAVE_MULTI, 0.f, 3.f, 3.f, "");
		configParam(wind::RING_MULTI, 0.f, 3.f, 3.f, "");
		configParam(wind::BOOST_MULTI, 0.f, 1.f, 0.f, "");

		for(auto i=0;i<BLOCK;i++) buffer[i]=0;
		waveSwitch=new MultiSwitch<4>(this,SINELED_LIGHT,wind::WAVE_MULTI);
		ringSwitch=new MultiSwitch<4>(this,NEVERRING_LIGHT,wind::RING_MULTI);
		envelopeSwitch=new OnOffSwitch(this,ENVELOPELED_LIGHT,wind::BOOST_MULTI);
	}

	void process(const ProcessArgs& args) override {

			if(offset==BLOCK) {
				auto w = static_cast<wind::WaveForm>(waveSwitch->value());
				oldParameters=parameters;
				parameters=wind::ParameterSet(this,args.sampleRate,w,ringSwitch->value(),envelopeSwitch->value());
				generator.Render(buffer,BLOCK,parameters);
				offset=0;
				parameters.dump(oldParameters);

				if(parameters.changeRange(oldParameters)) {
					std::stringstream s;
					s << parameters.range;
					led->setText(s.str());
				}

			}
			float output=clamp(buffer[offset++],-5.0f,5.0f);
			outputs[OUTPUT_OUTPUT].setVoltage(output);
	}
};


struct WindyWidget : ModuleWidget {
	using Button = ActionSwitch<TL1105>;
/*
	template<typename T>
	ActionSwitch<T> *createControlButtonCentred<T>(math::Vec pos,engine::Module *module,int paramId,SwitchCallback *cb) {
		auto button=(ActionSwitch<T> *)createParamCentered<ActionSwitch<T>>(pos, module, paramId);
		button->callback=cb;
		addParam(button);
		return button;
	}
*/
	WindyWidget(Windy* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Windy.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		INFO("UPPER");
		addParam(createParam<BefacoSlidePot>(mm2px(Vec(6.388, 18.143)), module, wind::UPPER_PARAM));
		INFO("LOWER");
		addParam(createParam<BefacoSlidePot>(mm2px(Vec(23.129, 18.224)), module, wind::LOWER_PARAM));
		INFO("CHANGE");


		auto wbutton=(Button *)createParamCentered<Button>(mm2px(Vec(8.429, 66.269)), module, wind::WAVEFORM_PARAM);
		INFO("Creted wbutton");
		if(module!=nullptr) wbutton->callback=module->waveSwitch;
		INFO("Set wbutton callback");
		//wbutton->callback=module;
		INFO("Added");
		addParam(wbutton);

		INFO("RINGER");
		auto rbutton=(Button *)createParamCentered<Button>(mm2px(Vec(35.806, 66.306)), module, wind::RINGING_PARAM);
		if(module!=nullptr) rbutton->callback=module->ringSwitch;
		addParam(rbutton);

		INFO("BOOST");
		auto bbutton=(Button *)createParamCentered<Button>(mm2px(Vec(7.321, 89.578)), module, wind::BOOST_PARAM);
		if(module!=nullptr) bbutton->callback=module->envelopeSwitch;
		addParam(bbutton);


		auto led=createWidget<LedDisplayTextField>(mm2px(Vec(10,10)));
		led->box.size=mm2px(Vec(74.480, 6));
		led->multiline=false;
		led->color = SCHEME_WHITE;
		led->font = APP->window->loadFont(asset::system("res/fonts/DejaVuSans.ttf"));
		led->textOffset=mm2px(Vec(0.5,0));

		if(module!=nullptr) module->led=led;
		addChild(led);

		INFO("Normal");
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(21.531, 101.352)), module, wind::PNORMAL_PARAM));
		INFO("RING");
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(36.867, 101.664)), module, wind::PRING_PARAM));
		INFO("ATTACK");
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(20.101, 86.063)), module, wind::ATTACK_PARAM));
		INFO("DECAY");
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(36.706, 86.063)), module, wind::DECAY_PARAM));
		INFO("OUT");
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(47.027, 33.884)), module, Windy::OUTPUT_OUTPUT));

		INFO("LEDs");
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(16.914, 59.739)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(16.914, 64.092)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(16.914, 68.445)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(16.914, 72.798)), module, Windy::SAWTOOTHLED_LIGHT));

		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(44.292, 59.777)), module, Windy::NEVERRING_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(44.292, 64.13)), module, Windy::ONCHANGERING_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(44.292, 68.483)), module, Windy::INTERNALRING_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(44.292, 72.836)), module, Windy::ALWAYSRING_LIGHT));

		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(7.321, 83.249)), module, Windy::ENVELOPELED_LIGHT));
		INFO("BUILT");
	}
};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
