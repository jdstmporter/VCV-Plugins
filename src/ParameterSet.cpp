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



ParameterSet::ParameterSet(rack::Module *au,const double rate) : sampleRate(rate), volume(OUTPUT_VOLTAGE) {
    auto lo= au->params[kLOWER_PARAM].getValue()*sampleRate/2.0;
    auto hi= au->params[kUPPER_PARAM].getValue()*sampleRate/2.0;
    range = util::Range(lo,hi);
    mode = static_cast<WaveForm>((unsigned)au->params[kWAVEFORM_PARAM].getValue());
    
    auto ringMode = (unsigned)au->params[kRINGING_PARAM].getValue();
    auto edge = ringMode==1 || ringMode==3;
    auto body = ringMode==2 || ringMode==3;
    auto pNormal = au->params[kPNORMAL_PARAM].getValue();
    auto pRing   = au->params[kPRING_PARAM].getValue();


    
    
    //std::cerr << "pnormal = " << pNormal << " pring = " << pRing << std::endl;
    
    pEdge=(edge) ? pRing : pNormal;
    pBody=(body) ? pRing : pNormal;
    N = NOSCILLATORS; //(unsigned)au->Globals()->GetParameter(kParameterNID);
    
    auto envActive=au->params[kBOOST_PARAM].getValue() != 0;
    auto envAttack=au->params[kATTACK_PARAM].getValue();
    auto envDecay=au->params[kDECAY_PARAM].getValue();
    env = util::EnvelopeState(envAttack,envDecay,envActive);
    
};

}
