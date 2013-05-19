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
#include <v8/math/math_utils.hpp>
#include <v8/math/matrix4x4.hpp>

namespace v8 { namespace math {

/// \defgroup   __projections   Projection matrices.

/// \addtogroup __projections
/// @{    

///
/// \brief Helper class to build common projection matrices.
struct projection {

    struct type {
        enum {
            perspective,
            perspective_oblique,
            perspective_infinity,
            parallel_orthographic,
            parallel_oblique
        };
    };

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
    template<typename real_type>
    static matrix_4X4<real_type> perspective(
        const real_type aspect_ratio,
        const real_type vertical_field_of_view,
        const real_type near_plane,
        const real_type far_plane,
        const real_type ndc_min,
        const real_type ndc_max
        );

    ///
    /// \brief Makes a matrix for symmetric perspective projection to infinity.
    ///        (far plane = +inf).
    /// \param aspect_ratio Aspect ratio for the projection window.
    /// \param vertical_fov Vertical field of view angle, in radians.
    /// \param near_plane Near plane position.
    /// \param ndc_min         Minimum depth value in NDC coordinates.
    /// \param ndc_max         Maximum depth value in NDC coordinates.
    template<typename real_type>
    static matrix_4X4<real_type> perspective_infinity(
        const real_type aspect_ratio,
        const real_type vertical_fov,
        const real_type near_plane,
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
    template<typename real_type>
    static matrix_4X4<real_type> perspective_oblique(
        const real_type r_min,
        const real_type r_max,
        const real_type u_min,
        const real_type u_max,
        const real_type d_min,
        const real_type d_max,
        const real_type ndc_min, 
        const real_type ndc_max
        );

    ///
    /// \brief  Creates a general parallel orthographic projection matrix,
    ///         assumming a left handed coordinate system.
    /// \param r_min Projection window minimum value right direction (X axis)
    /// \param r_max Projection window maximum value right direction (X axis)
    /// \param u_min Projection window minumum value up direction (Y axis)
    /// \param u_max Projection window maximum value up direction (Y axis)
    /// \param d_min Near plane distance
    /// \param d_max Far plane distance
    /// \param ndc_min   Minimum depth value in NDC coordinates.
    /// \param ndc_max   Maximum depth value in NDC coordinates.
    template<typename real_type>
    static matrix_4X4<real_type> parallel_orthographic(
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

/// @}

} // namespace math
} // namespace v8

template<typename real_type>
v8::math::matrix_4X4<real_type>
v8::math::projection::perspective(
    const real_type aspect_ratio,
    const real_type vertical_field_of_view,
    const real_type near_plane,
    const real_type far_plane,
    const real_type ndc_min,
    const real_type ndc_max
    ) {
    matrix_4X4<real_type> proj_matrix;
    proj_matrix.make_zero();

    const real_type half_angle            = to_radians(vertical_field_of_view / 2);
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

template<typename real_type>
v8::math::matrix_4X4<real_type>
v8::math::projection::perspective_infinity(
    const real_type aspect_ratio,
    const real_type vertical_fov,
    const real_type near_plane,
    const real_type ndc_min,
    const real_type ndc_max
    ) {
    matrix_4X4<real_type> proj_mtx;
    proj_mtx.make_zero();

    const real_type half_angle = to_radians(vertical_fov / 2);
    const real_type d_val = real_type(1) / tan(half_angle);

    proj_mtx.a11_ = d_val / aspect_ratio;
    proj_mtx.a22_ = d_val;
    proj_mtx.a33_ = real_type(1);
    proj_mtx.a34_ = -(near_plane * (ndc_max - ndc_min));
    proj_mtx.a43_ = real_type(1);

    return proj_mtx;
}

template<typename real_type>
v8::math::matrix_4X4<real_type>
v8::math::projection::perspective_oblique(
    const real_type r_min,
    const real_type r_max,
    const real_type u_min,
    const real_type u_max,
    const real_type d_min,
    const real_type d_max,
    const real_type ndc_min, 
    const real_type ndc_max
    ) {
    matrix_4X4<real_type> proj_matrix;
    proj_matrix.make_zero();

    const real_type inv_r_diff = real_type(1) / (r_max - r_min);
    const real_type r_sum      = r_max + r_min;
    const real_type inv_u_diff = real_type(1) / (u_max - u_min);
    const real_type u_sum      = u_max + u_min;
    const real_type ndc_diff   = ndc_max - ndc_min;
    const real_type inv_depth  = real_type(1) / (d_max - d_min);

    proj_matrix.a11_ = real_type(2) * d_min * inv_r_diff;
    proj_matrix.a13_ = -(r_sum * inv_r_diff);

    proj_matrix.a22_ = real_type(2) * d_min * inv_u_diff;
    proj_matrix.a23_ = -(u_sum * inv_u_diff);

    proj_matrix.a33_ = ndc_min + d_max * ndc_diff * inv_depth;
    proj_matrix.a34_ = -(d_min * d_max * ndc_diff * inv_depth);

    proj_matrix.a43_ = real_type(1);

    return proj_matrix;
}

template<typename real_type>
v8::math::matrix_4X4<real_type>
v8::math::projection::parallel_orthographic(
    const real_type r_min,
    const real_type r_max,
    const real_type u_min,
    const real_type u_max,
    const real_type d_min,
    const real_type d_max,
    const real_type ndc_min, 
    const real_type ndc_max 
    ) {
    matrix_4X4<real_type> proj_matrix;
    proj_matrix.make_zero();

    const real_type inv_r_diff = real_type(1) / (r_max - r_min);
    const real_type r_sum = r_max + r_min;
    const real_type inv_u_diff = real_type(1) / (u_max - u_min);
    const real_type u_sum = u_max + u_min;
    const real_type ndc_diff = ndc_max - ndc_min;
    const real_type inv_depth = real_type(1) / (d_max - d_min);

    proj_matrix.a11_ = real_type(2) * inv_r_diff;
    proj_matrix.a14_ = -(r_sum * inv_r_diff);

    proj_matrix.a22_ = real_type(2) * inv_u_diff;
    proj_matrix.a24_ = -(u_sum * inv_u_diff);

    proj_matrix.a33_ = ndc_diff * inv_depth;
    proj_matrix.a34_ = ndc_min - ndc_diff * d_min * inv_depth;

    proj_matrix.a44_ = real_type(1);

    return proj_matrix;
}
