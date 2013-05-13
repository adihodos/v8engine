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

///
/// \file projection.hpp
/// \brief Helper routines for generating projection matrices.

#include <v8/v8.hpp>
#include <v8/math/matrix4x4.hpp>

namespace v8 { namespace math {

/// \addtogroup algebra
/// @{    

///
/// \brief Helper class to build common projection matrices.
template<typename real_type>
struct projection {

    enum type {
        kPerspective,
        kPerspectiveOblique,
        kOrthographic,
        kOrthographicOblique
    };

    typedef matrix_4X4<real_type>                               matrix_type;

    ///
    /// \brief  Creates a symmetric perspective projection matrix, assuming a
    ///         left handed coordinate system. The projection window is centered
    ///         around the Z axis and the direction of projection through the
    ///         center of the projection window is orthogonal to the projection 
    ///         plane containing the window.
    /// \param  aspect_ratio    The aspect ratio of the window (width/height).
    /// \param  vertical_field_of_view  Angle of view in the vertical plane,
    ///                                 in radians.
    /// \param  near_plane      Distance to the near plane.
    /// \param  far_plane       Distance to the far plane.
    /// \param  ndc_min         Minimum depth value in NDC space.
    /// \param  ndc_max         Maximum depth value in NDC space.
    static matrix_type perspective(
        const real_type aspect_ratio,
        const real_type vertical_field_of_view,
        const real_type near_plane,
        const real_type far_plane,
        const real_type ndc_min,
        const real_type ndc_max
        );

    ///
    /// \brief  Creates a general perspective projection matrix, assuming a left
    ///         handed coordinate system. The projection window may not be centered
    ///         around the viewing direction and the direction of projection
    ///         through the center of the projection window is not necessarily
    ///         orthogonal to the projection plane.
    /// \param r_min    Projection window minimum value right direction (X axis)
    /// \param r_max    Projection window maximum value right direction (X axis)
    /// \param u_min    Projection window minumum value up direction (Y axis)
    /// \param u_max    Projection window maximum value up direction (Y axis)
    /// \param d_min    Near plane distance
    /// \param d_max    Far plane distance
    /// \param ndc_min  Minimum depth value in NDC coordinates.
    /// \param ndc_max  Maximum depth value in NDC coordinates.
    static matrix_type perspective_oblique(
        const real_type r_min,
        const real_type r_max,
        const real_type u_min,
        const real_type u_max,
        const real_type d_min,
        const real_type d_max,
        const real_type ndc_min, 
        const real_type ndc_max
        );
};    

typedef projection<float>                           projectionF;
typedef projection<double>                          projectionD;

/// @}

template<typename real_type>
typename projection<real_type>::matrix_type 
projection<real_type>::perspective(
    const real_type aspect_ratio,
    const real_type vertical_field_of_view,
    const real_type near_plane,
    const real_type far_plane,
    const real_type ndc_min,
    const real_type ndc_max
    ) {
    matrix_type proj_matrix;
    proj_matrix.make_zero();

    const real_type half_angle            = vertical_field_of_view / 2;
    const real_type distance_to_viewplane = real_type(1) / tan(half_angle);
    const real_type ndc_diff              = ndc_max - ndc_min;
    const real_type inv_depth             = real_type(1) / (far_plane - near_plane);

    proj_matrix.a11_ = distance_to_viewplane / aspect_ratio;
    proj_matrix.a22_ = distance_to_viewplane;
    proj_matrix.a33_ = ndc_min + far_plane * ndc_diff * inv_depth;
    proj_matrix.a34_ = -(near_plane * far_plane * ndc_diff * inv_depth);
    proj_matrix.a43_ = real_type(1);

    return proj_matrix;
}

template<typename real_t>
typename projection<real_t>::matrix_type
projection<real_t>::perspective_oblique(
    const real_t r_min,
    const real_t r_max,
    const real_t u_min,
    const real_t u_max,
    const real_t d_min,
    const real_t d_max,
    const real_t ndc_min, 
    const real_t ndc_max
    ) {
    matrix_type proj_matrix;
    proj_matrix.make_zero();

    const real_t inv_r_diff = real_t(1) / (r_max - r_min);
    const real_t r_sum      = r_max + r_min;
    const real_t inv_u_diff = real_t(1) / (u_max - u_min);
    const real_t u_sum      = u_max + u_min;
    const real_t ndc_diff   = ndc_max - ndc_min;
    const real_t inv_depth  = real_t(1) / (d_max - d_min);

    proj_matrix.a11_ = real_t(2) * d_min * inv_r_diff;
    proj_matrix.a13_ = -(r_sum * inv_r_diff);

    proj_matrix.a22_ = real_t(2) * d_min * inv_u_diff;
    proj_matrix.a23_ = -(u_sum * inv_u_diff);

    proj_matrix.a33_ = ndc_min + d_max * ndc_diff * inv_depth;
    proj_matrix.a34_ = -(d_min * d_max * ndc_diff * inv_depth);

    proj_matrix.a43_ = real_type(1);

    return proj_matrix;
}

} // namespace v8
} // namespace math