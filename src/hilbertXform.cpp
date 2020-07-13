/*
 * Hilbert.cpp
 *
 *  Created on: 10 Jul 2020
 *      Author: julianporter
 */

#include <cmath>
#include "hilbertXform.hpp"


namespace hilbert {

Hilbert::Hilbert(const unsigned n,const unsigned block) :
		nStages(n), nTaps(1+2*n), blockSize(block), firH(blockSize), firD(blockSize) {
	hilbertKernel=new float[nTaps];
	delayKernel=new float[nTaps];

	for(unsigned i=0;i<nStages;i++) {
		float value = (0==(i&1)) ? 0.0 : 2.0/(M_PI * i);
		hilbertKernel[nStages+i]=value;
		hilbertKernel[nStages-i]=-value;
	}
	firH.setKernel(hilbertKernel,nTaps);

	for(unsigned i=0;i<nTaps;i++) delayKernel[i] = (i==nStages) ? 1.0 : 0.0;
	firD.setKernel(delayKernel,nTaps);

};
Hilbert::~Hilbert() {
	delete [] hilbertKernel;
	delete [] delayKernel;
}

void Hilbert::process(float *input,float *outX, float *outY) {
	firH.processBlock(input,outX);
	firH.processBlock(input,outY);
}

} /* namespace hilbert */
