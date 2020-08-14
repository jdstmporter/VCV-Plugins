#include "plugin.hpp"
#include "wind/MultiNoiseGenerator.hpp"
#include <vector>
#include <initializer_list>
#include "widgets/customWidgets.hpp"

#define BLOCK 64
struct RateSet {
	int N;
	std::vector<TextDisplayField *> labels;

	RateSet(const Rect &box,const int n=0) : N(n), labels() {
		float frac = box.size.y /(N+1);
		auto left=box.pos.x;
		auto bottom=box.getBottom();
		auto size = Vec(box.size.x,frac);
		for(auto i=0;i<=N;i++) {
			auto origin=Vec(left,bottom-(i+1)*frac);
			auto label = createWidget<TextDisplayField>(origin);
			label->box=Rect(origin,size);
			label->setFG(0,0,0,1);
			label->centre=true;
			label->loadFont(asset::system("res/fonts/DejaVuSans.ttf"));
			label->setFontSize(6);
			label->setText("-");
			labels.push_back(label);
		}
	}
	virtual ~RateSet() = default;

	void add(ModuleWidget *ui) {
		for(auto it=labels.begin();it!=labels.end();it++) {
			ui->addChild(*it);
		}
	}
	void setRate(const float rate) {
		float fn=rate/(2.f*N);
		for(auto i=0;i<=N;i++) labels[i]->setText(asHz(i*fn,2));
	}


};

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
	RateSet *r;
	bool labelled;

	Windy() : offset(0), parameters(), oldParameters(), generator(), wave(NUM_LIGHTS-1), r(nullptr), labelled(false)  {
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
				if(!labelled && r !=  nullptr) {
					INFO("Labelling for %f",args.sampleRate);
					r->setRate(args.sampleRate);
					labelled=true;
				}
				parameters=wind::ParameterSet(this,args.sampleRate,w,ringSwitch->value(),envelopeSwitch->value());
				generator.Render(buffer,BLOCK,parameters);
				offset=0;
				parameters.dump(oldParameters);

				if(parameters.changeRange(oldParameters)) {
					std::stringstream s;
					s << parameters.range;


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
		auto upper=createParam<SlidePotV>(mm2px(Vec(9.061, 12.851)), module, wind::UPPER_PARAM);
		addParam(upper);
		INFO("LOWER");
		auto lower=createParam<SlidePotV>(mm2px(Vec(45.899, 12.851)), module, wind::LOWER_PARAM);
		addParam(lower);
		INFO("CHANGE");


		auto wbutton=(Button *)createParamCentered<Button>(mm2px(Vec(6.312, 60.17)), module, wind::WAVEFORM_PARAM);
		INFO("Creted wbutton");
		if(module!=nullptr) wbutton->callback=module->waveSwitch;
		INFO("Set wbutton callback");
		//wbutton->callback=module;
		INFO("Added");
		addParam(wbutton);

		INFO("RINGER");
		auto rbutton=(Button *)createParamCentered<Button>(mm2px(Vec(36.335, 59.956)), module, wind::RINGING_PARAM);
		if(module!=nullptr) rbutton->callback=module->ringSwitch;
		addParam(rbutton);

		INFO("BOOST");
		auto bbutton=(Button *)createParamCentered<Button>(mm2px(Vec(24.682, 87.908)), module, wind::BOOST_PARAM);
		if(module!=nullptr) bbutton->callback=module->envelopeSwitch;
		addParam(bbutton);




/*
		auto led=createWidget<LedDisplayTextField>(mm2px(Vec(10,10)));
		led->box.size=mm2px(Vec(74.480, 6));
		led->multiline=false;
		led->color = SCHEME_WHITE;
		led->font = APP->window->loadFont(asset::system("res/fonts/DejaVuSans.ttf"));
		led->textOffset=mm2px(Vec(0.5,0));

		if(module!=nullptr) module->led=led;
		addChild(led);
*/
		INFO("Making gap");
		auto origin = upper->box.getTopRight();
		auto height = upper->box.size.y;
		auto width = lower->box.pos.x - origin.x;

		auto rect = Rect(origin,Vec(width,height));
		INFO("Made sizes %f %f %f %f",rect.pos.x,rect.pos.y,rect.size.x,rect.size.y);
		auto r = new RateSet(rect,4);
		INFO("Made label set");
		r->add(this);
		INFO("Added to UI");
		if(module!=nullptr) module->r=r;
		INFO("Connected to module");

		INFO("Normal");
		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(48.707, 77.105)), module, wind::PNORMAL_PARAM));
		INFO("RING");
		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(48.556, 92.035)), module, wind::PRING_PARAM));
		INFO("ATTACK");
		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(11.407, 76.727)), module, wind::ATTACK_PARAM));
		INFO("DECAY");
		addParam(createParamCentered<BefacoTinyKnob>(mm2px(Vec(11.407, 92.602)), module, wind::DECAY_PARAM));
		INFO("OUT");
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.48, 114.771)), module, Windy::OUTPUT_OUTPUT));

		INFO("LEDs");
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(14.798, 53.389)), module, Windy::SINELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(14.798, 57.742)), module, Windy::SQUARELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(14.798, 62.095)), module, Windy::TRIANGLELED_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(14.798, 66.448)), module, Windy::SAWTOOTHLED_LIGHT));

		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(44.821, 53.427)), module, Windy::NEVERRING_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(44.821, 57.78)), module, Windy::ONCHANGERING_LIGHT));
		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(44.821, 62.133)), module, Windy::INTERNALRING_LIGHT));
		addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(44.821, 66.486)), module, Windy::ALWAYSRING_LIGHT));

		addChild(createLightCentered<SmallLight<YellowLight>>(mm2px(Vec(24.682, 82.638)), module, Windy::ENVELOPELED_LIGHT));
		INFO("BUILT");

	}


};


Model* modelWindy = createModel<Windy, WindyWidget>("Windy");
