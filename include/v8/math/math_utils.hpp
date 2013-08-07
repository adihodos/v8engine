//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <cmath>

#include <v8/v8.hpp>
#include <v8/base/fundamental_types.hpp>
#include <v8/math/math_constants.hpp>

namespace v8 { namespace math { namespace internals {

template<typename real_type, bool is_floating_point = false>
struct op_eq_helper {
    static bool result(real_type left, real_type right) {
        return left == right;
    }
};

template<typename real_type>
struct op_eq_helper<real_type, true> {
    static bool result(real_type left, real_type right) {
        return abs(left - right) < numerics<real_type>::epsilon();
    }
};

template<typename T, bool is_floating_point>
struct div_helper;

template<typename T>
struct div_helper<T, false> {
    T divisor_;
  
    div_helper(T divisor) : divisor_(divisor) {}
  
    void set_divisor(T div) { divisor_ = div; }
  
    T operator()(T divided_value) const {
        return divided_value / divisor_;
    }
};

template<typename T>
struct div_helper<T, true> {
    T divisor_;
    T inv_divisor_;
  
    div_helper(T divisor) : divisor_(divisor), inv_divisor_(T(1) / divisor) {}
  
    void set_divisor(T divisor) {
        divisor_ = divisor;
        inv_divisor_ = T(1) / divisor_;
    }
  
    T operator()(T divided_value) const {
        return divided_value * inv_divisor_;
    }
};

template<typename T>
struct div_wrap_t {
    typedef div_helper<T, base::is_floating_point_type<T>::Yes>  div_helper_t;
};

} // namespace internals

//! \defgroup Utility
//! \brief Math helper functions and classes.
//! @{

//!
//! \brief Test if two operands are equal. This will do an epsilon diff compare
//! for floating point operands and an equality test for integer operands.
//! \param[in] left Left operand.
//! \param[in] right Right operand.
//! \returns True if left operand is equal to right operand, false otherwise.
template<typename T>
inline
bool
operands_eq(const T left, const T right) {
    return internals::op_eq_helper<
        T, base::is_floating_point_type<T>::Yes
    >::result(left, right);
}

template<typename T>
inline 
bool
operands_ge(const T left, const T right) {
    return left > right || operands_eq(left, right);
}

template<typename T>
inline bool
operands_le(const T left, const T right) {
    return left < right || operands_eq(left, right);
}

template<typename real_t>
inline
bool
is_zero(const real_t value) {
    return fabs(value) < numerics<real_t>::epsilon();
}

/*!
 * \brief Returns sgn(x) = { 1,     x >= 0
 *                           -1,    x < 0 }
 */
template<typename T>
inline v8_int_t signum(const T val) {
    return (val > T(0)) - (val < T(0));
}

/**
 * \brief   Converts from radians to degrees.
 */
template<typename real_t>
inline
real_t
to_degrees(const real_t radians) {
    return radians * numerics<real_t>::one_eighty_over_pi();
}

/**
 * \brief   Converts from degrees to radians.
 */
template<typename real_t>
inline
real_t
to_radians(const real_t degrees) {
    //return degrees * constants::kPiOver180;
    return degrees * numerics<real_t>::pi_over180();
}

/*!
 * \brief Returns the inverse of the square of x.
 */
template<typename real_t>
inline 
real_t
inv_sqrt(const real_t val) {
    return real_t(1) / sqrt(val);
}

template<typename T>
inline T clamp(const T& val, const T& min, const T& max) {
  return (val < min ? min : (val > max ? max : val));
}

template<typename Ty>
inline Ty min(const Ty& left, const Ty& right) {
    return left < right ? left : right;
}

template<typename T>
inline T max(const T& left, const T& right) {
    return left > right ? left : right;
}

template<typename ty>
inline void swap(ty& left, ty& right) {
    ty temp = left;
    left = right;
    right = temp;
}

template<typename real_t>
real_t angle_from_xy(const real_t x, const real_t y) {
    real_t theta = real_t(0);

    if (operands_ge(x, real_t(0))) {
        theta = atan(y / x);
        if (theta < real_t(0)) {
            theta += real_t(2) * numerics<real_t>::pi();
        }
    } else {
        theta = atan(y / x) + numerics<real_t>::pi();
    }
    return theta;
}

//!
//! \brief Generates a random number, in the given range.
//! \param[in] range_min Minimum range value.
//! \param[in] range_max Maximum range value.
//! \returns Random number r, such that range_min <= r <= range_max.
//! \note A call to seed must be made prior to using this function for the
//! first time.
template<typename real_t>
inline real_t random_number_in_interval(
    const real_t range_min, const real_t range_max
    ) {
    const float rand_num = 
        (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) 
        * (range_max - range_min) 
        + range_min;
    return static_cast<real_t>(rand_num);
}

template<typename T>
struct Interpolator {
    T operator()(const T& left, const T& right, const float amount) const {
        return left * (1.0f - amount) + right * amount;
    }
};

//! @}

} // namespace math
} // namespace v8
