//
//  ParameterSet.hpp
//  AUPinkNoise
//
//  Created by Julian Porter on 04/08/2016.
//
//

#ifndef ParameterSet_hpp
#define ParameterSet_hpp

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include "PointSource.hpp"
//#include "Randomiser.hpp"
#include <rack.hpp>


namespace wind {
    
    enum  {
        kParameterLowerID=0,
        kParameterUpperID=1,
        kParameterOnID=2,
        kParameterVolumeID=3,
        kParameterModeID=4,
        kParameterRingEdgeID=5,
        kParameterRingBodyID=6,
        kParameterPNormalID=7,
        kParameterPFactorID=8,
        kParameterEnvelopeID=9,
        kParameterEnvelopeAttackID=10,
        kParameterEnvelopeDecayID=11,
        kParameterNParameters=12
    };
    
    inline unsigned Wave2Int(const WaveForm &w) {
        return static_cast<unsigned>(w);
    }
    
    inline WaveForm Int2Wave(const unsigned i) {
        return static_cast<WaveForm>(i);
    }
    
    
    struct ParameterSet {
    public:
        double volume;
        util::Range range;
        WaveForm mode;
        double pEdge;
        double pBody;
        unsigned N;
        util::EnvelopeState env;
        
        ParameterSet() : volume(sqrt(2)), range(0,44100), mode(WaveForm::SINE), pEdge(2.0e-3), pBody(1.0e-5), N(100), env() {};
        ParameterSet(rack::Module *au);
        virtual ~ParameterSet() = default;
        
    };
}

#endif /* ParameterSet_hpp */
