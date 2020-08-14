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
		BOOST_PARAM,
		RINGING_PARAM,
		UPPER_PARAM,
		LOWER_PARAM,
		WAVEFORM_PARAM,
		PNORMAL_PARAM,
		PRING_PARAM,
		ATTACK_PARAM,
		DECAY_PARAM,
		WAVE_MULTI,
		RING_MULTI,
		BOOST_MULTI,
		NUM_PARAMS
	};
    
    inline unsigned Wave2Int(const WaveForm &w) {
        return static_cast<unsigned>(w);
    }
    
    inline WaveForm Int2Wave(const unsigned i) {
        return static_cast<WaveForm>(i);
    }
    
    
struct ParameterSet {
protected:
		bool envActive;
	    double envAttack;
	    double envDecay;
	    unsigned ringMode;
    public:
        util::Range range;
        WaveForm mode;
        double pEdge;
        double pBody;
        unsigned N;
        util::EnvelopeState env;
        float sampleRate;
        float volume;
        

        ParameterSet() : envActive(false), envAttack(0),envDecay(0), ringMode(0),
        		range(0,44100), mode(WaveForm::SINE), pEdge(2.0e-3), pBody(1.0e-5), N(100), env(),
        		sampleRate(44100), volume(5)  {};
        ParameterSet(rack::Module *au,const float rate,const WaveForm &,const unsigned, const bool);
        virtual ~ParameterSet() = default;
        
        void dump(const ParameterSet &) const;
        bool changeProbability(const ParameterSet &old) const;
        bool changeRange(const ParameterSet &old) const;
    };
}

#endif /* ParameterSet_hpp */
