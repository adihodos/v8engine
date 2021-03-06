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

#include <memory.h>
#include <cmath>

#include <v8/v8.hpp>
#include <v8/base/fundamental_types.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

/** \addtogroup __grp_v8_math_algebra
 * @{
 */

/**
 * \brief Represents a quaternion, parameterized by the type of its elements.
 * \remarks Quaternion multiplication is not commutative, that is given
 *          two quaternions \a q0 and \a q1, <b> q0 * q1 != q1 * q0 </b>.
 */
template<typename real_t>
class quaternion {
public :
    /** Type of components. */
    typedef real_t              element_type;

    /** Type of reference to a component. */
    typedef real_t&             reference;

    /** Type of const reference to a component. */
    typedef const real_t&       const_reference;

    /** Fully qualified type of this class. */
    typedef quaternion<real_t>  class_type;

    union {
        struct {
            real_t w_;
            real_t x_;
            real_t y_;
            real_t z_;
        };
        real_t elements_[4];
    };

    /** The null quaternion (identity for addition). */
    static const quaternion<real_t>     null;

    /** The identity quaternion for multiplication */
    static const quaternion<real_t>     identity;

    /**
     \brief Default constructor, leaves the object uninitialized.
     */
    inline quaternion();

    /**
     \brief Constructs a quaternion using the specified values.
     */
    inline quaternion(
        const real_t w, 
        const real_t x, 
        const real_t y, 
        const real_t z
        );

    /**
     \brief Constructs a quaternion, using the specified array of values for
            initialization.
     \param init_data   Pointer to an array of at least 4 elements. Must not be
                        null.
     */
    inline quaternion(
        const real_t* init_data
        );

    /**
     \brief Constructs a quaternion from axis-angle format.
     \param angle   The angle of rotation around the axis, expressed in radians.
     \param axis    Vector that represents the axis of rotation. It is not 
                    necessary to be unit length.
     */
    inline quaternion(
        const float angle, 
        const math::vector3<real_t>& axis
        );

    /**
     \brief Given two vectors v1 and v2, constructs a quaternion that represents
            the rotation of v1 into v2.
     */
    inline quaternion(
        const math::vector3<real_t>& v1, 
        const math::vector3<real_t>& v2
        );

    /**
     \brief Set this quaternion to the null quaternion (0, 0, 0, 0).
     */
    inline quaternion<real_t>& make_zero();

    /**
     \brief Set this quaternion to be the identity quaternion (1, 0, 0, 0).
     */
    inline quaternion<real_t>& make_identity();

    /**
     * \brief Constructs from a vector and a scalar value. Given the vector v
              and the scalar w, it will create a quaternion q(w, v).
     */
    inline quaternion<real_t>& make_from_vector_and_scalar(
        const vector3<real_t>& vec,
        const real_t scalar = real_t(0)
        );

    /**
     \brief Construct from axis-angle format.
     \param angle   The angle of rotation arount the axis, expressed in radians.
     \param axis    Vector that represents the axis of rotation. It is not 
                    necessary to be unit length.
     */
    quaternion<real_t>& make_from_axis_angle(
        const float angle, 
        const math::vector3<real_t>& axis
        );

    /**
     \brief Given two unit length vectors v1 and v2, construct a quaternion 
            that represents the rotation of v1 into v2.
     \param[in] v1  The first vector.
     \param[in] v2  The second vector.
     \remarks Both v1 and v2 are expected to be unit length vectors.
     */
    quaternion<real_t>& make_from_vectors(
        const math::vector3<real_t>& v1, 
        const math::vector3<real_t>& v2
        );

    /**
     \brief Converts the specified rotation matrix to quaternion format.
     \param[in] mtx Rotation matrix.
     */
    quaternion<real_t>& make_from_matrix(
        const math::matrix_3X3<real_t>& mtx
        );

    quaternion<real_t>& operator+=(const quaternion<real_t>& rhs);

    quaternion<real_t>& operator-=(const quaternion<real_t>& rhs);

    quaternion<real_t>& operator*=(const real_t scalar);

    quaternion<real_t>& operator/=(const real_t scalar);

    /**
     \brief Makes this quaternion unit-length.
     */
    quaternion<real_t>& normalize();

    /**
     \brief Sets this quaternion's value to that of its conjugate. The conjugate
            of a quaternion q = [s, v] is the quaternion q' = [s, -v].
     */
    inline quaternion<real_t>& make_conjugate();

    /**
     \brief Sets this quaternion's value to that of it's inverse. The inverse of
            a quaternion q = [s, v] is the quaternion q' = (s, -v) / ||q|| ^ 2.
     */
    quaternion<real_t>& invert();

    /**
     \brief Stores the rotation encoded by this quaternion into a matrix.
     \param [in,out] mtx    Pointer to a matrix_4X4 that performs the rotation
                            encoded by this quaternion.
     */
    void extract_rotation_matrix(
        math::matrix_4X4<real_t>* mtx
        ) const;

    /**
     \brief Converts this quaternion to axis-angle format.
     \param [in,out] axis   Pointer to a vector3 that receives the axis of
                            rotation. Must not be null.
     \param [in,out] angle  Pointer to a value that receives the angle of
                            rotation around the axis (in radians). 
                            Must not be null.
     */
    quaternion<real_t>& extract_axis_angle(
        math::vector3<real_t>* axis, 
        real_t* angle
        ) const;

    /**
     \brief Applies the encoded rotation to the specified vector. If you need
            to transform multiple vectors, it is more efficient to convert the
            quaternion to a rotation matrix and use the matrix to transform the 
            vectors. The formula for rotation, given the quaternion \a Q and the
            vector \a V, is <b>QVQ^(-1)</b>.
     \param vec The vector to be rotated.
     */
    void rotate_vector(math::vector3<real_t>* vec);

    /**
     \brief Returns ||q|| ^ 2.
     */
    inline real_t length_squared() const;

    /**
     \brief Returns the magnitude of the quaternion.
     */
    inline real_t magnitude() const;

    /**
     \brief Query if this object is the null quaternion.
     */
    bool is_zero() const;

    /**
     \brief Query if this object is a unit length quaternion.    
     */
    bool is_unit() const;

    /**
     \brief Query if this object is the identity quaternion.
     */
    bool is_identity() const;
};

template<typename real_t>
const quaternion<real_t>
quaternion<real_t>::null(real_t(0), real_t(0), real_t(0), real_t(0));

template<typename real_t>
const quaternion<real_t>
quaternion<real_t>::identity(real_t(1), real_t(0), real_t(0), real_t(0));

/**
 \brief Computes the dot product of the specified quaternions.
 */
template<typename real_t>
inline
real_t
dot_product(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
math::quaternion<real_t>
inverse_of(
    const math::quaternion<real_t>& quat
    );

template<typename real_t>
inline
math::quaternion<real_t>
normal_of(
    const math::quaternion<real_t>& quat
    );

template<typename real_t>
inline
math::quaternion<real_t>
conjugate_of(
    const math::quaternion<real_t>& quat
    );

template<typename real_t>
bool
operator==(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
bool
operator!=(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
math::quaternion<real_t>
operator+(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
math::quaternion<real_t>
operator-(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
math::quaternion<real_t>
operator-(
    const math::quaternion<real_t>& quat
    );

template<typename real_t>
math::quaternion<real_t>
operator*(
    const math::quaternion<real_t>& lhs, 
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
math::quaternion<real_t>
operator*(
    const math::quaternion<real_t>& lhs, 
    const real_t scalar
    );

template<typename real_t>
inline
math::quaternion<real_t>
operator*(
    const real_t scalar,
    const math::quaternion<real_t>& rhs
    );

template<typename real_t>
math::quaternion<real_t>
operator/(
    const math::quaternion<real_t>& lhs,
    const real_t scalar    
    );

typedef quaternion<float>       quaternionF;

typedef quaternion<double>      quaternionD;

/** @} */

} // namespace math
} // namespace v8

#include "quaternion.inl"
