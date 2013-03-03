#pragma once

#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/material.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/scene/scene_entity.hpp>
#include <v8/scene/fwd_scene_loading_info.hpp>
#include <v8/scene/fwd_scene_system.hpp>

namespace v8 { namespace scene {

class custom_entity : public v8::scene::scene_entity {
public :
    typedef v8::scene::scene_entity                   base_class;

    custom_entity();

    virtual ~custom_entity();

    virtual v8_bool_t initialize(
        const v8::scene::userdefined_object_loading_info_t& ent_info
        ) = 0;

    void draw(
        v8::scene::scene_system*,
        v8::rendering::renderer*
        );

protected :
    rendering::vertex_buffer                    vertexbuffer_;
    rendering::index_buffer                     indexbuffer_;
    rendering::PrimitiveTopology::Type          topology_;
    rendering::material*                        material_;

private :
    NO_CC_ASSIGN(custom_entity);
};

} // namespace scene
} // namespace v8
