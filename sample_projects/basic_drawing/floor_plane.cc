#include <algorithm>
#include <vector>

#include <v8/io/filesystem.hpp>
#include <v8/math/color.hpp>
#include <v8/math/light.hpp>
#include <v8/math/geometry_generators.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_p.hpp>
#include <v8/rendering/vertex_pt.hpp>
#include <v8/rendering/vertex_shader.hpp>

#include <stlsoft/shims/access/string.hpp>

#include "app_context.hpp"
#include "floor_plane.hpp"

FloorPlane::FloorPlane()
    :   base_class()
{}

v8_bool_t FloorPlane::Initialize(const InitContext* init_context) {
    using namespace v8::math;
    using namespace v8::rendering;
    using namespace std;

    geometry_gen::mesh_data_t grid_data;
    geometry_gen::create_grid(1024.0f, 1024.0f, 256, 256, &grid_data);

    vector<vertex_pt> grid_vertices;
    grid_vertices.reserve(grid_data.md_vertices.size());

    transform(begin(grid_data.md_vertices), end(grid_data.md_vertices),
              back_inserter(grid_vertices),
              [](const geometry_gen::vertex_pntt& vtx) -> vertex_pt {
        return v8::rendering::vertex_pt(vtx.vt_position, vtx.vt_texcoord);
    });

    const v8_bool_t vb_created = vertexbuffer_.initialize(
        init_context->Renderer, grid_vertices.size(),
        sizeof(grid_vertices[0]), &grid_vertices[0]
    );
    if (!vb_created) {
        return false;
    }

    const v8_bool_t ib_created = indexbuffer_.initialize(
        init_context->Renderer, grid_data.md_indices.size(),
        sizeof(grid_data.md_indices[0]), &grid_data.md_indices[0]
    );

    if (!ib_created) {
        return false;
    }

    shader_info_t shader_info;
    shader_info.compile_flags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Matrix_Packing_Row_Major | Compile_Options::Optimization_L0 |
        Compile_Options::Skip_Optimization | Compile_Options::Warnings_Are_Errors;

    shader_info.entrypoint = "xform_vertex_pt";
    shader_info.is_filename = true;
    shader_info.name_or_source = init_context->FileSystem->make_shader_path(
        "basic_transform"
        );
    shader_info.shader_model = "vs_5_0";
    shader_info.shader_root_directory = init_context->FileSystem->get_dir_path(
        v8::filesys::Dir::Shaders
        );

    if (!vertexshader_.initialize(shader_info, init_context->Renderer)) {
        return false;
    }

    shader_info.entrypoint = "color_vertex_pt";
    shader_info.name_or_source = init_context->FileSystem->make_shader_path(
        "basic_coloring"
        );
    shader_info.shader_model = "ps_5_0";

    if (!fragshader_.initialize(shader_info, init_context->Renderer)) {
        return false;
    }

    raster_descriptor_t rsd(raster_descriptor_t::default_state());
    rsd.fill_mode = FillMode::Wireframe;
    rsd.cull_mode = CullMode::None;

    if (!rs_wireframe_.initialize(rsd, init_context->Renderer)) {
        return false;
    }

    const std::string tex_file_path(
            init_context->FileSystem->make_texture_path("ash_uvgrid01.dds"));

    const v8_bool_t texture_loaded = mtl_grid_.initialize(
            stlsoft::c_str_data(tex_file_path), *init_context->Renderer);

    if (!texture_loaded) {
        return false;
    }

    sampler_descriptor_t sampler_desc;
    sampler_desc.tex_address_u = Texture_Address_Mode::Wrap;
    sampler_desc.tex_address_v = Texture_Address_Mode::Wrap;

    const v8_bool_t sampler_initialized = tex_sampler_.initialize(
            sampler_desc, init_context->Renderer);
    if (!sampler_initialized) {
        return false;
    }

    return true;
}

void FloorPlane::Update(const float /*delta*/) {
}

void FloorPlane::Draw(const DrawingContext* draw_context) {
    using namespace v8::rendering;

    vertexbuffer_.bind_to_pipeline(draw_context->Renderer);
    indexbuffer_.bind_to_pipeline(draw_context->Renderer);

    draw_context->Renderer->ia_stage_set_primitive_topology_type(
        PrimitiveTopology::TriangleList
        );

    draw_context->Renderer->ia_stage_set_input_layout(
        vertexshader_.get_input_signature()
        );

    vertexshader_.set_uniform_by_name(
        "world_view_projection", draw_context->ProjectionViewXForm
        );

    const v8_int32_t use_texture = false;

    fragshader_.set_sampler("texSampler", tex_sampler_.internal_np_get_handle());
    fragshader_.set_uniform_by_name("g_use_texturing", use_texture);

    if (use_texture) {

        fragshader_.set_resource_view("colourTex", mtl_grid_.handle());

    } else {

        fragshader_.set_uniform_by_name(
            "g_vertex_color", v8::math::color_rgb::C_MediumSpringGreen
            );
    }

    vertexshader_.bind_to_pipeline(draw_context->Renderer);
    fragshader_.bind_to_pipeline(draw_context->Renderer);

    draw_context->Renderer->set_rasterizer_state(rs_wireframe_);
    draw_context->Renderer->draw_indexed(indexbuffer_.get_element_count());

    draw_context->Renderer->reset_rasterizer_state();
}