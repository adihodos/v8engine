#include "v8/global_state.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/rendering/effect_pass.hpp"
#include "v8/rendering/effect_technique.hpp"
#include "v8/utility/hash_string.hpp"
#include "v8/rendering/renderer.hpp"
#include "v8/rendering/simple_mesh.hpp"
#include "v8/scene/scene_system.hpp"
#include "v8/scene/scene_loading_info.hpp"

#include "v8/scene/geometric_entity.hpp"

v8_bool_t v8::scene::geometric_entity::initialize(
    const standard_object_loading_info_t& obj_info
    ) {
    assert(!mat_ptr_);
    assert(false && "Not implemented!");

    /*
    mat_ptr_ = global::state->AssetCache->get_material(
        hash_string(obj_info.material.c_str()));
    */        
    if (!mat_ptr_) {
        return false;
    }

    /*
    if (!global::state->AssetCache->get_mesh(hash_string(obj_info.mesh.c_str()), 
                                             &mesh_)) {
        return false;
    }
    */

    world_transform_.set_scale_component(obj_info.scale);
    world_transform_.set_translation_component(
        v8::math::vector3F(obj_info.translate[0], obj_info.translate[1], 
                           obj_info.translate[2]));
    return true;
}

void v8::scene::geometric_entity::draw(
    v8::scene::scene_system* sc_mgr,
    v8::rendering::renderer* re
    ) {
    assert(attached_effect_);
    assert(technique_);
    assert(mat_ptr_);

    pre_draw(sc_mgr, re);

    //
    // Note that the input layout is set by each pass, in the apply call.
    //re->ia_stage_set_vertex_buffers(mesh_.vertex_buffer_, 1);
    mesh_.vertex_buffer_->bind_to_pipeline(re);
    //re->ia_stage_set_index_buffer(mesh_.index_buffer_);
    mesh_.index_buffer_->bind_to_pipeline(re);
    re->ia_stage_set_primitive_topology_type(mesh_.topology_);

    for (v8_uint32_t pass_index = 0; pass_index < technique_->get_pass_count();
         ++pass_index) {
        rendering::effect_pass* current_pass = 
            technique_->get_pass_by_index(pass_index);
        current_pass->apply();
        re->draw_indexed(mesh_.index_buffer_->get_element_count());
    }
    post_draw(sc_mgr, re);
}
