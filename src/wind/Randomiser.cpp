//
//  Randomiser.cpp
//  AUPinkNoise
//
//  Created by Julian Porter on 22/06/2016.
//
//

#include "Randomiser.hpp"
#include <iomanip>
#include <sstream>

namespace wind { namespace util {

void RandomBoolean::rebase(float p) {
    std::bernoulli_distribution::param_type par(p);
    actor.param(par);
};

void RandomDouble::rebase(double max) {
    std::uniform_real_distribution<double>::param_type par(0.0,max);
    actor.param(par);
};
void RandomDouble::rebase(const Range &r) {
    std::uniform_real_distribution<double>::param_type par(r.lo,r.hi);
    actor.param(par);
};



void RandomInteger::rebase(unsigned max) {
    std::uniform_int_distribution<unsigned>::param_type par(0,max);
    actor.param(par);
};
}}

bool operator==(const wind::util::Range &l,const wind::util::Range &r) { return l.lo==r.lo && l.hi==r.hi; }
bool operator!=(const wind::util::Range &l,const wind::util::Range &r)  { return l.lo!=r.lo || l.hi!=r.hi; }

std::string asHz(const float f,const unsigned n) {
	std::stringstream s;
	if(f<1000.f) { s << (int)f << " Hz"; }
	else { s << std::fixed << std::setprecision(n) << (f/1000.f) << " kHz"; }
	return s.str();
}

std::ostream & operator<<(std::ostream &o,const wind::util::Range &r) {
	o << asHz(r.lo) << " to " << asHz(r.hi);
	return o;
}
