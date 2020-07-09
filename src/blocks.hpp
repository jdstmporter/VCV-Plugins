/*
 * blocks.hpp
 *
 *  Created on: 9 Jul 2020
 *      Author: julianporter
 */

#ifndef SRC_BLOCKS_HPP_
#define SRC_BLOCKS_HPP_

#include <vector>

namespace haar {
	class Block {
protected:
	unsigned N;
	std::vector<float> values;
	
public:
	Block(const unsigned n);
	Block(const Block &) = default;
	Block & operator =(const Block &) = default;
	virtual ~Block() = default;
	
	virtual float operator[](const unsigned) const;
	virtual void set(const unsigned,const float);
	void reset();
	
	Block operator +(const Block &);
	Block operator *(const Block &);
	Block operator /(const float);
	
};





class Threshold : public Block {
private:
	float lowerBound;
	float upperBound;
	
public:
	Threshold(const unsigned n,const float lower=0,const float upper=10);
	Threshold(const Threshold &) = default;
	Threshold & operator =(const Threshold &) = default;
	virtual ~Threshold() = default;
	
	virtual void set(const unsigned,const float);
	
};


}



#endif /* SRC_BLOCKS_HPP_ */
