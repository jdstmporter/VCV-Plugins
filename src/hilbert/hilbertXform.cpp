/*
 * Hilbert.cpp
 *
 *  Created on: 10 Jul 2020
 *      Author: julianporter
 */

#include <cmath>
#include <numeric>
#include <algorithm>
#include "hilbertXform.hpp"


namespace hilbert {

HilbertTransform::HilbertTransform(const unsigned n,const unsigned block) :
		nStages(n), nTaps(1+2*n), blockSize(block), firH(blockSize), firD(blockSize) {
	hilbertKernel=new float[nTaps];
	delayKernel=new float[nTaps];

	makeKernels();
}
HilbertTransform::~HilbertTransform() {
	delete [] hilbertKernel;
	delete [] delayKernel;
}
void HilbertTransform::makeKernels() {

	// delay
	for(unsigned i=0;i<nTaps;i++) delayKernel[i] = (i==nStages) ? 1.0 : 0.0;

	// hilbert transform
	for(unsigned i=0;i<nStages;i++) {
		float value = (0==(i&1)) ? 0.0 : 2.0/(M_PI * i);
		hilbertKernel[nStages+i]=value;
		hilbertKernel[nStages-i]=-value;
	}
	// normaliser
	//auto sumSquares = std::accumulate(hilbertKernel,hilbertKernel+nTaps,0.0,[](float n,float a) { return a+n*n; });
	//if(sumSquares>0) {
	//	auto normaliser = 1.0/(nTaps*sqrt(sumSquares));
	//	for(unsigned i=0;i<nTaps;i++) hilbertKernel[i]*=normaliser;
		//std::transform(hilbertKernel,hilbertKernel+nTaps,hilbertKernel,[normaliser](float x) { return x*normaliser; });
	//}

	firH.setKernel(hilbertKernel,nTaps);
	firD.setKernel(delayKernel,nTaps);
}

void HilbertTransform::process(const float *input,float *outX, float *outY)  {
	firD.processBlock(input,outX);
	firH.processBlock(input,outY);
}


} /* namespace hilbert */
