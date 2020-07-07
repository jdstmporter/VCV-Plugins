/*
 * Haar.hpp
 *
 *  Created on: 6 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_HAAR_HPP_
#define SRC_HAAR_HPP_

#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>
#include <map>
#include <memory>


namespace haar {

class Haar {
private:
    using vec_t = std::vector<float>;
    using iterator = std::vector<float>::iterator;
    
    unsigned N;
    unsigned block;
    vec_t row;
    vec_t tmp;
    vec_t details; 
    
    
    
    unsigned length(const unsigned n);
    iterator begin(const unsigned n);
    iterator end(const unsigned n);
    
    
public:
    Haar(const unsigned N_);
    Haar(const Haar &) = default;
    virtual ~Haar() =default;
    
    float sum(const unsigned level);
    float sumSq(const unsigned level);
    
    unsigned size() const { return block; }
    void reset();
    
    void analyse(float * input);
    void threshold(float *thresholds);
    void synthesise(float *out);
    
    void process(float *input,float *output,float *thresholds);
 
};


}


#endif /* SRC_HAAR_HPP_ */
