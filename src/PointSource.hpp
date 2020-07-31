//
//  PointSource.hpp
//  AUPinkNoise
//
//  Created by Julian Porter on 23/06/2016.
//
//

#ifndef PointSource_hpp
#define PointSource_hpp
#include <vector>
#include <iostream>
#include <iterator>


#include "Randomiser.hpp"
#include "Envelope.hpp"

namespace wind {
    
        
            enum class WaveForm : unsigned {
                SINE=3,
                SQUARE=2,
                TRIANGLE=1,
                SAWTOOTH=0
                };
        
             
            class PointSource {
            protected:
                
                double rate = 0;
                double freq = 0;
                double newDelta = 0, delta = 0;
                double phase = 0;
                
                WaveForm mode = WaveForm::SINE;
                
                double sTheta = 0,cTheta = 0;
                double sDelta = 0,cDelta = 0;
                
                util::Envelope::iterator envelope;;
                util::RandomBoolean randomiser;
                
                
                
            public:
                constexpr static const double TwoPi=2.0*M_PI;
                constexpr static const double Pi=M_PI;
                constexpr static const double InvPi=1.0/M_PI;
                
                PointSource() = default;
                PointSource(const double sampleRate,const double frequency,const double phase_,const util::Envelope::iterator &it);
                virtual ~PointSource() = default;
                
                double step();
                void reset(const double frequency,double p);
                void change(const WaveForm & mode_);
                double frequency() { return freq; };
                
            };
            
        }




#endif /* PointSource_hpp */
