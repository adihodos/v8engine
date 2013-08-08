#include <v8/event/input_event.hpp>
#include <v8/input/key_syms.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/math/light.hpp>
#include <v8/math/quaternion.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/texture_descriptor.hpp>
#include <v8/rendering/vertex_pnt.hpp>
#include <v8/utility/geometry_importer.hpp>

#include "app_context.hpp"
#include "aircraft_f4.hpp"

F4Phantom::F4Phantom() 
    :       is_valid_(false) {}

F4Phantom::~F4Phantom() {
}

v8_bool_t 
F4Phantom::Initialize(const InitContext* init_context) {
    assert(!is_valid());

    memset(keystates_, 0, sizeof(keystates_));
    SetDefaultAicraftDefaultParameters();

    using namespace v8::base;
    using namespace v8::rendering;

    scoped_ptr<vertex_pnt, default_array_storage>    vertices;
    scoped_ptr<v8_uint32_t, default_array_storage>  indices;
    v8_uint32_t num_vertices;
    v8_uint32_t num_indices;
    const std::string model_path = init_context->FileSystem->make_model_path(
        "f4_phantom.obj");

    const v8_bool_t load_succeeded = v8::utility::import_geometry(
        model_path.c_str(), true, scoped_pointer_get_impl(vertices),
        scoped_pointer_get_impl(indices), &num_vertices, &num_indices
        );

    if (!load_succeeded) {
        return false;
    }

    const v8_bool_t vb_created = vertexbuffer_.initialize(
        init_context->Renderer, num_vertices, sizeof(*vertices),
        scoped_pointer_get(vertices)
        );
    if (!vb_created) {
        return false;
    }

    const v8_bool_t ib_created = indexbuffer_.initialize(
        init_context->Renderer, num_indices, sizeof(*indices),
        scoped_pointer_get(indices)
        );
    if (!ib_created) {
        return false;
    }

    using v8::rendering::Compile_Options;
    const v8_uint32_t kShaderCompileFlags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Matrix_Packing_Row_Major | Compile_Options::Optimization_L0 |
        Compile_Options::Skip_Optimization | Compile_Options::Warnings_Are_Errors;

    v8::rendering::shader_info_t shader_info;
    shader_info.compile_flags         = kShaderCompileFlags;
    shader_info.is_filename           = true;
    shader_info.name_or_source        = 
        init_context->FileSystem->make_shader_path("ps_lighting");
    shader_info.shader_root_directory = init_context->FileSystem->get_dir_path(
        v8::filesys::Dir::Shaders
        );
    shader_info.entrypoint            = "ps_main";
    shader_info.shader_model          = "ps_5_0";
    shader_info.compile_macros        = "[__VERTEX_PNT__]";

    if (!fragshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    shader_info.name_or_source =
        init_context->FileSystem->make_shader_path("basic_transform");
    shader_info.entrypoint = "xform_vertex_pnt";
    shader_info.shader_model = "vs_5_0";
    if (!vertexshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    const char* const fname = "f4_texture.dds";
    const std::string tex_filename(init_context->FileSystem->make_texture_path(fname));

    if (!material_.initialize(tex_filename.c_str(), *init_context->Renderer)) {
        return false;
    }

    samplerDescriptor_t sampler_desc;
    sampler_desc.filter_type = Filter::Min_Mag_Mip_Linear;
    if (!sam_state_.initialize(sampler_desc, init_context->Renderer)) {
        return false;
    }

    //
    // Mark object as valid.
    is_valid_ = true;
    return true;
}

void 
F4Phantom::SetDefaultAicraftDefaultParameters() {
    flight_characteristics_.SetDefaultValues();
    coord_frame_.SetDefaultValues();
}

void 
F4Phantom::Update(const float /*delta*/) {
    using v8::input::Key_Sym_t;

    F4RotationData_t orientation;

    if (keystates_[Key_Sym_t::Key_W]) {
        MoveZ(1.0f);
    }

    if (keystates_[Key_Sym_t::Key_S]) {
        MoveZ(-1.0f);
    }

    if (keystates_[Key_Sym_t::Key_A]) {
        MoveX(-1.0f);
    }

    if (keystates_[Key_Sym_t::Key_D]) {
        MoveX(1.0f);
    }

    if (keystates_[Key_Sym_t::Key_X]) {
        MoveY(1.0f);
    }

    if (keystates_[Key_Sym_t::Key_C]) {
        MoveY(-1.0f);
    }

    if (keystates_[Key_Sym_t::Key_Q]) {
        Roll(+1.0f, &orientation);
    }

    if (keystates_[Key_Sym_t::Key_E]) {
        Roll(-1.0f, &orientation);
    }

    if (keystates_[Key_Sym_t::Key_U]) {
        Pitch(+1.0f, &orientation);
    }

    if (keystates_[Key_Sym_t::Key_J]) {
        Pitch(-1.0f, &orientation);
    }

    if (keystates_[Key_Sym_t::Key_N]) {
        Yaw(-1.0f, &orientation);
    }

    if (keystates_[Key_Sym_t::Key_M]) {
        Yaw(+1.0f, &orientation);
    }

    UpdateCorrdFrameAxisVectors(orientation);
}

void 
F4Phantom::UpdateCorrdFrameAxisVectors(
    const F4Phantom::F4RotationData_t& rot_data
    ) {
    using namespace v8::math;

    quaternionF q_all_rot(quaternionF::identity);
    v8_bool_t needs_rotated = false;

    if (rot_data.do_roll) {
        needs_rotated = true;
        quaternionF q_roll(rot_data.quant_roll, coord_frame_.axis_z);
        q_all_rot = q_roll * q_all_rot;
    }

    if (rot_data.do_pitch) {
        needs_rotated = true;
        quaternionF q_pitch(rot_data.quant_pitch, coord_frame_.axis_x);
        q_all_rot = q_all_rot * q_pitch;
    }

    if (rot_data.do_yaw) {
        needs_rotated = true;
        quaternionF q_yaw(rot_data.quant_yaw, coord_frame_.axis_y);
        q_all_rot = q_all_rot * q_yaw;
    }

    if (needs_rotated) {
        q_all_rot.rotate_vector(&coord_frame_.axis_x);
        q_all_rot.rotate_vector(&coord_frame_.axis_z);

        coord_frame_.axis_x.normalize();
        coord_frame_.axis_z.normalize();
        coord_frame_.axis_y = cross_product(coord_frame_.axis_z,
                                            coord_frame_.axis_x);
    }
}

void 
F4Phantom::Draw(const DrawingContext* draw_ctx) {
    assert(is_valid());

    using namespace v8::math;

    indexbuffer_.bind_to_pipeline(draw_ctx->Renderer);
    vertexbuffer_.bind_to_pipeline(draw_ctx->Renderer);
    draw_ctx->Renderer->ia_stage_set_input_layout(
        vertexshader_.get_input_signature()
        );
    draw_ctx->Renderer->ia_stage_set_primitive_topology_type(
        v8::rendering::PrimitiveTopology::TriangleList
        );

    matrix_4X4F model_to_world_xform;

    model_to_world_xform.set_column(
        1, coord_frame_.axis_x.x_, coord_frame_.axis_x.y_, coord_frame_.axis_x.z_, 0.0f
        );
    model_to_world_xform.set_column(
        2, coord_frame_.axis_y.x_, coord_frame_.axis_y.y_, coord_frame_.axis_y.z_, 0.0f
        );
    model_to_world_xform.set_column(
        3, coord_frame_.axis_z.x_, coord_frame_.axis_z.y_, coord_frame_.axis_z.z_, 0.0f
        );
    model_to_world_xform.set_column(
        4, coord_frame_.origin.x_, coord_frame_.origin.y_, coord_frame_.origin.z_, 1.0f
        );
        
    vertexshader_.set_uniform_by_name(
        "world_view_projection", 
        draw_ctx->ProjectionViewXForm * model_to_world_xform
        );

    //
    // Model to world is just a rotation + a translation so we can use it as is
    // to transform the normals.
    vertexshader_.set_uniform_by_name(
        "normal_transform", model_to_world_xform
        );

    fragshader_.set_uniform_by_name("k_lighting_model", 0);

    fragshader_.set_uniform_by_name(
        "k_scene_lights", draw_ctx->ActiveLights.base(),
        8 * sizeof(draw_ctx->ActiveLights[0])
        );

    fragshader_.set_uniform_by_name("k_eye_pos", draw_ctx->EyePosition);
    fragshader_.set_uniform_by_name("k_light_count", 1);

    fragshader_.set_uniform_by_name("use_diffuse_map", 1);
    fragshader_.set_uniform_by_name("specular_pow", 128.0f);

    fragshader_.set_sampler("sam_linear", sam_state_.internal_np_get_handle());
    fragshader_.set_resource_view("tex_diffuse", material_.handle());

    vertexshader_.bind_to_pipeline(draw_ctx->Renderer);
    fragshader_.bind_to_pipeline(draw_ctx->Renderer);

    draw_ctx->Renderer->draw_indexed(indexbuffer_.get_element_count());
}

void 
F4Phantom::InputEvent(const v8::input_event& in_evt) {
    if (in_evt.type != v8::InputEventType::Key) {
        return;
    }

    if ((in_evt.key_ev.key == v8::input::Key_Sym_t::Backspace)
        && (in_evt.key_ev.down)) {
        SetDefaultAicraftDefaultParameters();
        return;
    }

    keystates_[in_evt.key_ev.key] = in_evt.key_ev.down;
}

inline void F4Phantom::MoveX(const float modifier) {
    coord_frame_.origin.x_ += modifier * 0.003f /** coord_frame_.axis_x.x_*/;
}

inline void F4Phantom::MoveY(const float modifier) {
    coord_frame_.origin.y_ += modifier * 0.003f /** coord_frame_.axis_y.y_*/;
}

inline void F4Phantom::MoveZ(const float modifier) {
    coord_frame_.origin.z_ += modifier * 0.003f /** coord_frame_.axis_z.z_*/;
}

inline void F4Phantom::Roll(
    const float modifier, F4Phantom::F4RotationData_t* rot_data
    ) {
    assert(rot_data);
    rot_data->do_roll = true;
    rot_data->quant_roll = modifier * flight_characteristics_.roll_speed;
}

inline void F4Phantom::Yaw(
    const float modifier, F4Phantom::F4RotationData_t* rot_data
    ) {
    assert(rot_data);
    rot_data->do_yaw = true;
    rot_data->quant_yaw = modifier * flight_characteristics_.yaw_speed;
}

inline void F4Phantom::Pitch(
    const float modifier, F4Phantom::F4RotationData_t* rot_data
    ) {
    rot_data->do_pitch = true;
    rot_data->quant_pitch = modifier * flight_characteristics_.pitch_speed;
}
