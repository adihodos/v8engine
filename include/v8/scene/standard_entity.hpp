#pragma once

#include <v8/v8.hpp>
#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_effect.hpp>
#include <v8/rendering/fwd_effect_technique.hpp>
#include <v8/rendering/material.hpp>
#include <v8/rendering/simple_mesh.hpp>
#include <v8/scene/fwd_scene_loading_info.hpp>
#include <v8/scene/fwd_scene_system.hpp>
#include <v8/scene/scene_entity.hpp>

namespace v8 { namespace scene {

class standard_entity : public scene_entity {
/// \name Construction/initialization
/// @{

public :

    typedef scene_entity                                            base_class;

    standard_entity();

    virtual ~standard_entity();

    virtual v8_bool_t initialize(const standard_object_loading_info_t& obj_info);

/// @}

/// \name Operations.
/// @{

public :

    virtual void draw(v8::scene::scene_system*, v8::rendering::renderer*);
/// @}

protected :
    rendering::simple_mesh*                                     mesh_;
    rendering::material                                         material_;
};

} // namespace scene
} // namespace v8
