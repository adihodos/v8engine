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
#include <cassert>
#include <cmath>

#include <v8/base/count_of.hpp>
#include <v8/base/copy_pod_range.hpp>
#include <v8/base/fundamental_types.hpp>
#include <v8/math/math_utils.hpp>
#include <v8/math/matrix3X3.hpp>
#include <v8/math/vector4.hpp>

namespace v8 { namespace math {

/** \addtogroup __grp_v8_math_algebra
 *  @{
 */

/**
 * \brief   A 4x4 matrix, stored using in row major format.
 * \remarks When concatenating matrices that represent 
 *          a sequence of transformations, care must be taken so
 *          that the final matrix is built properly. Let M0, M1, ... Mn-1, Mn
 *          be a matrices representing a sequence of transormations to be applied
 *          in this order. Then the proper order of concatenating them into a
 *          single matrix is T = Mn * Mn - 1 * ... * M2 * M1 * M0 (right to left).
 *          The matrix follows the convention that it multiplies column vectors 
 *          (the vector is on the right side). Given a matrix \a M 
 *          and a vector \a V, to transform the vector \a V by the matrix \a M, 
 *          one would write \a V1 = \a M * \a V. 
 *          Note that access to individual elements using the function call 
 *          operator syntax uses 1 based indices.
 */
template<typename real_t>
class matrix_4X4 {
private :

  int index_at(int row, int col) const {
    return (row - 1) * 4 + col - 1;
  }

public :
    enum { 
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    /** The type of elements stored by the matrix. */
    typedef real_t              element_type;

    /** Type of reference to an element. */
    typedef real_t&             reference;

    /** Type of reference to a const element. */
    typedef const real_t&       const_reference;

    /** Fully qualified type of this class. */
    typedef matrix_4X4<real_t>   matrix4X4_t;

    union {
        struct {
            real_t a11_, a12_, a13_, a14_;
            real_t a21_, a22_, a23_, a24_;
            real_t a31_, a32_, a33_, a34_;
            real_t a41_, a42_, a43_, a44_;
        };
        real_t elements_[16];	/*!< Access to elements using an array */
    };

    static const matrix_4X4<real_t>	null;   /*!< The null matrix */

    static const matrix_4X4<real_t>	identity;   /*!< The identity matrix */

    /** \brief Default constructor. Leaves elements uninitialized. */
    matrix_4X4() {}

    matrix_4X4(
        real_t a11, real_t a12, real_t a13, real_t a14,
        real_t a21, real_t a22, real_t a23, real_t a24,
        real_t a31, real_t a32, real_t a33, real_t a34,
        real_t a41, real_t a42, real_t a43, real_t a44
        );

    /**
     * \brief Constructs from an array of real_t values.
     * \param input   Pointer to an array of real_t values.
     * \param count   Number of entries in the array.
     * \remarks If the array has less than 16 elements, the remaining matrix
     *          components will be left uninitialized. Values beyond the 16th
     *          element of the array will be ignored.
     */
    matrix_4X4(const real_t* input, size_t count) {
        std::memcpy(elements_, 
                    input, min((size_t)16, count) * sizeof(real_t));
    }

    template<typename convertible_type>
    matrix_4X4(const matrix_4X4<convertible_type>& rhs) {
        base::copy_pod_range(rhs.elements_, 16, elements_);
    }

    template<typename convertible_type>
    matrix_4X4(const convertible_type (&arr)[16]) {
        base::copy_pod_range(&arr[0], 16, elements_);
    }

    template<typename convertible_type>
    matrix_4X4(const matrix_3X3<convertible_type>& linear_tf);

    /** \brief   Constructs a diagonal matrix. */
    matrix_4X4(real_t a11, real_t a22, real_t a33, real_t a44 = real_t(1));

    /**
     * \brief   Construct from 4 vectors.
     * \param column True if vectors represent columns, false if they represent
     *               rows.
     */
    matrix_4X4(
        const math::vector4<real_t>& v1, 
        const math::vector4<real_t>& v2,
        const math::vector4<real_t>& v3,
        const math::vector4<real_t>& v4,
        bool column = true
        );

    /**
     * \brief Component read-write access, using the [row][column] syntax.
     * \param row Component's row index (1 based).
     * \param col Component's column index (1 based).
     */
    real_t& operator()(int row, int col) {
        return elements_[index_at(row, col)];
    }

    /**
     * \brief Component read only access, using the [row][column] syntax.
     * \param row Component's row index (1 based).
     * \param col Component's column index (1 based).
     */
    real_t operator()(int row, int col) const {
        return elements_[index_at(row, col)];
    }

    /** \brief Addition assignment operator. */
    matrix_4X4<real_t>& operator+=(const matrix_4X4<real_t>& rhs);

    /** \brief Subtraction assignment operator. */
    matrix_4X4<real_t>& operator-=(const matrix_4X4<real_t>& rhs);

    /** 
     * \brief Multiplication assignment operator.
     * \param k   Scalar to multiply with.
     */
    matrix_4X4<real_t>& operator*=(real_t k);

    /**
     * \brief Division assignment operator.
     * \param k   The scalar to divide with. Ensure k != 0.
     */
    matrix_4X4<real_t>& operator/=(real_t k);

    /**
     * \brief Computes the determinant for this matrix.
     */
    real_t determinant() const;

    /**
     * \brief Query if this matrix4X4 is invertible (that is det(A) is not 0).
     * \return    true if matris is invertible, false if not.
     */
    bool is_invertible() const {
        return !math::operands_eq(real_t(0), determinant());
    }

    /**
     * \brief   Inverts the matrix.
     * \remarks Only call this function if is_invertible() returns true.		
     * \return  Reference to the matrix object.
     */
    matrix_4X4<real_t>& invert();

    /**
     * \brief   Stores the inverse of this matrix into the the input matrix.
     * \sa      \link #invert
     */
    inline void get_inverse(math::matrix_4X4<real_t>* mx) const;

    /**
     * \brief Transposes the matrix.
     */
    matrix_4X4<real_t>& transpose();

    /**
     * \brief Copies the elements of transpose of the matrix into the
     *        specified matrix.
     */
    void get_transpose(math::matrix_4X4<real_t>* mx) const;

    /** \brief Copies the adjoint of the matrix into the specified matrix. */
    void get_adjoint(math::matrix_4X4<real_t>* mx) const;

   /**
    * \brief Copies the values from an array into the upper 3x3 matrix.
    * \param data    Pointer to an array of real_t values. Cannot be null.
    * 				Must contain at least 9 elements.
    */
    template<typename real_u>
    matrix_4X4<real_t>& set_upper3x3(const real_u* data);

   /**
    * \brief Sets the contents of the upper 3x3 matrix.
    * \param src Source 3x3 matrix for the assignment
    */
    template<typename real_u>
    matrix_4X4<real_t>& set_upper3x3(const matrix_3X3<real_u>& src) {
        return set_upper3x3(src.elements_);
    }

   /**
    * \brief Gets the contents of the upper 3x3 matrix.
    * \param [in,out]    data    Pointer to an array of real_t values, where the
    * 					data is to be copied. Cannot be null. 
    * 					Must have at least 9 elements.
    */
    template<typename real_u>
    void get_upper3x3(real_u* data) const;

   /**
    * \brief Gets the contents of the upper 3x3 matrix.
    * \param [in,out]    out_mtx Pointer to a 3x3 matrix that receives the data.
    */
    template<typename real_u>
    void get_upper3x3(matrix_3X3<real_u>* out_mtx) const {
        get_upper3x3(out_mtx->elements_);
    }

    /**
     * \brief Sets a row's contents to the specified values.
     * \param row Row index (1 based)
     */
    matrix_4X4<real_t>& set_row(
        int row, real_t r1, real_t r2, real_t r3, real_t r4
        );

   /**
    * \brief Sets a row's contents.
    * \param row         The row index (1 based).
    * \param row_data    Pointer to source data.
    */
    matrix_4X4<real_t>& set_row(int row, const real_t* row_data);

   /**
    * \brief Assigns the contents of a vector4 to a matrix row.
    * \param row The row index (1 based).
    * \param vec Source vector for the data.
    */
    matrix_4X4<real_t>& set_row(int row, const vector4<real_t>& vec) {
        return set_row(row, vec.elements_);
    }

    /**
     * \brief Sets the contents of a column equal to the specified values.
     * \param column Column index, 1 based.
     */
    matrix_4X4<real_t>& set_column(
        int column, real_t c1, real_t c2, real_t c3, real_t c4
        );

   /**
    * \brief Sets a column's data.
    * \param column  The column index (1 based).
    * \param data    Pointer to source data.
    */
    matrix_4X4<real_t>& set_column(int column, const real_t* data);

   /**
    * \brief Assigns the contents of a vector4 to a matrix column.
    * \param column  The column index (1 based).
    * \param v4      Source vector.
    */
    matrix_4X4<real_t>& set_column(int column, const vector4<real_t>& v4) {
        return set_column(column, v4.elements_);
    }

   /**
    * \brief Copies the contents of a row.
    * \param row                 The row index (1 based).
    * \param [in,out]    data    Pointer to a real_t array that will receive the
    * 					row's data. The array size must be at least 4. 
    * 					Can not be null.
    */
    void get_row(int row, real_t* data) const;

   /**
    * \brief Copies the contents of a row.
    * \param row             The row index (1 based).
    * \param [in,out]    v4  Pointer to a vector4 object that will receive the 
    * 					    row's contents. Must not be null.
    */
    void get_row(int row, vector4<real_t>* v4) const {
        return get_row(row, v4->elements_);
    }

   /**
    * \brief Copies the contents of a column into an array.
    * \param col                 The column index (1 based).
    * \param [in,out]    data    Pointer to a real_t array that will receive the
    * 					column's data. The array size must be at least 4.
    * 					Can not be null.
    */
    inline void get_column(int col, real_t* data) const;

   /**
    * \brief Copies the contents of a column into a vector4 object.
    * \param col             The column index (1 based).
    * \param [in,out]    v4  Pointer to a vector4 object. Cannot be null.
    */
    void get_column(int col, vector4<real_t>* v4) const {
        get_column(col, v4->elements_);
    }

    template<typename R2>
    const matrix_4X4<real_t>& transform_affine_vector(vector3<R2>* pvec) const;

    template<typename R2>
    const matrix_4X4<real_t>& transform_affine_point(vector3<R2>* point) const;

   /**
    * \brief Transforms an affine (w component is 0) 
    * 		 vector and assigns the result to it.
    * \param [in,out]    pvec    Pointer to the vector4 to be transformed. 
    * 					Must not be null.
    */
    const matrix_4X4<real_t>& transform_affine_vector(
        vector4<real_t>* pvec
        ) const;

    /**
     * \brief   Transforms an affine point (w component is 1) and assigns 
     * 			the result to it.
     * \param [in,out]  point   If non-null, the point.
     */
    const matrix_4X4<real_t>& transform_affine_point(
        vector4<real_t>* point
        ) const;

    /**
     * \brief   Transforms a homogeneous point 
     * 			(w component may be different than 1).
     * \param [in,out]  point   The point.
     */
    const matrix_4X4<real_t>& transform_homogeneous_point(
        vector4<real_t>* point
        ) const;

    /**
     * \brief   Makes a symmetric perspective projection matrix,
     *          assuming a right handed coordinate system. The projection window
     *          is centered around the Z axis and the direction of projection
     *          through the center of the projection window is orthogonal to
     *          it.
     * \param   aspect_ratio    The aspect ratio of the window.
     * \param   vertical_fov    The vertical field of view angle, in radians.
     * \param   near_plane      The near plane.
     * \param   far_plane       The far plane.
     * \param   ndc_min         Minimum depth value in NDC coordinates.
     * \param   ndc_max         Maximum depth value in NDC coordinates.
     */
    matrix_4X4<real_t>& make_symmetric_perspective_projection_rh(
        real_t aspect_ratio,
        real_t vertical_fov,
        real_t d_min,
        real_t d_max,
        real_t ndc_min,
        real_t ndc_max
        );

    /**
     * \brief Makes a matrix for symmetric perspective projection to infinity.
     *        (far plane = +inf).
     * \param aspect_ratio Aspect ratio for the projection window.
     * \param vertical_fov Vertical field of view angle, in radians.
     * \param d_min Near plane position.
     * \param ndc_min         Minimum depth value in NDC coordinates.
     * \param ndc_max         Maximum depth value in NDC coordinates.
     */
    matrix_4X4<real_t>& make_symmetric_perspective_infinity_rh(
        real_t aspect_ratio,
        real_t vertical_fov,
        real_t d_min,
        real_t ndc_min,
        real_t ndc_max
        );

    /**
     * \brief Makes a generic perspective projection matrix, assuming a right handed
     *        coordinate system. It is not necessary that the projection window
     *        be centered around the Z axis, nor that the direction of projection
     *        to the window's center is orthogonal to it. Thus, it can be used
     *        to generate oblique perspective projections.
     * \param r_min Projection window minimum value right direction (X axis)
     * \param r_max Projection window maximum value right direction (X axis)
     * \param u_min Projection window minumum value up direction (Y axis)
     * \param u_max Projection window maximum value up direction (Y axis)
     * \param d_min Near plane distance
     * \param d_max Far plane distance
     * \param ndc_min   Minimum depth value in NDC coordinates.
     * \param ndc_max   Maximum depth value in NDC coordinates.
     */
    matrix_4X4<real_t>& make_perspective_projection_rh(
        real_t r_min,
        real_t r_max,
        real_t u_min,
        real_t u_max,
        real_t d_min,
        real_t d_max,
        real_t ndc_min,
        real_t ndc_max
        );

    /**
     * \brief Sets the matrix contents equal to zero.
     */
    inline matrix_4X4<real_t>& make_zero();

    /**
     * \brief Sets the matrix contents to the identity matrix.
     */
    inline matrix_4X4<real_t>& make_identity();
};

template<typename real_t>
const math::matrix_4X4<real_t>
math::matrix_4X4<real_t>::null(
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0)
    );

template<typename real_t>
const math::matrix_4X4<real_t>
    math::matrix_4X4<real_t>::identity(
    real_t(1), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(1), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(1), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(1)
    );

template<typename real_t>
bool
operator==(
    const math::matrix_4X4<real_t>& lhs, const math::matrix_4X4<real_t>& rhs
        );

template<typename real_t>
inline
bool
operator!=(
    const math::matrix_4X4<real_t>& lhs, const math::matrix_4X4<real_t>& rhs
        );

/**
 * \brief   Addition operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator+(
    const math::matrix_4X4<real_t>& lhs, const math::matrix_4X4<real_t>& rhs
    );

/**
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator-(
    const math::matrix_4X4<real_t>& lhs, const math::matrix_4X4<real_t>& rhs
    );

/**
 * \brief   Negation operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator-(const math::matrix_4X4<real_t>& mtx);

/**
 * \brief   Multiplication operator.
 */
template<typename real_t>
matrix_4X4<real_t>
operator*(
    const math::matrix_4X4<real_t>& lhs, const math::matrix_4X4<real_t>& rhs
    );

/**
 * \brief   scalar -> matrix multiplication operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator*(float k, const math::matrix_4X4<real_t>& mtx
    );

/**
 * \brief   matrix -> scalar multiplication operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator*(const math::matrix_4X4<real_t>& mtx, real_t k);

/**
 * \brief   matrix -> scalar division operator.
 */
template<typename real_t>
inline
math::matrix_4X4<real_t>
operator/(const math::matrix_4X4<real_t>& mtx, real_t k);

/**
 * \brief matrix_4X4F 4x4 matrix, simple precision floating point components.
 */
typedef matrix_4X4<float>    matrix_4X4F;

/**
 * \brief matrix_4X4D 4x4 matrix, double precision floating point components.
 */
typedef matrix_4X4<double>   matrix_4X4D;

/** @} */

} // namespace math
} // namespace v8

#include "matrix4X4.inl"
