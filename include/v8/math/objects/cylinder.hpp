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

#include <v8/math/tridimensional_space_traits.hpp>
#include <v8/math/objects/line.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/**
 * \class   cylinder
 * \brief   Abstract representation of a cylinder. The cylinder is the set of all
 * 			points situated at a distance r from a line (the cylinder's axis). 
 * 			A cylidenr can be finite or infinite. An infinite cylinder has 
 * 			infinite height. A finite cylinder has endpoints 
 * 			(C - H/2 * D, C + H/2 * D), where C is the center 
 * 			point of the axis line, H is the height of the cylinder and D is
 * 			the unit length direction vector of the axis line.
 */
template<typename real_t>
class cylinder {
public :
    typedef vector3<real_t>                     vector3_t;
    typedef line<real_t, Space_Tridimensional>  line3_t;

    line3_t axis_;  /*!< The axis of the cyliner. */
    real_t  radius_;	/*!< The radius around the axis line. */
    real_t  height_;	/*!< The height. Set to FLT_MAX for an infinite cylinder. */

    cylinder() {}

    cylinder(const line3_t& axis, real_t radius, real_t height)
        : axis_(axis), radius_(radius), height_(height) {}
};

/**
 * \typedef cylinder<float> cylinderF
 * \brief   A cylinder object, using single precision floating point components.
 */
typedef cylinder<float>     cylinderF;

/**
 * \typedef cylinder<double> cylinderD
 * \brief   A cylinder object, using double precision floating point components.
 */
typedef cylinder<double>    cylinderD;

} // namespace math
} // namespace v8
