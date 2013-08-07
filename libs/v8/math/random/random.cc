#include "mtrand.h"
#include "v8/math/random/random.hpp"

struct v8::math::random::implementation_details {
    implementation_details() 
        :       MersenneTwisterRND() {}

    implementation_details(const v8_uint32_t seed)
        :       MersenneTwisterRND(seed) {}


    MTRand_int32        MersenneTwisterRND;
};

v8::math::random::random()
    :       impl_(new implementation_details()) 
{}

v8::math::random::random(const v8_uint32_t seed)
    :       impl_(new implementation_details(seed)) 
{}

v8::math::random::~random() {}

v8_uint32_t 
v8::math::random::next() {
    return impl_->MersenneTwisterRND();
}

v8_uint32_t 
v8::math::random::next(const v8_uint32_t max) {
    return static_cast<v8_uint32_t>(next_double() * static_cast<double>(max));
}

double 
v8::math::random::next_double() {
    return static_cast<double>(next()) * (1. / 4294967296.); // divided by 2^32
}

double v8::math::random::next_double_closed() {
    return static_cast<double>(next()) * (1. / 4294967295.);
}