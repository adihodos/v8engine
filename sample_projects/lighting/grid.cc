#include <cassert>
#include <v8/io/filesystem.hpp>
#include <v8/global_state.hpp>
#include <v8/io/config_file_reader.hpp>
#include <v8/rendering/effect.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/scene/scene_loading_info.hpp>
#include <v8/scene/scene_system.hpp>
#include <v8/rendering/vertex_pn.hpp>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/geometry_generators.hpp>
#include <v8/math/matrix4X4.hpp>

#include "grid.hpp"

v8_bool_t simple_grid::initialize(const char* config_file_name) {
    assert(!vertexbuffer_);
    assert(!indexbuffer_);

    grid_config_.set_defaults();

    using namespace v8;
    platformstl::path config_file_path(state->file_sys()->get_dir_path(
        filesys::Dir::Config)
    );
    config_file_path.push(config_file_name);

    const io::V8ConfigFile config_file(config_file_path.c_str());
    if (!config_file.CheckIfValid()) {
        return false;
    }

    const v8::io::V8ConfigFile_ReadOnlySection conf_section(
        config_file.GetSection()
        );
    
    conf_section.GetValue("num_rows", grid_config_.num_rows);
    conf_section.GetValue("num_cols", grid_config_.num_columns);
    conf_section.GetValue("width", grid_config_.width);
    conf_section.GetValue("height", grid_config_.height);
    const char* k_material = nullptr;
    conf_section.GetValue("material", k_material);

    if (!material_.initialize(k_material)) {
        return false;
    }

    const v8_int_t k_num_rows = grid_config_.num_rows;
    const v8_int_t k_num_cols = grid_config_.num_columns;

    using namespace v8::math::geometry_gen;
    mesh_data_t mesh_data;
    create_grid(grid_config_.width, grid_config_.height, 
                grid_config_.num_rows, grid_config_.num_columns, 
                &mesh_data);

    using namespace v8::rendering;
    std::vector<vertex_pn> grid_vertices;
    grid_vertices.reserve(mesh_data.md_vertices.size());
    std::transform(
        std::begin(mesh_data.md_vertices), std::end(mesh_data.md_vertices),
        std::back_inserter(grid_vertices),
        [](const vertex3F_t& in_vtx) {
        return v8::rendering::vertex_pn(in_vtx.vt_position, in_vtx.vt_normal);
    });

    renderer* k_rsys = state->render_sys();

    vertexbuffer_.initialize(k_rsys, grid_vertices.size(), sizeof(grid_vertices[0]), 
                             &grid_vertices[0]);
    if (!vertexbuffer_) {
        return false;
    }
    indexbuffer_.initialize(k_rsys, mesh_data.md_indices.size(), 
                            sizeof(mesh_data.md_indices[0]), 
                            &mesh_data.md_indices[0]);
    if (!indexbuffer_) {
        return false;
    }
    topology_ = rendering::PrimitiveTopology::TriangleList;
    return true;
}

void simple_grid::pre_draw(v8::scene::scene_system*, v8::rendering::renderer*) {
    const auto k_to_world = v8::math::matrix_4X4F::identity;
    const auto k_wvp = v8::state->scene()->cam()->get_projection_wiew_transform() 
                       * k_to_world;

    attached_effect_->set_variable_by_semantic(
        "WorldViewProjectionTransform", k_wvp
        );
    attached_effect_->set_variable_by_semantic("WorldTransform", k_to_world);
    attached_effect_->set_variable_by_semantic(
        "NormalTransform", v8::math::matrix_4X4F::identity
        );
    material_.bind_to_effect(attached_effect_);
    attached_effect_->set_variable_by_semantic("GenerateSurfaceHeightViaVS", 1);
}

void simple_grid::post_draw(v8::scene::scene_system*, v8::rendering::renderer*) {
    attached_effect_->set_variable_by_semantic("GenerateSurfaceHeightViaVS", 0);
}

void simple_grid::update(float /*delta*/) {
}