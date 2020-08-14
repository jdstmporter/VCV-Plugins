//
//  PointSource.cpp
//  AUPinkNoise
//
//  Created by Julian Porter on 23/06/2016.
//
//

#include "PointSource.hpp"









namespace wind {


PointSource::PointSource(const double sampleRate,const double frequency,const double phase_,const util::Envelope::iterator &it) : rate(sampleRate), freq(frequency), newDelta(TwoPi*freq/rate), delta(newDelta), phase(phase_), mode(WaveForm::SINE), sTheta(sin(phase)), cTheta(cos(phase)), sDelta(sin(delta)), cDelta(cos(delta)),  envelope(it), randomiser()  {
    
};



void PointSource::reset(const double frequency,double p) {
    newDelta=TwoPi*frequency/rate;
    randomiser.rebase(p);
};

void PointSource::change(const WaveForm & mode_) {
    mode = mode_;
    if(mode==WaveForm::SINE) {
        sDelta=sin(delta);
        cDelta=cos(delta);
        sTheta=sin(phase);
        cTheta=cos(phase);
    }
};


double PointSource::step() {
    if(randomiser.get() && delta!=newDelta) {
        delta=newDelta;
        envelope.reset();
        if(mode==WaveForm::SINE) {
            sDelta=sin(delta);
            cDelta=cos(delta);
        }
    }
    
    phase=phase+delta;
    if(phase>=TwoPi) phase-=TwoPi;
    
    double env=*envelope;
    if(mode==WaveForm::SINE) {
        auto c=cTheta*cDelta-sTheta*sDelta;
        auto s=cTheta*sDelta+sTheta*cDelta;
        cTheta=c;
        sTheta=s;
        return env*c;
    }
    else {
        double sawtooth=phase*InvPi-1.0;
        auto out= (mode==WaveForm::SAWTOOTH) ? sawtooth : ((mode==WaveForm::SQUARE) ? ((sawtooth>0.0) ? 1.0 : -1.0) : 2.0*fabs(sawtooth)-1.0);
        return env*out;
    }
};

}
