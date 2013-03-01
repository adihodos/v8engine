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

#include <utility>

#include <v8/math/matrix3X3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/math/vector3.hpp>

namespace v8 { namespace math {

template<typename real_t>
class transform {
private :
    vector3<real_t>                 translation_component_;
    real_t                          scale_factor_component_;
    matrix_3X3<real_t>              matrix_component_;
    mutable matrix_4X4<real_t>      transform_matrix_;
    mutable bool                    cache_valid_;
    bool                            is_identity_;
    bool                            is_rotation_reflection_;
    bool                            is_scale_;

    void compute_transform_matrix() const;

    void reset_row4() const;
    
public :
    //! \name Constructors.
    //! @{

    //! \brief Default constructor. Set the transform to the identity.
    transform();

    transform(const vector3<real_t>&);

    transform(const matrix_3X3<real_t>&, bool is_rotation_or_reflection = true);

    transform(float scale_factor);

    transform(
        const matrix_3X3<real_t>& matrix_component, 
        bool matrix_is_rotation_or_reflection,
        const vector3<real_t>& translation_component, 
        float scale_component
        );

    inline bool is_identity() const;

    inline bool is_rotation_or_reflection() const;

    inline bool is_scaling() const;

    transform<real_t>& make_identity();

    transform<real_t>& make_non_scaling();
    
    void set_scale_component(float scale_factor);

    inline real_t get_scale_component() const;

    void set_matrix_component_rotate_reflect(const math::matrix_3X3<real_t>&);

    void set_matrix_component(const math::matrix_3X3<real_t>&);

    inline const math::matrix_3X3<real_t>& get_matrix_component() const;

    void set_translation_component(const math::vector3<real_t>&);

    inline const math::vector3<real_t>& get_translation_component() const;

    const math::matrix_4X4<real_t>& get_transform_matrix() const;

    void compute_inverse(math::matrix_4X4<real_t>* inv) const;

    transform<real_t>& invert();

    transform<real_t>& operator*=(const math::transform<real_t>& rhs);

public :

    //! \name Vector transform functions.
    //! @{

public :

    //!
    //! \brief Applies a transformation to the input vector. Let V be the input
    //! vector. Then, V' = R * S * V.
    //! \param[in,out] vec Reference to a vector to be transformed.
    void transform_vector(
        vector3<real_t>& vec
        ) const {

        vector_tf_matrix_t tf_data(compute_vec_tf_matrix());

        if (tf_data.first) {
            //
            // Transform is identity, nothing to do.
            return;
        }

        vec = tf_data.second * vec;
    }

    //!
    //! \brief Applies the inverse of this transformation to the input vector.
    //! Let V be the input vector. Then, V' = RT * 1/S * V.
    //! \param[in,out] input_vec Reference to a vector to be transformed.
    void transform_inverse_vector(
        vector3<real_t>& input_vec
        ) const {

        vector_tf_matrix_t tf_data(compute_vec_tf_inverse_matrix());

        if (tf_data.first) {
            //
            // Transform is identity, nothing to do.
            return;
        }

        input_vec = tf_data.second * input_vec;
    }

    //!
    //! \brief Applies the transformation to each vector in the [first, last)
    //! range and writes the transformed vector to the destination.
    //! \param[in] first Pointer to the first element in the input sequence.
    //! \param[in] last Pointer to one beyond the last element in the input
    //! sequence.
    //! \note This code assumes the the output sequence can store at least
    //! (last - first) elements.
    void transform_vector_sequence(
        const vector3<real_t>* first,
        const vector3<real_t>* last,
        vector3<real_t>* out
        ) {

        vector_tf_matrix_t tf_data(compute_vec_tf_matrix());

        while (first != last) {

            if (tf_data.first) {

                *out = *first;

            } else {

                *out = tf_data.second * (*first);

            }

            ++out;
            ++first;
        }
    }

    //!
    //! \brief Applies the transformation to each vector in the [first, last)
    //! range and writes the transformed vector to the destination.
    //! \param[in] first Pointer to the first element in the input sequence.
    //! \param[in] last Pointer to one beyond the last element in the input
    //! sequence.
    //! \note This code assumes the the output sequence can store at least
    //! (last - first) elements.
    void transform_inverse_vector_sequence(
        const vector3<real_t>* first,
        const vector3<real_t>* last,
        vector3<real_t>* out
        ) {

        vector_tf_matrix_t tf_data(compute_vec_tf_inverse_matrix());

        while (first != last) {

            if (tf_data.first) {

                *out = *first;

            } else {

                *out = tf_data.second * (*first);

            }

            ++out;
            ++first;
        }
    }

private :

    //!
    //! \brief A boolean + matrix3 pair. The boolean member is set to true
    //! if the matrix represents the identity matrix.
    typedef std::pair<bool, v8::math::matrix_3X3<real_t>> vector_tf_matrix_t;

    vector_tf_matrix_t compute_vec_tf_matrix() const {
        vector_tf_matrix_t tf_matrix;
        tf_matrix.first = false;

        if (is_identity()) {
            tf_matrix.first = false;
        } else {
            tf_matrix.second = matrix_component_;

            if (is_scaling()) {
                tf_matrix.second *= scale_factor_component_;
            }
        }

        return tf_matrix;
    }

    vector_tf_matrix_t compute_vec_tf_inverse_matrix() const {
        vector_tf_matrix_t tf_matrix;
        tf_matrix.first = false;

        if (is_identity()) {
            tf_matrix.first = false;
        } else {

            if (is_rotation_or_reflection()) {
                matrix_component_.get_transpose(&tf_matrix.second);
            } else {
                matrix_component_.get_inverse(&tf_matrix.second);
            }

            if (is_scaling()) {
                tf_matrix.second /= scale_factor_component_;
            }
        }

        return tf_matrix;
    }

    //! @}

public :
};

template<typename real_t>
inline
math::transform<real_t>
operator*(const math::transform<real_t>&, const math::transform<real_t>&);

typedef transform<float>    transformF;
typedef transform<double>   transformD;

} // namespace math
} // namespace v8

#include "transform.inl"
