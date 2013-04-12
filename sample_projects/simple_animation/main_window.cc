#include <v8/v8.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/light.hpp>

#include <v8/global_state.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/rendering/effect_info.hpp>
#include <v8/rendering/effect.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/rendering/sampler_state.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/event/window_event.hpp>

#include "animated_aircraft.hpp"
#include "draw_context.hpp"
#include "main_window.hpp"

const v8_int_t MAX_LIGHTS = 8;

struct lighting_model {
    enum Value {
        blinn_phong,
        cook_torrance,
        max = 0xffffffff
    };
};

struct main_window::scene_details {
public :    
    scene_details();

public :
    animated_aircraft               aircraft;
    v8::rendering::effect*          draw_effect; 
    v8::rendering::vertex_shader    vert_shader;
    v8::rendering::fragment_shader  frag_shader;
    v8::rendering::sampler_state    smp_state;
    v8::math::light                 lights[MAX_LIGHTS];
    v8_int_t                        lights_count;
};

main_window::scene_details::scene_details()
    :       aircraft()
        ,   draw_effect(nullptr)
        ,   lights_count(0)
{}        

main_window::main_window()
    :       base_class()
        ,   scene_(new scene_details()) {}

main_window::~main_window() {}

v8_bool_t main_window::initialize_scene() {
    scene_->lights[0] = v8::math::light::make_point_light(
        v8::math::color_rgb(0.1f, 0.1f, 0.1f, 1.0f),
        v8::math::color_rgb(1.0f, 1.0f, 1.0f, 1.0f), 
        v8::math::color_rgb(1.0f, 1.0f, 1.0f, 1.0f),
        v8::math::vector3F(-20.0f, 10.0f, 0.0f), 
        v8::math::vector3F(0.0f, 0.01f, 0.0f), 
        100.0f
        );
    scene_->lights_count = 1;

    const std::string model_config_file_path = 
        v8::state->file_sys()->make_full_path(v8::filesys::Dir::Models, "f4", "json");

    const v8_bool_t ret_val = scene_->aircraft.initialize(
        model_config_file_path.c_str(), v8::state->render_sys()
        );
    if (!ret_val) {
        return false;
    }

    using namespace v8::rendering;

    shader_info_t vs_info;
    vs_info.compile_macros = "[__VERTEX_PNT__]";
    vs_info.entrypoint = "vs_main";
    vs_info.is_filename = true;
    vs_info.name_or_source = v8::state->file_sys()->make_shader_path("vs_anim");
    vs_info.shader_model = "vs_5_0";
    vs_info.compile_flags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Matrix_Packing_Column_Major | 
        Compile_Options::Optimization_L0 |
        Compile_Options::Skip_Optimization | 
        Compile_Options::Warnings_Are_Errors;

    if (!scene_->vert_shader.initialize(vs_info, v8::state->render_sys())) {
        return false;
    }

    shader_info_t ps_info;
    ps_info.compile_macros = "[__VERTEX_PNT__]";
    ps_info.entrypoint = "ps_main";
    ps_info.is_filename = true;
    ps_info.name_or_source = v8::state->file_sys()->make_shader_path("ps_lighting");
    ps_info.shader_model = "ps_5_0";
    ps_info.compile_flags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Matrix_Packing_Column_Major | 
        Compile_Options::Optimization_L0 |
        Compile_Options::Skip_Optimization | 
        Compile_Options::Warnings_Are_Errors;

    if (!scene_->frag_shader.initialize(ps_info, v8::state->render_sys())) {
        return false;
    }

    sampler_descriptor_t sstate_info;
    if (!scene_->smp_state.initialize(sstate_info, v8::state->render_sys())) {
        return false;
    }
    return true;
}

void main_window::app_frame_tick() {
    const float delta_ms = m_runstats.timer.tick();
    m_runstats.fpscount.frame_tick(delta_ms);
    
    /*assert(scene_->draw_effect);

    scene_->draw_effect->set_variable_by_semantic(
        "LightingModel", lighting_model::blinn_phong
        );
    scene_->draw_effect->set_variable_by_semantic(
        "SceneLights", scene_->lights
        );
    scene_->draw_effect->set_variable_by_semantic(
        "EyePosition", v8::state->camera()->get_origin()
        );
    scene_->draw_effect->set_variable_by_semantic(
        "LightsCount", scene_->lights_count
        );*/

    scene_->frag_shader.set_uniform_by_name(
        "k_lighting_model", lighting_model::blinn_phong
        );
    scene_->frag_shader.set_uniform_by_name(
        "k_scene_lights", scene_->lights
        );
    scene_->frag_shader.set_uniform_by_name(
        "k_eye_pos", v8::state->camera()->get_origin()
        );
    scene_->frag_shader.set_uniform_by_name(
        "k_light_count", scene_->lights_count
        );
    scene_->frag_shader.set_sampler(
        "sam_linear", scene_->smp_state.internal_np_get_handle()
        );

    scene_->aircraft.update(delta_ms);
}

void main_window::frame_draw_impl() {
    const v8::math::matrix_4X4F proj_view = 
        v8::state->camera()->get_projection_wiew_transform();

    draw_context_t draw_ctx(v8::state->render_sys(), proj_view, *v8::state->camera());

    scene_->aircraft.draw(draw_ctx, &scene_->vert_shader, &scene_->frag_shader);
    
    v8::base::auto_buffer<wchar_t> displ_string;
    v8::state->render_sys()->draw_string(
        L"Test", 12.0f, 5.0f, 5.0f, v8::math::color_rgb::C_White
        );
}