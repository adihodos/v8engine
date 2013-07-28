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

#include <v8/v8.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/math/color.hpp>

namespace v8 { namespace rendering {

///
/// \brief Parameters for rrender device initialization.
struct renderOptions_t {

    renderOptions_t(void*                           output_window,
                    const v8_int_t                  width = 1024,
                    const v8_int_t                  height = 1024,
                    const v8_bool_t                 use_hardware_accel = true,
                    const v8_bool_t                 full_screen = false,
                    const v8_bool_t                 aa_enabed = false,
                    const v8_int_t                  buff_element_type = ElementType::Unorm8,
                    const v8_int_t                  buff_element_count = 4,
                    const v8_int_t                  num_render_targets = 1,
                    const v8_int_t                  depth_bits = 24,
                    const v8_int_t                  stencil_bits = 8,
                    const v8::math::color_rgb&      clear_color = v8::math::color_rgb::C_OrangeRed)
        :       OutputWindow(output_window)
            ,   Width(width)
            ,   Height(height)
            ,   UseHardwareAcceleration(use_hardware_accel)
            ,   FullScreen(full_screen)
            ,   EnableAntialiasing(aa_enabed)
            ,   BufferElementType(buff_element_type)
            ,   BufferElementCount(buff_element_count)
            ,   RenderTargetsCount(num_render_targets)
            ,   DepthBits(depth_bits)
            ,   StencilBits(stencil_bits)
            ,   ClearColor(clear_color)
    {}

    ///< Destination output window.
    void*       OutputWindow;
    
    ///< Window width.
    v8_int_t    Width;

    ///< Window height.
    v8_int_t    Height;

    ///< True if using hardware acceleration.
    v8_bool_t   UseHardwareAcceleration;

    ///< True if rendering full screen.
    v8_bool_t   FullScreen;

    ///< Enable support for antialiasing (not yet implemented).
    v8_bool_t   EnableAntialiasing;

    ///< Element type for the back buffer (see ElementType).
    v8_int_t    BufferElementType;

    ///< Number of components of a single backbuffer entry.
    v8_int_t    BufferElementCount;

    ///< Number of render targets (only 1 atm).
    v8_int_t    RenderTargetsCount;

    ///< Number of bits for depth testing.
    v8_int_t    DepthBits;

    ///< Number of bits for stenciling.
    v8_int_t    StencilBits;

    ///< Backbuffer clear color.
    math::color_rgb ClearColor;
};

} // namespace rendering
} // namespace v8
