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

#include <cassert>
#include <cmath>
#include <cstring>

#include <v8/base/fundamental_types.hpp>
#include <v8/math/math_utils.hpp>

namespace v8 { namespace math {

/**
 * \class   vector3
 * \brief   Represents a vector/point in R3. It is up to the user of the class
 * 			to make the distinction in code. A vector has a direction, 
 * 			a length(magnitude)	and a direction. A point has only the property of 
 * 			location. So operations	like length(), normalize(), addition, etc
 * 			are not valid for points. Yet many operations are common, so rather
 * 			then creating a point class and duplicating much of the code, it is 
 * 			up to the user to choose the meaning and apply the correct 
 * 			operations in code.
 */
template<typename real_t>
class vector3 {
public:
    enum { 
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    /**
     * \brief   Defines an alias representing type of the element.
     */
    typedef real_t          element_type;
    typedef real_t&         reference;
    typedef const real_t&   const_reference;
    typedef vector3<real_t> vector3_t;

    union {
        struct {
            real_t x_;
            real_t y_;
            real_t z_;
        };
        real_t elements_[3];	///< Allows array like access to the elements */
    };

    static const vector3_t zero;  ///< The null vector (0, 0, 0) of corresponding type */

    static const vector3_t unit_x;	///< The unit vector along the x axis (1, 0, 0) */

    static const vector3_t unit_y;	///< The unit vector along the y axis (0, 1, 0) */
  
    static const vector3_t unit_z;	///< The unit vector along the z axis (0, 0, 1) */

    /**
     * \brief   Default constructor. Leaves components uninitialized.
     */
    vector3() {}

    /**
     * \brief   Construct from 3 components.
     */
    vector3(real_t x, real_t y, real_t z) : x_(x), y_(y), z_(z) {}

    /**
     * \brief   Construct from an array of values.
     * \param   input   Pointer to an array of values.
     * \param   count   Number of elements in the array.
     */
    vector3(const real_t* input, size_t count);

    template<typename Convertible_Type>
    vector3(const vector3<Convertible_Type>& other)
        : x_(other.x_), y_(other.y_), z_(other.z_) {}
        
    template<typename Convertible_Type>
    vector3(const Convertible_Type (&arr)[3]) 
        : x_(arr[0]), y_(arr[1]), z_(arr[2]) {
    }

    template<typename Convertible_Type>
    vector3<real_t>& operator=(const vector3<Convertible_Type>& other) {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
        return *this;
    }
    
    template<typename Convertible_Type>
    vector3<real_t>& operator=(const Convertible_Type (&arr)[3]) {
        x_ = arr[0];
        y_ = arr[1];
        z_ = arr[2];
        return *this;
    }

    /**
     * \brief   Addition assignment operator.
     */
    template<typename Convertible_Type>
    inline vector3<real_t>& operator+=(const vector3<Convertible_Type>& rhs);

    /**
     * \brief   Subtraction assignment operator.
     */
    template<typename Convertible_Type>
    inline vector3<real_t>& operator-=(const vector3<Convertible_Type>& rhs);

    /**
     * \brief   Scalar multiplication assignment operator.
     */
    template<typename Convertible_Type>
    inline vector3<real_t>& operator*=(Convertible_Type k);

    /**
     * \brief   Scalar division assignment operator.
     */
    template<typename Convertible_Type>
    inline vector3<real_t>& operator/=(Convertible_Type k);

    /**
     * \brief   Returns the sum of the squares of the vector's components,
     * 			that is x^2 + y^2 + z^2 .
     */
    inline real_t sum_components_squared() const;

    /**
     * \brief length_squared
     * \returns The squared length of the vector.
     */
    inline real_t length_squared() const;

    /**
     * \brief   Computes the magnitude of the vector.
     */
    inline real_t magnitude() const;

    /**
     * \brief   Normalizes the vector (v = v / ||v||);
     */
    inline vector3<real_t>& normalize();

    /**
     * \brief Test if vector is unit length.
     * \return True if vector is unit length, false if not.
     */
    inline bool is_unit_length() const;
};

/**
 * \brief   Equality operator.
 */
template<typename real_t>
inline
bool
operator==(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Inequality operator.
 */
template<typename real_t>
inline
bool
operator!=(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Addition operator.
 */
template<typename real_t>
inline
math::vector3<real_t>
operator+(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
math::vector3<real_t>
operator-(
    const math::vector3<real_t>& lhs, const vector3<real_t>& rhs
    );

/**
 * \brief   Negation operator.
 */
template<typename real_t>
inline
math::vector3<real_t>
operator-(const math::vector3<real_t>& vec);

/**
 * \brief   Scalar multiplication operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector3<real_t>
operator*(
    const math::vector3<real_t>& vec, 
    Convertible_Type k
    );

/**
 * \brief   Scalar multiplication operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector3<real_t>
operator*(
    Convertible_Type k, 
    const math::vector3<real_t>& vec
    );

/**
 * \brief   Scalar division operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector3<real_t>
operator/(const math::vector3<real_t>& vec, Convertible_Type k);

/**
 * \brief   Dot product.
 */
template<typename real_t>
inline
real_t
dot_product(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Cross product.
 */
template<typename real_t>
inline
math::vector3<real_t>
cross_product(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Tests if vectors are orthogonal.
 */
template<typename real_t>
inline
bool
ortho_test(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Computes the angle between the two vectors.
 */
template<typename real_t>
inline
float
angle_of(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Projects a vector on another vector.
 * \param   lhs The projected vector.
 * \param   rhs The projection vector.
 * \remarks The formula for the projection of a vector P on a vector Q is :
 * 			((dot(P, Q) / ||Q||^2) * Q.
 * \return  A vector, representing the projection of lhs on rhs.
 */
template<typename real_t>
inline
math::vector3<real_t>
project_vector_on_vector(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Project vector on a unit vector.
 * \param   lhs The projected vector.
 * \param   rhs The projection vector.
 * \remarks Use this function if you know that the projection vector has 
 * 			unit length. The formula is dot(lhs, rhs) * rhs.
 * \return  A vector, representing the projection of lhs on rhs.
 */
template<typename real_t>
inline
math::vector3<real_t>
project_vector_on_unit_vector(
    const math::vector3<real_t>& lhs, const math::vector3<real_t>& rhs
    );

/**
 * \brief   Returns the normal of the given vector.
 */
template<typename real_t>
inline
math::vector3<real_t>
normal_of(const math::vector3<real_t>& vec);

/**
 * \brief   Returns the triple scalar product.
 * \remarks The formula for the triple scalar product of 3 vectors u, v, w is
 * 			dot(u, cross(v, w)).
 */
template<typename real_t>
inline
real_t
triple_scalar_product(
    const math::vector3<real_t>& v1, 
    const math::vector3<real_t>& v2, 
    const math::vector3<real_t>& v3
);

/**
 * \brief   Returns the triple vector product.
 * \remarks The formula for the triple vector product of three vectors u, v, w
 * 			is cross(u, cross(v, w)).
 */
template<typename real_t>
inline
math::vector3<real_t>
triple_vector_product(
    const math::vector3<real_t>& v1, 
    const math::vector3<real_t>& v2, 
    const math::vector3<real_t>& v3
);

/**
 * \brief   Returns the square of the distance between two points in R3.
 * \param   point1  The first point.
 * \param   point2  The second point.
 */
template<typename real_t>
inline
real_t
distance_squared(
    const math::vector3<real_t>& point1, const math::vector3<real_t>& point2
);

/**
 * \brief   Returns the distance between two points in R3.
 * \param   point1  The first point.
 * \param   point2  The second point.
 */
template<typename real_t>
inline
real_t
distance(
    const math::vector3<real_t>& point1, const math::vector3<real_t>& point2
    );

/**
 * \brief Convert from spherical coordinates to Cartesian coordinates, 
 * using a left handed coordinate system.
 * \param   delta   The radius of the sphere.
 * \param   phi     Angle with the y axis (in degrees).
 * \param   theta   Angle with the z axis (in degrees).
 * \remarks The formula for conversion is :
 * 			x = delta * sin(phi) * sin(theta)
 * 			y = delta * cos(phi)
 * 			z = -delta * sin(phi) * cos(theta)
 */
template<typename real_t>
inline vector3<real_t> 
point_from_spherical_coordinates(real_t delta, real_t phi, real_t theta);

/**
 * \brief   Convert from Cartesian coordinates to spherical coordinates.
 * \param   pt  A point, in Cartesian coordinates.
 * \return  A vector3 object, representing a tuple with the conversion result.
 * 			The x_ member represents the sphere's radius, 
 * 			y_ member is angle phi (angle with the y axis),
 * 			z_ member is angle theta (angle with z axis).
 * \remarks The formula for conversion is :
 * 			delta = pt.magnitude()
 * 			phi = atan2(sqrt(pt.x_ * pt.x_ + pt.z_ * pt.z_), pt.y_)
 * 			theta = atan2(pt.x_, pt.z_)
 */
template<typename real_t>
inline vector3<real_t> 
point_to_spherical_coordinates(const vector3<real_t>& pt);

template<typename real_t>
void generate_complement_basis(
    const vector3<real_t>& u_vec,
    vector3<real_t>* v_vec,
    vector3<real_t>* w_vec
    );

/**
 * \typedef vector3<float> vector3F
 * \brief   Defines an alias representing a vector3 having simple precision 
 * 			floating point components.
 */
typedef vector3<float>  vector3F;

/**
 * \typedef vector3<double> vector3D
 * \brief   Defines an alias representing a vector3 having double precision
 * 			floating point components.
 */
typedef vector3<double> vector3D;

} // namespace math
} // namespace v8

#include "vector3.inl"
