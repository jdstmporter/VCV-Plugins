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
    float * row;
    float * tmp;
    float * details; 
    
    
    
    unsigned length(const unsigned n) const;
    float * begin(const unsigned n);
    float * end(const unsigned n);

public:
    Haar(const unsigned N_);
    Haar(const Haar &) = default;
    virtual ~Haar();

    unsigned size() const { return block; }
    void reset();
    
    void analyse(float *);
    void threshold(float *,float scale=1.0);
    void synthesise(float *out);
    
    float absMaximum() const;

 
};


}


#endif /* SRC_HAAR_HPP_ */
