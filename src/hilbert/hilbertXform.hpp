/*
 * Hilbert.hpp
 *
 *  Created on: 10 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_HILBERTXFORM_HPP_
#define SRC_HILBERTXFORM_HPP_


#include <dsp/fir.hpp>

namespace hilbert {

class HilbertTransform {
private:
	unsigned nStages;
	unsigned nTaps;
	unsigned blockSize;
	float *hilbertKernel;
	float *delayKernel;
	rack::dsp::RealTimeConvolver firH;
	rack::dsp::RealTimeConvolver firD;
	
	void makeKernels();
public:
	HilbertTransform(const unsigned n,const unsigned block=64);
	virtual ~HilbertTransform();
	
	
	void process(const float *input,float *outX, float *outY);
	
};

} /* namespace hilbert */

#endif /* SRC_HILBERTXFORM_HPP_ */
