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

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/math/vector3.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/sampler_state.hpp>
#include <v8/rendering/texture.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_shader.hpp>

#include "igeometry_shape.hpp"

class F4Phantom : public IGeometryShape {

/// \name Initialization
/// @{

public :

    F4Phantom();

    ~F4Phantom();

    v8_bool_t Initialize(const InitContext* init_context);

private :

    /// \brief Resets aricraft parameters to their default values.
    void SetDefaultAicraftDefaultParameters();

/// @}


/// \name Updating, events and drawing.
/// @{

public :

    void Update(const float delta);

    void Draw(const DrawingContext* draw_context);

    void InputEvent(const v8::input_event& in_evt);

private :
    struct F4RotationData_t;

    void UpdateCorrdFrameAxisVectors(const F4RotationData_t& frame_data);

/// @}

/// \name Movement & rotation
/// @{

private :

    inline void MoveX(const float modifier);

    inline void MoveY(const float modifier);

    inline void MoveZ(const float modifier);

    /// \brief Rotates the aircraft around the Z axis.
    inline void Roll(const float modifier, F4RotationData_t* rot_data);

    /// \brief Rotates the aircraft around the X axis.
    inline void Pitch(const float modifier, F4RotationData_t* rot_data);

    /// \brief Rotates the aircraft around the Y axis.
    inline void Yaw(const float modifier, F4RotationData_t* rot_data);

/// @}

private :

    v8_bool_t is_valid() const {
        return is_valid_;
    }

private :

    ///
    /// \brief Parameters that control the flight characteristics of an aicraft.
    struct F4PhantomFlightCharacteristics_t {
        F4PhantomFlightCharacteristics_t() {
            SetDefaultValues();
        }

        float   roll_speed;
        float   yaw_speed;
        float   pitch_speed;

        void SetDefaultValues() {
            roll_speed = 0.003f;
            yaw_speed = 0.0015f;
            pitch_speed = 0.0045f;
        }
    };

    ///
    /// \brief  Contains the vectors that define the aircraft's orientation 
    /// and position in the world.
    struct F4PhantomCoordinateFrame_t {
        F4PhantomCoordinateFrame_t()
            :       axis_x(v8::math::vector3F::unit_x)
                ,   axis_y(v8::math::vector3F::unit_y)
                ,   axis_z(v8::math::vector3F::unit_z)
                ,   origin(v8::math::vector3F::zero)
        {}

        ///< Pitch is applied along this axis.
        v8::math::vector3F  axis_x;

        ///< Yaw is applied along this axis.
        v8::math::vector3F  axis_y;

        ///< Roll is applied along this axis.
        v8::math::vector3F  axis_z;

        ///< Position in the world.
        v8::math::vector3F  origin;

        void SetDefaultValues() {
            axis_x = v8::math::vector3F::unit_x;
            axis_y = v8::math::vector3F::unit_y;
            axis_z = v8::math::vector3F::unit_z;
            origin = v8::math::vector3F::zero;
        }
    };

    ///
    /// \brief  Stores information about the aircrafts state, at a given frame.
    struct F4RotationData_t {
        F4RotationData_t() {
            Reset();
        }

        void Reset() {
            memset(this, 0, sizeof(*this));
        }

        ///< Amount of rotation, along the Z axis (in radians).
        float       quant_roll;

        ///< Amount of rotation, along the X axis (in radians).
        float       quant_pitch;

        ///< Amount of rotation, along the Y axis (in radians).
        float       quant_yaw;

        ///< True if there was a rotation along the X axis.
        v8_bool_t   do_pitch;

        ///< True if there was a rotation along the Y axis.
        v8_bool_t   do_yaw;

        ///< True if there was a rotation along the Z axis.
        v8_bool_t   do_roll;
    };

private :
    v8::rendering::index_buffer         indexbuffer_;
    v8::rendering::vertex_buffer        vertexbuffer_;
    v8::rendering::vertex_shader        vertexshader_;
    v8::rendering::fragment_shader      fragshader_;
    v8::rendering::texture              mtl_diffuse_;
    v8::rendering::sampler_state        sam_state_;

    v8_bool_t                           keystates_[v8::input::Key_Sym_t::Last];
    v8_bool_t                           is_valid_;

    ///< Flight parameters.
    F4PhantomFlightCharacteristics_t    flight_characteristics_;

    ///< Data that defines the aircraft's orientation and position.
    F4PhantomCoordinateFrame_t          coord_frame_;

private :
    NO_CC_ASSIGN(F4Phantom);
};
