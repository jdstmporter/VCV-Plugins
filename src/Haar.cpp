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
#include "Log.hpp"


namespace haar {





Haar::Haar(const unsigned N_) : N(N_), block(1<<N) {
	row = new float[block];
	tmp = new float[block];
	details = new float[block*(N+1)];

	reset();
	log() << "Initialised Haar wavelet class" << std::endl;
}

Haar::~Haar() {
	delete[] details;
	delete[] tmp;
	delete[] row;
}

unsigned Haar::length(const unsigned n) const {
	if(n<N) { return 1<<(N-1-n); }
	else { return 1; }
}
float * Haar::begin(const unsigned n) const { return details+block*n; }


void Haar::reset() {
	std::fill_n(tmp,block,0);
    std::fill_n(row,block,0);
    std::fill_n(details,block*(N+1),0);
}

float Haar::sum(const unsigned level) const {
	float sum=0;
	auto it=begin(level);
	for(unsigned i=0;i<length(level);i++) sum += it[i];
    return sum/(float)length(level);
}
float Haar::absMax(const unsigned level) const {
	auto it=begin(level);
	auto pair = std::minmax_element(it,it+length(level));
	return std::max(abs(*pair.first),*(pair.second));
}
float Haar::var(const unsigned level) const {
	float mean = sum(level);
	float sum=0;
	auto it=begin(level);
	for(unsigned i=0;i<length(level);i++) sum += (it[i]-mean)*(it[i]-mean);
	   return sum/(float)length(level);
}

void Haar::analyse(float * input) {
	log() << "copying input" << std::endl;
    std::copy_n(input,block,row);
    for(unsigned level=0;level<N;level++) {
    	log() << "Level " << level << std::endl;
    	auto it=begin(level);
        for(unsigned i=0; i<length(level); i++) {
            it[i] = 0.5*(row[2*i]-row[2*i+1]);
            tmp[i] = 0.5*(row[2*i]+row[2*i+1]);

        }
        log() << "Copying tmp" << std::endl;
        std::copy_n(tmp,length(level),row);
    }
    log() << "Copying approximation" << std::endl;
    begin(N)[0]=row[0];
}

void Haar::threshold(float *thresholds)  {
	auto s=sum();
    for(unsigned level=0;level<=N;level++) {
    	log() << "Level " << level << std::endl;
        auto ma = std::max(1.0f,absMax(level)*1.1f);
        auto th=thresholds[level] * ma;
        log() << "Abs max is " << ma << " raw threshold " << thresholds[level] << " threshold " << th << std::endl;
        auto it=begin(level);
        for(unsigned i=0;i<length(level);i++) {
        	if(abs(it[i])>th) it[i]=0;
        }
    }
}

void Haar::threshold(const Block &mu,const Block &sigma) {
	for(unsigned level=0;level<=N;level++) {
		auto it=begin(level);
		auto m=mu[level];
		auto t=sigma[level];
		for(unsigned i=0;i<length(level);i++) {
			if(abs(it[i]-m)>=t) it[i]=0;
		}
	}
}

void Haar::synthesise(float *out) {
	row[0]=begin(N)[0];
    for(int level=N-1;level>=0;level--) {
    	log() << "Level " << level << std::endl;
    	auto it=begin(level);
        for(unsigned i=0; i<length(level); i++) {
            tmp[2*i] = row[i] + it[i];
            tmp[2*i+1] = row[i] - it[i];
        }
        std::copy_n(tmp,2*length(level),row);
    }
    log() << "Copying out " << std::endl;
    std::copy_n(row,block,out);
}

void Haar::process(float *input, float *output,float *thresholds) {
	log() << "Analyse" << std::endl;
    analyse(input);
    log() << "Threshold" << std::endl;
    threshold(thresholds);
    log() << "Synthesise" << std::endl;
    synthesise(output);
    log() << "Done" << std::endl;
}




}


