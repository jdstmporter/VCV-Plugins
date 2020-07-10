/*
 * Hilbert.hpp
 *
 *  Created on: 10 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_HILBERT_HPP_
#define SRC_HILBERT_HPP_

#include <dsp/fir.hpp>

namespace hilbert {

class Hilbert {
private:
	unsigned nStages;
	unsigned nTaps;
	unsigned blockSize;
	float *hilbertKernel;
	float *delayKernel;
	rack::dsp::RealTimeConvolver firH;
	rack::dsp::RealTimeConvolver firD;
public:
	Hilbert(const unsigned n,const unsigned block=64);
	virtual ~Hilbert();
	
	
	void process(float *input,float *outX, float *outY);
};

} /* namespace hilbert */

#endif /* SRC_HILBERT_HPP_ */
