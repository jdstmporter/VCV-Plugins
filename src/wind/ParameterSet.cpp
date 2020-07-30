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

#define L 4
#define U 3

ParameterSet::ParameterSet(rack::Module *au) {
    volume = 1.0;
    auto lo= au->params[L].getValue();
    auto hi= au->params[U].getValue();
    range = util::Range(lo,hi);
    mode = static_cast<WaveForm>((unsigned)au->Globals()->GetParameter(kParameterModeID));
    
    auto edge = au->Globals()->GetParameter(kParameterRingEdgeID)!=0.0;
    auto body = au->Globals()->GetParameter(kParameterRingBodyID)!=0.0;
    auto pNormal = au->Globals()->GetParameter(kParameterPNormalID);
    auto pRing   = au->Globals()->GetParameter(kParameterPFactorID);
    
    
    //std::cerr << "pnormal = " << pNormal << " pring = " << pRing << std::endl;
    
    pEdge=(edge) ? pRing : pNormal;
    pBody=(body) ? pRing : pNormal;
    N = 100; //(unsigned)au->Globals()->GetParameter(kParameterNID);
    
    auto envActive=au->Globals()->GetParameter(kParameterEnvelopeID)!=0.0;
    auto envAttack=au->Globals()->GetParameter(kParameterEnvelopeAttackID);
    auto envDecay=au->Globals()->GetParameter(kParameterEnvelopeDecayID);
    env     = EnvelopeState(envAttack,envDecay,envActive);
    
};

}
