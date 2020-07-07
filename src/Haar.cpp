/*
 * Haar.cpp
 *
 *  Created on: 6 Jul 2020
 *      Author: julianporter
 */

#include "Haar.hpp"

namespace haar {

Haar::Haar(const unsigned N_) : N(N_), block(1<<N), row(block,0), tmp(block,0), details(block,0) {
}

unsigned Haar::length(const unsigned n) {
	if(n<N) { return 1<<(N-1-n); }
	else { return 1; }
}
Haar::iterator Haar::begin(const unsigned n) {
	if(n<N) { return details.begin()+block-(1<<(N-n)); }
	else { return details.begin()+block-1; }
}
Haar::iterator Haar::end(const unsigned n) {
	if(n<N) { return begin(n)+length(n); }
	else { return details.end(); }
}

void Haar::reset() {
	details.assign(block,0);
    row.assign(block,0);
    tmp.assign(block,0);
}

float Haar::sum(const unsigned level) {
    return std::accumulate(begin(level),end(level), 0, [](float p,float x) { return p+x;} );
}
float Haar::sumSq(const unsigned level) {
    return std::accumulate(begin(level),end(level), 0, [](float p,float x) { return p+x*x;} );
}

void Haar::analyse(float * input) {
    std::copy_n(input,block,row.begin());
    for(unsigned level=0;level<N;level++) {
    	unsigned i=0;
        for(auto it=begin(level); it!=end(level); i++,it++) {
            *it = 0.5*(row[2*i]-row[2*i+1]);
            tmp[i] = 0.5*(row[2*i]+row[2*i+1]);

        }
        std::copy_n(tmp.begin(),length(level),row.begin());
    }
    std::copy_n(row.begin(),length(N),begin(N));
}

void Haar::threshold(float *thresholds)  {
    for(unsigned level=0;level<=N;level++) {
        auto mu=sum(level)/float(length(level));
        auto th=thresholds[level];
        std::replace_if(begin(level),end(level),[mu,th](float value) { return abs(value-mu)>th; },0);
    }
}

void Haar::synthesise(float *out) {
    std::copy_n(begin(N),length(N),row.begin());
    for(unsigned level=N-1;level>=0;level--) {
    	unsigned i=0;
        for(auto it=begin(level); it!=end(level); i++,it++) {
            tmp[2*i] = row[i] + *it;
            tmp[2*i+1] = row[i] - *it;
        }
        std::copy_n(tmp.begin(),2*length(level),row.begin());
    }
    std::copy_n(row.begin(),block,out);
}

void Haar::process(float *input, float *output,float *thresholds) {
    analyse(input);
    threshold(thresholds);
    synthesise(output);
}




}


