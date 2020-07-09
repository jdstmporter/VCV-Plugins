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
    
    unsigned N;
    unsigned block;
    float *row;
    float *tmp;
    float *details; 
    
    
    
    unsigned length(const unsigned n);
    float * begin(const unsigned n);

    
    
public:
    Haar(const unsigned N_);
    Haar(const Haar &) = default;
    virtual ~Haar();
    
    Block sum();
    Block var();
    
    unsigned size() const { return block; }
    void reset();
    
    void analyse(float * input);
    void threshold(float *thresholds);
    void threshold(const Block & mu,const Block & theta);
    void synthesise(float *out);
    
    void process(float *input,float *output,float *thresholds);
 
};


}


#endif /* SRC_HAAR_HPP_ */
