/*
 * blocks.cpp
 *
 *  Created on: 9 Jul 2020
 *      Author: julianporter
 */

#include "blocks.hpp"
#include "math.hpp"

namespace haar {

Block::Block(const unsigned n) : N(n), values(N+1,0) {};

float Block::operator[](const unsigned level) const { return values.at(level); }
void Block::set(const unsigned level,const float value) { values.at(level)=value; }
void Block::reset() { values.assign(N+1,0); }


Block Block::operator +(const Block &rhs) {
		Block s(N);
		for(unsigned i=0;i<=N;i++) s.set(i,values[i]+rhs[i]);
		return s;
	}
Block Block::operator *(const Block &rhs) {
		Block s(N);
		for(unsigned i=0;i<=N;i++) s.set(i,values[i]*rhs[i]);
		return s;
	}
Block Block::operator /(const float f) {
	Block s(N);
	for(unsigned i=0;i<=N;i++) s.set(i,values[i]/f);
	return s;
}

Threshold::Threshold(const unsigned n,const float lower,const float upper) : Block(n),
		lowerBound(lower), upperBound(upper) {};

void Threshold::set(const unsigned level,const float value) {
	values.at(level)=rack::math::clamp(value,lowerBound,upperBound);
}


}


