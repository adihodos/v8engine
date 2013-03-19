#include "v8/global_state.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/rendering/effect_pass.hpp"
#include "v8/rendering/effect_technique.hpp"
#include "v8/utility/hash_string.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/render_assets_cache.hpp"
#include "v8/rendering/simple_mesh.hpp"
#include "v8/scene/scene_system.hpp"
#include "v8/scene/scene_loading_info.hpp"

#include "v8/scene/standard_entity.hpp"

v8::scene::standard_entity::standard_entity()
    :       base_class()
        ,   mesh_(nullptr)
        ,   material_()
{}

v8::scene::standard_entity::~standard_entity() {}

v8_bool_t v8::scene::standard_entity::initialize(
    const standard_object_loading_info_t& obj_info
    ) {
    if (mesh_) {
        return true;
    }
    if (!material_.initialize(hash_string(obj_info.material.c_str()))) {
        return false;
    }
    mesh_ = state->asset_cache()->get_mesh(hash_string(obj_info.mesh.c_str()));
    if (!mesh_) {
        return false;
    }

    world_transform_.set_scale_component(obj_info.scale);
    world_transform_.set_translation_component(
        v8::math::vector3F(obj_info.translate[0], obj_info.translate[1], 
                           obj_info.translate[2]));
    return true;
}

void v8::scene::standard_entity::draw(
    v8::scene::scene_system* scene_sys,
    v8::rendering::renderer* render_sys
    ) {
    assert(attached_effect_);
    assert(technique_);
    assert(mesh_);

    pre_draw(scene_sys, render_sys);

    //
    // Note that the input layout is set by each pass, in the apply call.
    mesh_->get_vertex_buffer()->bind_to_pipeline(render_sys);
    mesh_->get_index_buffer()->bind_to_pipeline(render_sys);
    render_sys->ia_stage_set_primitive_topology_type(mesh_->get_topology());

    for (v8_uint32_t pass_index = 0; pass_index < technique_->get_pass_count();
         ++pass_index) {
        rendering::effect_pass* current_pass = 
            technique_->get_pass_by_index(pass_index);
        current_pass->apply();
        render_sys->draw_indexed(mesh_->get_index_buffer()->get_element_count());
    }
    post_draw(scene_sys, render_sys);
}
