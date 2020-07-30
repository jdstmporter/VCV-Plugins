//
//  Envelope.hpp
//  AUPinkNoise
//
//  Created by Julian Porter on 24/06/2016.
//
//

#ifndef Envelope_hpp
#define Envelope_hpp

#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <initializer_list>

namespace wind { namespace util {


struct EnvelopeIterator  {
private:    
    std::vector<double>::iterator begin;
    std::vector<double>::iterator end;
    std::vector<double>::iterator it;
    
public:
    
    EnvelopeIterator() = default;
    EnvelopeIterator(std::vector<double> & v);
    EnvelopeIterator(const EnvelopeIterator &o) : begin(o.begin), end(o.end), it(o.it) {};
    //EnvelopeIterator(const vec_t & v) : vector(v), index(0), size(v->size()-1), value(0) {};
    //EnvelopeIterator(const EnvelopeIterator &o) : vector(o.vector), index(o.index), size(o.size), value(o.value) {};
    virtual ~EnvelopeIterator() = default;
    EnvelopeIterator & operator=(const EnvelopeIterator &o);

    double operator *();
    void reset();
    
};
        
        struct EnvelopeState {
            bool active;
            double attack;
            double decay;
            
            EnvelopeState(double attack_=8.0,double decay_=-1.0,bool active_=false) : active(active_), attack(attack_), decay(decay_) {};
            EnvelopeState(const EnvelopeState &o) : active(o.active), attack(o.attack), decay(o.decay) {};
            virtual ~EnvelopeState() = default;
            
            EnvelopeState & operator=(const EnvelopeState &o);
            
            bool operator==(const EnvelopeState &o) ;
            bool operator!=(const EnvelopeState &o) ;
            
            std::vector<double> operator()() const ;
        };


class Envelope {
private:
    
    static const std::vector<double> X;
    std::vector<double> envelope;

public:
    using iterator = EnvelopeIterator;
    using val_t = std::pair<double,double>;
    
    Envelope();
    Envelope(const EnvelopeState &state);
    virtual ~Envelope() = default;
    
    void load(const EnvelopeState &s);
    iterator it() { return iterator(envelope); };
    
};
        
    }}




#endif /* Envelope_hpp */
