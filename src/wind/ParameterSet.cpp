//
//  ParameterSet.cpp
//  AUPinkNoise
//
//  Created by Julian Porter on 04/08/2016.
//
//

#include "ParameterSet.hpp"
#include <numeric>
#include <iostream>
#include "Envelope.hpp"
#include "Windy.hpp"


namespace wind {



ParameterSet::ParameterSet(rack::Module *au,const float rate,
		const WaveForm &waveform,const unsigned ring,const bool e) :
		envActive(e), ringMode(ring), mode(waveform), sampleRate(rate), volume(OUTPUT_VOLTAGE) {
    auto lo = au->params[LOWER_PARAM].getValue()*sampleRate/2.0;
    auto hi = au->params[UPPER_PARAM].getValue()*sampleRate/2.0;
    range = util::Range(lo,hi);
    
    auto edge = ringMode==1 || ringMode==3;
    auto body = ringMode==2 || ringMode==3;
    auto pNormal = pow(2,au->params[PNORMAL_PARAM].getValue())-1.f; // logarithmic
    auto pRing   = pow(2,au->params[PRING_PARAM].getValue())-1.f; // logarithmic


    
    
    //std::cerr << "pnormal = " << pNormal << " pring = " << pRing << std::endl;
    
    pEdge=(edge) ? pRing : pNormal;
    pBody=(body) ? pRing : pNormal;
    N = NOSCILLATORS; //(unsigned)au->Globals()->GetParameter(kParameterNID);
    
    envAttack=au->params[ATTACK_PARAM].getValue();
    envDecay=au->params[DECAY_PARAM].getValue();
    env = util::EnvelopeState(envAttack,envDecay,envActive);
    
};

bool ParameterSet::changeProbability(const ParameterSet &old) const {
	return pEdge != old.pEdge || pBody != old.pBody;
}
bool ParameterSet::changeRange(const ParameterSet &old) const {
	return range != old.range;
}

void ParameterSet::dump(const ParameterSet &old) const {
	if(range != old.range) INFO("RANGE (%f,%f) -> (%f,%f)",old.range.lo,old.range.hi,range.lo,range.hi);
	if(mode != old.mode) INFO("WAVE %d -> %d",static_cast<unsigned>(old.mode),static_cast<unsigned>(mode));
	if(ringMode != old.ringMode) INFO("RINGMODE %d -> %d",old.ringMode,ringMode);
	if(pEdge != old.pEdge) INFO("PEDGE %f -> %f",old.pEdge,pEdge);
	if(pBody != old.pBody) INFO("PBODY %f -> %f",old.pBody,pBody);
	if(envActive!=old.envActive) INFO("ENVACTIVE %d -> %d",envActive,old.envActive);
	if(envAttack != old.envAttack) INFO("ATTACK %f -> %f",old.envAttack,envAttack);
	if(envDecay != old.envDecay) INFO("DECAY %f -> %f",old.envDecay,envDecay);
}

}
