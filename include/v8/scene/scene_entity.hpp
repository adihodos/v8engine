#pragma once

#include <v8/v8.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/transform.hpp>

#include <v8/rendering/fwd_renderer.hpp>
#include <v8/rendering/fwd_effect.hpp>
#include <v8/rendering/fwd_effect_technique.hpp>
#include <v8/scene/fwd_scene_system.hpp>

namespace v8 { namespace scene {

class scene_entity {
/// \name Construction/Initialization.
/// @{

public :

    scene_entity()
        :   attached_effect_(nullptr),
            technique_(nullptr)
    {}

    virtual ~scene_entity() {}

/// @}

/// \name Operations.
/// @{

public :
    virtual void pre_draw(
        v8::scene::scene_system*,
        v8::rendering::renderer*
        ) = 0;

    virtual void draw(
        v8::scene::scene_system*,
        v8::rendering::renderer*
        ) = 0;

    virtual void post_draw(
        v8::scene::scene_system*,
        v8::rendering::renderer*
        ) = 0;

    virtual void update(
        float delta
        ) = 0;

/// @}

/// \name Attributes.
/// @{

public :

    void attach_effect(
        rendering::effect* render_effect,
        const char* technique_name
        );

    rendering::effect* get_attached_effect() const {
        return attached_effect_;
    }

    rendering::effect_technique* get_active_technique() const {
        return technique_;
    }

    void set_active_technique(
        const char* technique_name
        );

    const v8::math::transformF& get_world_transform() const {
        return world_transform_;
    }

/// @}

/// \name Members.
/// @{

protected :

    ///< Every entity has an effect attached to it that will be used
    ///< when it is rendered.
    rendering::effect*                                      attached_effect_;

    ///< The technique to use when rendering the entity.
    rendering::effect_technique*                            technique_;

    ///< Stores the world transform for this entity.
    v8::math::transformF                                        world_transform_;

/// @}
};

} // namespace scene
} // namespace v8
