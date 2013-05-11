#include "v8/rendering/renderer.hpp"
#include "v8/rendering/effect.hpp"
#include "v8/rendering/effect_pass.hpp"
#include <v8/scene/scene_system.hpp>

#include "v8/scene/custom_entity.hpp"

v8::scene::custom_entity::custom_entity()
    :       base_class()
        ,   topology_(v8::rendering::PrimitiveTopology::Undefined)
{}

v8::scene::custom_entity::~custom_entity() 
{}

void v8::scene::custom_entity::draw(
    v8::scene::scene_system* scene_mgr,
    v8::rendering::renderer* render_sys
    ) {
    assert(attached_effect_);
    assert(technique_);
    assert(vertexbuffer_);
    assert(indexbuffer_);

    pre_draw(scene_mgr, render_sys);

    vertexbuffer_.bind_to_pipeline(render_sys);
    indexbuffer_.bind_to_pipeline(render_sys);
    material_.bind_to_effect(attached_effect_);

    render_sys->ia_stage_set_primitive_topology_type(topology_);

    using namespace v8;

    assert(false);
    //for (v8_uint32_t pass_index = 0; pass_index < technique_->get_pass_count();
    //     ++pass_index) {
    //    rendering::effect_pass* current_pass = technique_->get_pass_by_index(pass_index);
    //    current_pass->apply();
    //    render_sys->draw_indexed(indexbuffer_.get_element_count());
    //}
    post_draw(scene_mgr, render_sys);
}