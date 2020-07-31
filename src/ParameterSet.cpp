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



namespace wind {

#define NOSCILLATORS 100

ParameterSet::ParameterSet(rack::Module *au) {
    volume = 1.0;
    auto lo= au->params[kLOWER_PARAM].getValue();
    auto hi= au->params[kUPPER_PARAM].getValue();
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
