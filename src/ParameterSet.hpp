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
    
enum ParameterIds {
		kBOOST_PARAM,
		kRINGING_PARAM,
		kWAVEFORM_PARAM,
		kUPPER_PARAM,
		kLOWER_PARAM,
		kPNORMAL_PARAM,
		kPRING_PARAM,
		kDECAY_PARAM,
		kATTACK_PARAM,
		kNUM_PARAMS
	};
    
    inline unsigned Wave2Int(const WaveForm &w) {
        return static_cast<unsigned>(w);
    }
    
    inline WaveForm Int2Wave(const unsigned i) {
        return static_cast<WaveForm>(i);
    }
    
    
    struct ParameterSet {
    public:
        util::Range range;
        WaveForm mode;
        double pEdge;
        double pBody;
        unsigned N;
        util::EnvelopeState env;
        double sampleRate;
        double volume;
        
        ParameterSet() : range(0,44100), mode(WaveForm::SINE), pEdge(2.0e-3), pBody(1.0e-5), N(100), env(), sampleRate(44100), volume(5) {};
        ParameterSet(rack::Module *au,const double rate);
        virtual ~ParameterSet() = default;
        
    };
}

#endif /* ParameterSet_hpp */