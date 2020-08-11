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

std::ostream & operator<<(std::ostream &o,const wind::util::Range &r) {
	std::stringstream s;

	auto l=r.lo;
	if(l<1000.f) { s << (int)l << " Hz"; }
	else { s << std::fixed << std::setprecision(3) << (l/1000.f) << " kHz"; }
	s << " to ";
	auto h=r.hi;
	if(h<1000.f) { s << (int)h << " Hz"; }
	else { s << std::fixed << std::setprecision(3) << (h/1000.f) << " kHz"; }
	o << s.str();
	return o;
}
