//
//  Randomiser.hpp
//  AUPinkNoise
//
//  Created by Julian Porter on 22/06/2016.
//
//

#ifndef Randomiser_hpp
#define Randomiser_hpp

#include <random>
#include <chrono>

namespace wind {
    namespace util {
        
        
        struct Range {
        public:
            double lo;
            double hi;
            double width;
            
            Range() : lo(0), hi(0), width(hi-lo) {};
            Range(const double lo_,const double hi_) : lo(std::min<double>(lo_,hi_)), hi(std::max<double>(lo_,hi_)), width(hi-lo) {};
            virtual ~Range() = default;
            Range & operator=(const Range o) {
                lo=o.lo;
                hi=o.hi;
                width=o.width;
                return *this;
            };
            bool contains(const double value) const { return (lo<=value) && (value<=hi); };
            
        };
        
        
        template <typename T,typename A>
        class RandomBase {
        protected:
            std::mt19937_64 generator;
            A actor;
            
        public:
            
            RandomBase() : generator(), actor() {
                generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
            };
            virtual ~RandomBase() = default;
            
            T get_() { return actor(generator); };
            virtual T get(T v)=0;
        };
        
        class RandomBoolean : public RandomBase<bool,std::bernoulli_distribution> {
            
        public:
            RandomBoolean() : RandomBase<bool,std::bernoulli_distribution>() {};
            virtual ~RandomBoolean() = default;
            
            virtual bool get(bool v=false) override { return get_(); };
            void rebase(float p);
        };
        
        class RandomDouble : public RandomBase<double,std::uniform_real_distribution<double>> {
        public:
            RandomDouble() : RandomBase<double,std::uniform_real_distribution<double>>() {};
            virtual ~RandomDouble() = default;
            
            virtual double get(double v=1.0) override { return v* get_(); };
            void rebase(double max);
            void rebase(const Range &r);
        };
        
        class RandomInteger : public RandomBase<unsigned,std::uniform_int_distribution<unsigned>> {
        public:
            RandomInteger() : RandomBase<unsigned,std::uniform_int_distribution<unsigned>>() {};
            virtual ~RandomInteger() = default;
            
            virtual unsigned get(unsigned v=1) override { return v*get_(); };
            void rebase(unsigned max);
        };
        
    }
}




#endif /* Randomiser_hpp */
