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
#include <cstdint>

#include <v8/math/camera.hpp>
#include <v8/math/objects/plane.hpp>

namespace v8 { namespace math {

class bounded_volume;
class camera;

/** \addtogroup Culling
 *  @{
 */

/**
 * \brief Supports culling of objects in relation to the view frustrum 
 *      and user defined culling planes. Before using this class, a camera
 *      must be set, as the culling process will always test 
 *      the frustrum planes. Besides the six frustrum planes, a user can add
 *      26 aditional culling planes.
 */
class culler {
private :
    static const uint32_t C_Max_Cull_Planes = 32;

    /*!
     * \brief Camera used for culling.
     */
    const camera*   cam_;
    /*!
     * \brief Copy of the camera's frustrum parameters.
     */
    float           frustrum_params_[6];
    /*!
     * \brief Array of culling planes. The first 6 planes 
     * 		  are the frustrum planes. The remaining 26 planes
     * 		  are user defined.
     */
    plane3F         cull_plane_stack_[C_Max_Cull_Planes];   
    /*!
     * \brief Bitmask to keep track of active culling planes. The first
     * 		  six planes (the frustrum planes) are always active.
     */
    uint32_t        active_planes_;
    /*!
     * \brief Count of culling planes.
     */
    uint32_t        plane_count_;

public :

    /**
     * \brief   Default constructor. Before using any object of this class,
     * 			make sure to call set_camera.
     */
    culler();

    /**
     * \brief   Culls the given bound object.
     * \param   bound_object    Pointer to a bound object.
     * \return  True if the object can be culled, false otherwise.
     */
    bool cull(const bounded_volume* bound_object) const;

    /**
     * \brief   Toggle a plane's state (active/inactive).
     * \param   plane_id    Identifier for the plane. Must be > 6.
     * \param   status      Status bit (true = on, false = off)
     */
    void toggle_plane_state(int plane_id, bool status);

    /**
     * \brief   Sets a plane's data.
     * \param   plane_id    Identifier for the plane. 
     * 						Must be a value in the [6, 31] range.
     * \param   user_plane  Plane's data.
     */
    void set_plane(uint32_t plane_id, const plane3F& user_plane) {
        assert(plane_id > 5U && plane_id < C_Max_Cull_Planes);
        cull_plane_stack_[plane_id] = user_plane;
        active_planes_ |= (1 << plane_id);
    }

    /**
     * \brief   Adds a user defined plane. 
     * 			The plane's state is set to active.
     */
    void push_plane(const plane3F& new_plane) {
        set_plane(plane_count_, new_plane);
        ++plane_count_;
    }

    /**
     * \brief   Removes the last user defined plane. 
     * 			The frustrum planes cannot be removed.
     */
    void pop_plane();

    /**
     * \brief   Sets the camera that will be used for culling. A camera must
     * 			be set before any culling can take place.
     * \param   cam Pointer to a camera. Must not be null.
     */
    void set_camera(const camera* cam);

    /**
     * \brief   Removes all user defined planes. After the call, 
     * 			only the frustrum planes remain active.
     */
    void clear() {
        active_planes_ = 0x3F;
        plane_count_ = 6;
    }
};

/** @} */

} // namespace math
} // namespace v8
