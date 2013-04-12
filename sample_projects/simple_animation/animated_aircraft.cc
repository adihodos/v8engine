#include <v8/math/color.hpp>
#include <v8/math/matrix3X3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/math/vector3.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/rendering/effect.hpp>
#include <v8/rendering/effect_info.hpp>
#include <v8/rendering/effect_technique.hpp>
#include <v8/rendering/effect_pass.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/texture_info.hpp>
#include <v8/rendering/texture.hpp>
#include <v8/rendering/vertex_pnt.hpp>
#include <v8/global_state.hpp>
#include <third_party/assimp/Importer.hpp>
#include <third_party/assimp/scene.h>
#include <third_party/assimp/postprocess.h>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "draw_context.hpp"
#include "animated_aircraft.hpp"
#include "texture_wrapper.hpp"

struct animated_aircraft::implementation_details {
    implementation_details();

    v8_int_t                                topology;
    v8::rendering::vertex_buffer            vertex_buff;
    v8::rendering::index_buffer             index_buff;
    //v8::rendering::texture                  diffuse_mat;
    texture_wrapper                         diffuse_mat;
    v8::math::color_rgb                     specular_mat;
    v8::rendering::effect*                  draw_effect;
    v8::rendering::effect_technique*        draw_technique;
    v8::math::matrix_4X4F                   model_rotate_translate;
    float                                   model_scale;
};

animated_aircraft::implementation_details::implementation_details()
    :       topology(v8::rendering::PrimitiveTopology::Undefined)
        ,   draw_effect(nullptr)
        ,   draw_technique(nullptr)
        ,   model_rotate_translate(v8::math::matrix_4X4F::identity)
        ,   model_scale(1.0f)
{}    

animated_aircraft::animated_aircraft()
    :       pimpl_(new implementation_details())
{}

animated_aircraft::~animated_aircraft() {}

v8_bool_t animated_aircraft::initialize(
    const char* const model_config, v8::rendering::renderer* r_sys
    ) {
    assert(r_sys);

    const char* const k_mesh_file = "f4\\f4_phantom.obj";
    std::string mesh_fullpath = v8::state->file_sys()->make_model_path(k_mesh_file);
    if (!load_mesh(mesh_fullpath.c_str(), r_sys)) {
        return false;
    }

    if (!load_effect()) {
        return false;
    }

    return true;
}

void animated_aircraft::update(const float delta_tm) {
}

void animated_aircraft::draw(
    const draw_context_t& draw_cont, void* vs, void* ps
    ) {
    assert(pimpl_->vertex_buff);
    assert(pimpl_->index_buff);
    assert(pimpl_->diffuse_mat);
    //assert(pimpl_->draw_effect);
    //assert(pimpl_->draw_technique);

    v8::rendering::vertex_shader* vs_shader = 
        static_cast<v8::rendering::vertex_shader*>(vs);
    v8::rendering::fragment_shader* fs_shader = 
        static_cast<v8::rendering::fragment_shader*>(ps);

    const v8::math::matrix_4X4F mwvp_xform = 
        draw_cont.context_view_proj * pimpl_->model_rotate_translate;

    vs_shader->set_uniform_by_name(
        "Matrix_WVP", mwvp_xform
        );
    vs_shader->set_uniform_by_name(
        "Matrix_W", pimpl_->model_rotate_translate
        );
    vs_shader->set_uniform_by_name(
        "Matrix_NT", v8::math::matrix_4X4F::identity
        );
    //
    // Since the world transform is just a rotation + a translation
    // we can use it as is to transform normals.
    /*vs_shader->set_uniform_by_name(
        "Matrix_W", pimpl_->model_rotate_translate
        );*/

    fs_shader->set_uniform_by_name("use_diffuse_map", 1);
    fs_shader->set_resource_view(
        "tex_diffuse", pimpl_->diffuse_mat.get_srv()
        );

    fs_shader->set_uniform_by_name("use_specular_map", 0);

    v8::math::color_rgb specular_highlight_clr(1.0f, 1.0f, 1.0f, 128.0f);
    fs_shader->set_uniform_by_name(
        "mat_specular", specular_highlight_clr
        );


    pimpl_->vertex_buff.bind_to_pipeline(draw_cont.context_renderer);
    pimpl_->index_buff.bind_to_pipeline(draw_cont.context_renderer);
    draw_cont.context_renderer->ia_stage_set_primitive_topology_type(
        static_cast<v8::rendering::PrimitiveTopology::Type>(pimpl_->topology)
        );
    draw_cont.context_renderer->ia_stage_set_input_layout(
        vs_shader->get_input_signature()
        );
    vs_shader->bind_to_pipeline(draw_cont.context_renderer);
    fs_shader->bind_to_pipeline(draw_cont.context_renderer);

    draw_cont.context_renderer->draw_indexed(pimpl_->index_buff.get_element_count());

    /*
    for (size_t pass_idx = 0; pass_idx < pimpl_->draw_technique->get_pass_count();
         ++pass_idx) {
        v8::rendering::effect_pass* pass = 
            pimpl_->draw_technique->get_pass_by_index(pass_idx);

        draw_cont.context_renderer->ia_stage_set_input_layout(pass->get_input_layout());
        pass->apply();
        draw_cont.context_renderer->draw_indexed(pimpl_->index_buff.get_element_count());
    }
    */
}

v8_bool_t animated_aircraft::load_mesh(
    const char* mesh_file_path, v8::rendering::renderer* r_sys
    ) {
    assert(!pimpl_->vertex_buff);
    assert(!pimpl_->index_buff);
    assert(!pimpl_->diffuse_mat);

    pimpl_->topology = v8::rendering::PrimitiveTopology::TriangleList;

    Assimp::Importer importer;
    const v8_uint_t k_load_flags = aiProcess_JoinIdenticalVertices 
        | aiProcess_MakeLeftHanded | aiProcess_Triangulate 
        | aiProcess_GenSmoothNormals | aiProcess_FlipWindingOrder;
    const aiScene* k_scene       = importer.ReadFile(mesh_file_path, k_load_flags);
    if (!k_scene) {
        OUTPUT_DBG_MSGA("Failed to import scene, error %s", importer.GetErrorString());
        return false;
    }

    v8_size_t num_indices  = 0;
    v8_size_t num_vertices = 0;

    for (v8_uint_t mesh_index = 0; mesh_index < k_scene->mNumMeshes; ++mesh_index) {
        const aiMesh* k_mesh = k_scene->mMeshes[mesh_index];
        num_vertices += k_mesh->mNumVertices;
        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            num_indices += k_face->mNumIndices;
        }
    }

    using namespace v8::rendering;
    std::vector<vertex_pnt> vertices;
    vertices.reserve(num_vertices);
    std::vector<uint32_t> indices;
    indices.reserve(num_indices);

    using namespace v8;
    v8_uint_t indices_offset = 0;
    for (v8_uint_t mesh_index = 0; mesh_index < k_scene->mNumMeshes; ++mesh_index) {
        const aiMesh* k_mesh = k_scene->mMeshes[mesh_index];
        
        for (v8_uint_t vertex_idx = 0; vertex_idx < k_mesh->mNumVertices; ++vertex_idx) {
            const aiVector3D* k_imported_vtx = &k_mesh->mVertices[vertex_idx];
            vertex_pnt new_vtx;
            new_vtx.position.x_ = k_imported_vtx->x;
            new_vtx.position.y_ = k_imported_vtx->y;
            new_vtx.position.z_ = k_imported_vtx->z;
            
            k_imported_vtx = &k_mesh->mNormals[vertex_idx];
            new_vtx.normal.x_ = k_imported_vtx->x;
            new_vtx.normal.y_ = k_imported_vtx->y;
            new_vtx.normal.z_ = k_imported_vtx->z;

            k_imported_vtx = &k_mesh->mTextureCoords[0][vertex_idx];

            new_vtx.texcoord.x_ = k_imported_vtx->x;
            new_vtx.texcoord.y_ = k_imported_vtx->y;

            vertices.push_back(new_vtx);
        }
        for (v8_uint_t face_index = 0; face_index < k_mesh->mNumFaces; ++face_index) {
            const aiFace* k_face = &k_mesh->mFaces[face_index];
            for (v8_uint_t i = 0; i < k_face->mNumIndices; ++i) {
                indices.push_back(k_face->mIndices[i] + indices_offset);
            }
        }
        indices_offset += k_mesh->mNumVertices;
    }

    pimpl_->vertex_buff.initialize(r_sys, vertices.size(), sizeof(vertices[0]),
                                   &vertices[0]);
    if (!pimpl_->vertex_buff) {
        return false;
    }

    pimpl_->index_buff.initialize(r_sys, indices.size(), sizeof(indices[0]),
                                  &indices[0]);
    if (!pimpl_->index_buff) {
        return false;
    }

    pimpl_->specular_mat = v8::math::color_rgb(0.752941f, 0.752941f, 0.752941f);

    texture_info_t tex_info;
    tex_info.tex_bindflags = BindingFlag::ShaderResource;
    tex_info.tex_filename = v8::state->file_sys()->make_texture_path(
        "f4\\f4_texture.jpg"
        );

    if (!pimpl_->diffuse_mat.initialize(r_sys, tex_info.tex_filename.c_str())) {
        return false;
    }    
    return true;
}

v8_bool_t animated_aircraft::load_effect() {
    assert(!pimpl_->draw_effect);
    assert(!pimpl_->draw_technique);

    v8::rendering::effect_info_t eff_info;
    eff_info.eff_compile_macros = "[__VERTEX_PNT__]";
    eff_info.eff_target = v8::rendering::FX_Model::FX_5_0;
    const char* const effect_file_name = "lighting";
    eff_info.eff_filepath = v8::state->file_sys()->make_effect_path(
        effect_file_name
        );
    using v8::rendering::Compile_Options;
    eff_info.eff_compile_flags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Matrix_Packing_Column_Major | 
        Compile_Options::Skip_Optimization | Compile_Options::Warnings_Are_Errors;

    pimpl_->draw_effect = v8::state->asset_cache()->get_effect(eff_info);
    if (!pimpl_->draw_effect) {
        return false;
    }

    const char* const draw_technique_name = "lighting";
    pimpl_->draw_technique = pimpl_->draw_effect->get_technique_by_name(
        draw_technique_name
        );
    return pimpl_->draw_technique != nullptr;
}