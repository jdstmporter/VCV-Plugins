//
//  Envelope.cpp
//  AUPinkNoise
//
//  Created by Julian Porter on 24/06/2016.
//
//

#include "Envelope.hpp"
#include "splines.hpp"
#include <initializer_list>

namespace wind { namespace util {


EnvelopeIterator::EnvelopeIterator(std::vector<double> & v) : begin(v.begin()), it(begin) {
     end=v.begin()+v.size()-1;
 };


EnvelopeIterator & EnvelopeIterator::operator=(const EnvelopeIterator &o) {
    begin=o.begin;
    end=o.end;
    it=o.it;
    return *this;
};

double EnvelopeIterator::operator*() {
    if(it<end) it++;
    return *it;
}

void EnvelopeIterator::reset() {
    it=begin;
}

EnvelopeState & EnvelopeState::operator=(const EnvelopeState &o) {
	active=o.active;
	attack=o.attack;
	decay=o.decay;
	return *this;
}

bool EnvelopeState::operator==(const EnvelopeState &o) {
                return (active==o.active) && (attack==o.attack) && (decay==o.decay);
            };
bool EnvelopeState::operator!=(const EnvelopeState &o) {
                return (active != o.active) || (attack!=o.attack) || (decay!=o.decay);
            };

            std::vector<double> EnvelopeState::operator()() const {
                auto a=pow(10.0,attack/10.0);
                auto d=pow(10.0,decay/10.0);
                //std::cerr << "Envelope RAW " << attack << " " << decay << " Processed " << a << " " << d <<std::endl;
                double il[4] = {0.0,a,d,(2.0+a)/3.0,1.0};
                return std::vector<double>(il,il+4);
            };


const double XA[5]={0.0,100.0,150.0,250.0,300.0};
const std::vector<double> Envelope::X(XA,XA+5);

Envelope::Envelope() : envelope(300) {
    EnvelopeState e;
    load(e);
}

Envelope::Envelope(const EnvelopeState &state) : envelope(300) {
    load(state);
}




void Envelope::load(const EnvelopeState &state) {
    if(!state.active) {
        std::fill_n(envelope.begin(),300,1.0);
    }
    else {
        tk::spline spline;
        spline.set_points(X,state());
        unsigned n=0;
        std::generate_n(envelope.begin(),300,[&spline,&n]() { return spline(n++); });
    }
}

}}
