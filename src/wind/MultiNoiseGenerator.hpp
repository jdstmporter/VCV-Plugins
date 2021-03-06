/*
Copyright (C) 2016 Apple Inc. All Rights Reserved.
See LICENSE.txt for this sample’s licensing information

*/

#ifndef __NoiseGenerator
#define __NoiseGenerator

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include "PointSource.hpp"
#include "Randomiser.hpp"
#include "ParameterSet.hpp"
#include "Windy.hpp"



namespace wind {


    class MultiNoiseGenerator {
    using SourcePointer = std::shared_ptr<PointSource>;
    using data_t = float;
    
private:
    
    double sampleRate;
    const unsigned N;
    std::vector<SourcePointer> sources;
    util::RandomDouble generator;
    util::RandomBoolean chooser;
    util::EnvelopeState envState;
    util::Envelope env;
 
protected:
    void reset(const ParameterSet &params);
    void generateN(const unsigned n,data_t *array,double v);

    
    
public:
    
    
    MultiNoiseGenerator(const double sampleRate=SAMPLE_RATE,const unsigned nSources=NOSCILLATORS);
    virtual ~MultiNoiseGenerator() = default;
    
    
    virtual data_t Render(data_t *inBuffer, uint32_t inNumFrames, ParameterSet &params);
    
    
    
};

}

#endif



