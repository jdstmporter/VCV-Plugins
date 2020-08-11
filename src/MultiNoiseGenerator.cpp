//
//  Pink.cpp
//  AUPinkNoise
//
//  Created by Julian Porter on 05/06/2016.
//
//

#include "MultiNoiseGenerator.hpp"
#include <numeric>
#include <memory>
#include <iostream>
#include "Envelope.hpp"


namespace wind {



MultiNoiseGenerator::MultiNoiseGenerator(const double sampleRate_,const unsigned nSources)  : sampleRate(sampleRate_), N(nSources),
sources(N), generator(), chooser(), envState(), env() {
    generator.rebase(1.f);
    std::generate_n(sources.begin(), N, [this]() {
        double freq=generator.get(sampleRate);
        double phase=generator.get(PointSource::TwoPi);
        std::cerr << sampleRate << " " << freq << " " << phase << std::endl;
        return std::make_shared<PointSource>(sampleRate,freq,phase,env.it());
    });
};



void MultiNoiseGenerator::reset(const ParameterSet &params) {
     for(unsigned long index=0;index<sources.size();index++) {
        SourcePointer source=sources[index];
        source->change(params.mode);
        if(!params.range.contains(source->frequency())) {
            auto freq=generator.get();
            source->reset(freq,params.pEdge);
        }
        else if(chooser.get()) {
            auto freq=generator.get();
            source->reset(freq,params.pBody);
        }
    };
};


void MultiNoiseGenerator::generateN(const unsigned n,data_t *array,double v) {
    double vol=v/sqrt(double(n));
    data_t *it=array;
    for(unsigned tick=0;tick<n;tick++) {
        double out=std::accumulate(sources.begin(), sources.end(), 0.0, [](double tmp,SourcePointer &p) {
            return tmp+p->step();
        });
        *(it++)=data_t(vol*out);
    }
};

MultiNoiseGenerator::data_t MultiNoiseGenerator::Render(data_t *inBuffer, uint32_t inNumFrames, ParameterSet &params) {
    
    generator.rebase(params.range);
    reset(params);
    if(envState!=params.env) {
        envState=params.env;
        env.load(envState);
    }
    //env.reload(params.env);
    generateN(inNumFrames,inBuffer,params.volume);
    
    return (data_t)std::accumulate(inBuffer, inBuffer+inNumFrames, (data_t)0.0, [](data_t sum,data_t x) { return sum+x*x; });
};



}



