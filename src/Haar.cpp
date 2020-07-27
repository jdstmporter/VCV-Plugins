/*
 * Haar.cpp
 *
 *  Created on: 6 Jul 2020
 *      Author: julianporter
 */

#include "Haar.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <rack.hpp>


namespace haar {





Haar::Haar(const unsigned N_) : N(N_), block(1<<N) {
	row = new float[block];
	tmp = new float[block];
	details=new float[block*(N+1)];
	reset();
	INFO("Initialised Haar wavelet class");
}
Haar::~Haar() {
	INFO("Deinitialising Haar wavelet class");
	delete [] row;
	delete [] tmp;
	delete [] details;
}


unsigned Haar::length(const unsigned n) const {
	if(n<N) { return 1<<(N-1-n); }
	else { return 1; }
}
float * Haar::begin(const unsigned n) {
	return details+block*n;
}
float * Haar::end(const unsigned n) {
	return begin(n)+length(n);
}


void Haar::reset() {
	std::fill_n(tmp,block,0);
	std::fill_n(row,block,0);
	std::fill_n(details,block*(N+1),0);
}

void Haar::analyse(float * input) {
	//DEBUG("Haar wavelet copying input");
    std::copy_n(input,block,row);
    for(unsigned level=0;level<N;level++) {
    	//DEBUG("Haar wavelet at level %d", level);
    	auto it=begin(level);
        for(unsigned i=0; i<length(level); i++) {
            it[i]  = 0.5*(row[2*i]-row[2*i+1]);
            tmp[i] = 0.5*(row[2*i]+row[2*i+1]);

        }
        //DEBUG("Copying tmp");
        std::copy_n(tmp,length(level),row);
    }
    //DEBUG("Copying approximation");
    *begin(N)=row[0];
}

void Haar::threshold(float *thresholds,float scale)  {
    for(unsigned level=0;level<=N;level++) {
        auto th=thresholds[level]*scale;
        std::replace_if(begin(level),end(level),[th](float x) { return abs(x)>th; },0);
    }
}

void Haar::scale(float *scales) {
	for(unsigned level=0;level<=N;level++) {
		auto scale=scales[level];
		auto it=begin(level);
		for(unsigned i=0; i<length(level); i++) {  it[i] *= scale; }
	}
}

void Haar::synthesise(float *out) {
	row[0]=*begin(N);
    for(int level=N-1;level>=0;level--) {
    	auto it=begin(level);
        for(unsigned i=0; i<length(level); i++) {
            tmp[2*i]   = row[i] + it[i];
            tmp[2*i+1] = row[i] - it[i];
        }
        std::copy_n(tmp,2*length(level),row);
    }
    //DEBUG("Copying out wavelet reconstruction");
    std::copy_n(row,block,out);
}

float Haar::absMaximum() const {
	auto its = std::minmax_element(details,details+block*(N+1));
	return std::max(abs(*its.first),abs(*its.second));
}





}


