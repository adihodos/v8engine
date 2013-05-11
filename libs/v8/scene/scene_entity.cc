#include "v8/rendering/effect.hpp"
#include "v8/scene/scene_entity.hpp"

void v8::scene::scene_entity::attach_effect(
    rendering::effect* render_effect,
    const char* technique_name
    ) {
    assert(render_effect);
    assert(technique_name);
    attached_effect_ = render_effect;
    set_active_technique(technique_name);
}

void v8::scene::scene_entity::set_active_technique(
    const char* technique_name
    ) {
    assert(attached_effect_);
    assert(technique_name);
    /*technique_ = attached_effect_->get_technique_by_name(technique_name);*/
    assert(technique_);
}