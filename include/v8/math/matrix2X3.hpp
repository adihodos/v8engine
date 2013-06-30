//
// Copyright (c) 2011, 2012, 2013 Adrian Hodos
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
#include <v8/base/count_of.hpp>
#include <v8/base/fundamental_types.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/vector2.hpp>
#include <v8/math/angle_units.hpp>

namespace v8 { namespace math {

/*! \addtogroup __grp_v8_math_algebra
 *  @{
 */    

/*!
 * \brief A two by three matrix for applying linear/affine
 *      transformations to R^2 vectors/points. The matrix is
 *      stored in row major format. The third row is implied
 *      to be (0, 0, 1). The matrix follows the convention
 *      that it multiplies column vectors, on the right side.
 *      When concatenating matrices representings a sequence
 *      of transformations, the matrices must be multiplied from
 *      last to first. So the sequence S1, S2, S3, ... S(n - 1), Sn
 *      must be concatenated as :
 *      Sn * S(n-1) * S(n -2) * ... * S3 * S2 * S1.
 */
template<typename real_t>
class matrix_2X3 {

/*! \name Data members.
 *  @{
 */

public :

    enum {
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    union {
        struct {
            real_t a11_; real_t a12_; real_t a13_;
            real_t a21_; real_t a22_; real_t a23_;
        };
        real_t elements_[6];
    };

/*! @} */

/*! \name Defined types and class constants.
 *  @{
 */

public :

     /*!< Alias for the type of elements of the matrix. */
    typedef real_t                              element_type;

    /*!< Type of an object this class. */
    typedef matrix_2X3<real_t>                  class_type;

    static const matrix_2X3<real_t>             zero;

    static const matrix_2X3<real_t>             identity;

/*! @} */

/*! \name Constructors.
 *  @{
 */

public :

    /*!
     * \brief Default constructor, leaves elements uninitialized.
     */
    matrix_2X3() {}

    /*!
     * \brief Construct with six explicit values.
     */
    inline matrix_2X3(const real_t      a11, 
                      const real_t      a12, 
                      const real_t      a13,
                      const real_t      a21, 
                      const real_t      a22, 
                      const real_t      a23);

    /*!
     * \brief Construct from a matrix with convertible element type.
     */
    template<typename real_u>
    inline matrix_2X3(const matrix_2X3<real_u>& other);

    /*!
     * \brief Construct from an array of existing values.
     * \param[in] data Pointer to existing data.
     * \param[in] count Number of entries in the data array.
     */
    inline matrix_2X3(const real_t* data, const v8_size_t count);

/*! @} */

/*! \name Operators.
 *  @{
 */

public :

    /*!
     * \brief Indexing operator.
     * \param[in] row Row of the accessed element, 
     *              using <b>1 based indexing</b>.
     * \param[in] col Column of the accessed element,
     *              using <b>1 based indexing</b>.
     */
    real_t& 
    operator()(const v8_size_t row, const v8_size_t col) {
        return elements_[(row - 1) * 3 + (col - 1)];
    }

    /*!
     * \brief Indexing operator.
     * \param[in] row Row of the accessed element, 
     *              using <b>1 based indexing</b>.
     * \param[in] col Column of the accessed element,
     *              using <b>1 based indexing</b>.
     */
    const real_t& 
    operator()(const v8_size_t row, const v8_size_t col) const {
        return elements_[(row - 1) * 3 + (col - 1)];
    }

    template<typename real_u>
    inline matrix_2X3<real_t>& operator=(
        const matrix_2X3<real_u>& convertible
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator+=(
        const matrix_2X3<real_u>& conv
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator-=(
        const matrix_2X3<real_u>& conv
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator*=(
        real_u conv_scalar
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator/=(
        real_u conv_scalar
        );

/*! @} */

/*! \name   Helper functions for building various affine transforms.
 *  @{
 */

public :

    /*!
     * \brief Set this matrix to the null matrix.
     */
    inline matrix_2X3<real_t>& 
    make_zero();

    /*!
     * \brief Set this matrix to the identity matrix.
     */
    inline matrix_2X3<real_t>& 
    make_identity();

    /*!
     * \brief Builds a rotation matrix around the origin.
     * \param[in] theta Angle of rotation, <b>in radians</b>.
     */
    inline class_type& 
    make_rotation(const real_t radians);

    /*!
     * \brief Builds a rotation matrix around an arbitrary point.
     * \param radians Angle of rotation, <b>in radians</b>.
     * \param xpos X coordinate of the point.
     * \param ypos Y coordinate of the point.
     */
    inline class_type& 
    make_rotation_off_center(const real_t radians,
                             const real_t xpos,
                             const real_t ypos);

    /*!
     * \brief Builds a scaling matrix.
     * \param[in] sx Scale factor along the X axis.
     * \param[in] sy Scale factor along the Y axis.
     */
    inline matrix_2X3<real_t>& 
    make_scaling(const real_t sx, const real_t sy);

    /**
     * \brief Builds a scaling matrix from a vector2 object.
     * \param[in] sv Two component vector, containing scale
     *              factors along the X and Y axes.
     */
    matrix_2X3<real_t>& 
    make_scaling(const vector2<real_t>& sv) {
        return make_scale(sv.x_, sv.y_);
    }

    /*!
     * \brief Builds a translation matrix.
     * \param[in] tx Translation component along the <b>X axis</b>.
     * \param[in] ty Translation component along the <b>Y axis</b>.
     */
    inline matrix_2X3<real_t>& 
    make_translation(const real_t tx, 
                     const real_t ty);

    /*!
     * \brief Builds a translation matrix from a vector2 object.
     * \param[in] sv Two component vector, containing translation
     *          factors along the X and Y axes.
     */
    matrix_2X3<real_t>& 
    make_translation(const vector2<real_t>& sv) {
        return make_translation(sv.x_, sv.y_);
    }

    inline class_type& 
    make_reflection(const real_t vdir_x,
                    const real_t vdir_y);

    inline class_type& 
    make_reflection_line(const real_t org_x, 
                         const real_t org_y,
                         const real_t dir_x, 
                         const real_t dir_y);

    inline class_type& 
    make_reflection_point(const real_t pt_x,
                          const real_t pt_y);

    /*!
     * \brief Builds a reflection matrix around the <b>X axis</b>.
     */
    inline class_type& 
    make_reflection_x();

    /*!
     * \brief Builds a reflection matrix around the <b>Y axis</b>.
     */
    inline class_type& 
    make_reflection_y();

    /*!
     * \brief Builds a reflection matrix across a line that makes an angle
     *          of <b>theta radians</b> with the X axis.
     * \param[in] theta Angle <b>in radians</b> of the reflection line
     *          with the X axis.
     * \remarks The reflection matrix is build with the following formula :
     *          [ cos(2 * theta)    sin(2 * theta)  ]
     *          [ sin(2 * theta)    -cos(2 * theta) ]
     */          
    inline class_type& 
    make_reflection_from_angle_with_x(const real_t theta);

    /*!
     * \brief Builds a reflection matrix across a line with the
     *        specified slope.
     * \param[in] slope Slope of the reflection line.
     * \remarks The reflection matrix is build with the following formula :
     *          [ (1 - slope^2) / (1 + slope^2)     2 * slope / (1 + slope^2) ]
     *          [ 2 * slope / ( 1 + slope^2)    (slope^2 - 1) / (1 + slope^2) ]
     */          
    inline class_type& 
    make_reflection_slope(const real_t slope);

/*! @} */

/*! \name   Point and vector operations.
 *  @{
 */

public :

    /*!
     * \brief Transform a 2D vector (translation has no effect).
     * \param[in] vec Pointer to a 2D vector object.
     */
    template<typename real_u>
    inline matrix_2X3<real_t>& 
    transform_vector(vector2<real_u>* vec);

    /*!
     * \brief Transform a 2D point (applies translation).
     * \param[in] pt Pointer to a vector2 object, representing a point.
     */
    template<typename real_u>
    inline matrix_2X3<real_t>& 
    transform_point(vector2<real_u>* pt);

/*! @} */
}; 

template<typename real_t>
inline bool 
operator==(const matrix_2X3<real_t>& lhs,
           const matrix_2X3<real_t>& rhs);

template<typename real_t>
inline bool 
operator!=(const matrix_2X3<real_t>& lhs,
           const matrix_2X3<real_t>& rhs);

template<typename real_t>
inline matrix_2X3<real_t> 
operator+(const matrix_2X3<real_t>& lhs,
          const matrix_2X3<real_t>& rhs);

template<typename real_t>
inline matrix_2X3<real_t> 
operator-(const matrix_2X3<real_t>& lhs,
          const matrix_2X3<real_t>& rhs);

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> 
operator*(real_u                            scalar,
          const matrix_2X3<real_t>&         mtx);

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> 
operator*(const matrix_2X3<real_t>&     mtx,
          real_u                        scalar);

template<typename real_t>
matrix_2X3<real_t> 
operator*(const matrix_2X3<real_t>& lhs,
          const matrix_2X3<real_t>& rhs);

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> 
operator/(const matrix_2X3<real_t>&     mtx,
          real_u                        scalar);

/*! @} */

} // namespace math
} // namespace v8

#include "v8/math/matrix2X3.inl"
