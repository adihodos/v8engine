#include <D3D11.h>
#include <D3Dcompiler.h>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include <v8/v8.hpp>
#include <v8/base/count_of.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/event/input_event.hpp>
#include <v8/event/window_event.hpp>
#include <v8/global_state.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/depth_stencil_state.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/input_layout.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/rendering/vertex_pt.hpp>
#include <v8/input/key_syms.hpp>

#include "fractal.hpp"

fractal* g_fractal = nullptr;

namespace {

v8_bool_t key_stats[v8::input::Key_Sym_t::Last];

const fractal::complex_type C_shape_constants[] = {
    fractal::complex_type(-0.7f, 0.27015f)
    , fractal::complex_type(0.400f, 0.0f)
    , fractal::complex_type(-0.8f, +0.156f)
    , fractal::complex_type(0.285f, 0.0f)
    , fractal::complex_type(-0.4f, 0.6f) 
    , fractal::complex_type(0.285f, 0.01f)
    , fractal::complex_type(-0.70176f, -0.3842f)
    , fractal::complex_type(-0.835f, -0.2321f)
    , fractal::complex_type(-0.74543f, +0.11301f)
    , fractal::complex_type(-0.75f, +0.11f)
    , fractal::complex_type(-0.1f, +0.651f)     
};

struct fractal_params_t {
    int                                                     width;
    int                                                     height;
    int                                                     max_iterations;
    float                                                   zoom_factor;
    float                                                   offset_x;
    float                                                   offset_y;
    float                                                   C_real;
    float                                                   C_imag;
};

static_assert((sizeof(fractal_params_t) % 16) == 0, "Size must be multiple of 16");

} // anonymous namespace

struct fractal::implementation {

    implementation(int w = 1024, int h = 1024, int iter = 256, float zoom = 1.0f);

    fractal_params_t                                        frac_params;
    v8_bool_t                                               initialized;
    v8_bool_t                                               size_is_current;
    v8_bool_t                                               solution_is_current;
    v8_size_t                                               shape_idx;
    v8::rendering::index_buffer                             indexbuffer;
    v8::rendering::vertex_buffer                            vertexbuffer;
    v8::rendering::depth_stencil_state                      no_depth_test;
    v8::rendering::fragment_shader                          frag_shader;
    v8::rendering::vertex_shader                            vert_shader;
};

fractal::implementation::implementation(
    int w,
    int h,
    int iter,
    float zoom
    ) 
    :       initialized(false)
        ,   size_is_current(false)
        ,   solution_is_current(false)
        ,   shape_idx(0) {

    frac_params.width = w;
    frac_params.height = h;
    frac_params.max_iterations = iter;
    frac_params.zoom_factor = zoom;
    frac_params.offset_x = frac_params.offset_y = 0.0f;
    frac_params.C_real = -0.835f;
    frac_params.C_imag = -0.2321f;
}

fractal::fractal(
    int width,
    int height,
    int iter,
    float zoom
    )
    :   impl_(new implementation(width, height, iter, zoom)) {}

fractal::~fractal() {}

v8_bool_t fractal::initialize() {

    if (impl_->initialized)
        return true;

    using namespace v8;

    const v8::rendering::vertex_pt quad_vertices[] = {
        v8::rendering::vertex_pt(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        v8::rendering::vertex_pt(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f),
        v8::rendering::vertex_pt(+1.0f, +1.0f, 0.0f, 1.0f, 0.0f),
        v8::rendering::vertex_pt(+1.0f, -1.0f, 0.0f, 1.0f, 1.0f)
    };

    v8::rendering::renderer* k_rsys = v8::state->render_sys();

    impl_->vertexbuffer.initialize(k_rsys, dimension_of(quad_vertices),
                                   sizeof(quad_vertices[0]), quad_vertices);
    if (!impl_->vertexbuffer) {
        return false;
    }

    const v8_short_t indices[] = { 0, 1, 2, 0, 2, 3 };
    impl_->indexbuffer.initialize(k_rsys, dimension_of(indices), 
                                  sizeof(indices[0]), indices);

    if (!impl_->indexbuffer)
        return false;

    rendering::depthstencil_descriptor_t state_nodepth;
    state_nodepth.depth_enable = false;
    impl_->no_depth_test.initialize(state_nodepth, k_rsys);
    if (!impl_->no_depth_test) {
        return false;
    }

    v8::rendering::shader_info_t shader_info;
    shader_info.name_or_source  = v8::state->file_sys()->make_shader_path("vertex_to_ndc");
    shader_info.is_filename     = true;
    shader_info.entrypoint      = "vertex_pt_to_ndc";
    shader_info.compile_flags   = v8::rendering::Compile_Options::Generate_Debug_Info |
                                  v8::rendering::Compile_Options::IEEE_Strictness |
                                  v8::rendering::Compile_Options::Skip_Optimization |
                                  v8::rendering::Compile_Options::Matrix_Packing_Row_Major;
    shader_info.shader_model    = "vs_5_0";

    impl_->vert_shader.initialize(shader_info, k_rsys);
    if (!impl_->vert_shader) {
        return false;
    }

    shader_info.name_or_source  = v8::state->file_sys()->make_shader_path("fractals");
    shader_info.entrypoint      = "ps_julia";
    shader_info.shader_model    = "ps_5_0";

    impl_->frag_shader.initialize(shader_info, k_rsys);
    if (!impl_->frag_shader) {
        return false;
    }

    return true;
}

int fractal::get_width() const {
    return impl_->frac_params.width;
}

int fractal::get_height() const {
    return impl_->frac_params.height;
}

void fractal::set_width(int w) {
    assert(w > 0);

    impl_->frac_params.width = w;
    impl_->solution_is_current = false;
    impl_->size_is_current = false;
}

void fractal::set_height(int h) {
    assert(h > 0);

    impl_->frac_params.height = h;
    impl_->solution_is_current = false;
    impl_->size_is_current = false;
}

int fractal::get_iteration_count() const {
    return impl_->frac_params.max_iterations;
}

void fractal::set_iteration_count(int iterations) {
    assert(iterations > 0);

    impl_->frac_params.max_iterations = iterations;
    impl_->solution_is_current = false;
}

float fractal::get_zoom_factor() const {
    return impl_->frac_params.zoom_factor;
}

void fractal::set_zoom_factor(float z) {
    impl_->frac_params.zoom_factor = z;
    impl_->solution_is_current = false;
}

void fractal::set_x_offset(float off_x) {
    impl_->frac_params.offset_x = off_x;
    impl_->solution_is_current = false;
}

float fractal::get_x_offset() const {
    return impl_->frac_params.offset_x;
}

void fractal::set_y_offset(float off_y) {
    impl_->frac_params.offset_y = off_y;
    impl_->solution_is_current = false;
}

float fractal::get_y_offset() const {
    return impl_->frac_params.offset_y;
}

void fractal::set_constant(const fractal::complex_type& const_factor) {
    impl_->frac_params.C_real = const_factor.real();
    impl_->frac_params.C_imag = const_factor.imag();

    impl_->solution_is_current = false;
}

fractal::complex_type fractal::get_constant() const {
    return complex_type(impl_->frac_params.C_real, impl_->frac_params.C_imag);
}

void fractal::evaluate(float delta_ms) {
    using namespace v8::input;

    const float C_offset_factor = 0.0003f;

    if (key_stats[Key_Sym_t::Down]) {
        float new_offset_y = get_y_offset() + C_offset_factor * delta_ms 
                                              / impl_->frac_params.zoom_factor;
        set_y_offset(new_offset_y);
    }
    if (key_stats[Key_Sym_t::Up]) {
        float new_offset_y = get_y_offset() - C_offset_factor * delta_ms 
                                              / impl_->frac_params.zoom_factor;
        set_y_offset(new_offset_y);
    }
    if (key_stats[Key_Sym_t::Left]) {
        float new_offset_x = get_x_offset() - C_offset_factor * delta_ms
                                              / impl_->frac_params.zoom_factor;
        set_x_offset(new_offset_x);
    }
    if (key_stats[Key_Sym_t::Right]) {
        float new_offset_x = get_x_offset() + C_offset_factor * delta_ms
                                              / impl_->frac_params.zoom_factor;
        set_x_offset(new_offset_x);
    }

    if (impl_->solution_is_current) {
        return;
    }
    impl_->frag_shader.set_uniform_block_by_name("globals", impl_->frac_params);
    impl_->solution_is_current = true;
}

void fractal::draw() {
    v8::rendering::renderer* k_rendersys = v8::state->render_sys();

    impl_->vertexbuffer.bind_to_pipeline(k_rendersys);
    impl_->indexbuffer.bind_to_pipeline(k_rendersys);
    k_rendersys->ia_stage_set_input_layout(
        impl_->vert_shader.get_input_signature()
        );
    k_rendersys->ia_stage_set_primitive_topology_type(
        v8::rendering::PrimitiveTopology::TriangleList
        );

    impl_->vert_shader.bind_to_pipeline(k_rendersys);
    impl_->frag_shader.bind_to_pipeline(k_rendersys);

    v8::state->render_sys()->set_depth_stencil_state(impl_->no_depth_test);
    v8::state->render_sys()->draw_indexed(impl_->indexbuffer.get_element_count());
}

v8_bool_t fractal::mouse_wheel_event(
    v8_int_t rotations,
    v8_int_t /*xpos*/, 
    v8_int_t /*ypos*/
    ) {
    float zoom_val          = g_fractal->get_zoom_factor();
    const float C_Exponent  = 3.0f;
    const float C_Base      = 1.5f;

    if (rotations < 0) {
        zoom_val *= pow(C_Base, C_Exponent);
    } else {
        zoom_val /= pow(C_Base, C_Exponent);
    }
    g_fractal->set_zoom_factor(zoom_val);
    return true;
}

v8_bool_t fractal::key_press_event(
    v8_int_t key_code
    ) {
    using namespace v8::input;

    if (Key_Sym_t::KP_Add == key_code) {
        ++impl_->shape_idx;
        const v8_size_t kSelectedShapeIndex = 
            impl_->shape_idx % dimension_of(C_shape_constants);
        set_constant(C_shape_constants[kSelectedShapeIndex]);
    } else if (Key_Sym_t::KP_Multiply == key_code) {
        set_iteration_count(get_iteration_count() * 2);
    } else if (Key_Sym_t::KP_Divide == key_code) {
        int iter_cnt = get_iteration_count();
        if (iter_cnt > 2) {
            iter_cnt /= 2;
            set_iteration_count(iter_cnt);
        }
    }
    key_stats[key_code] = true;
    return true;
}

v8_bool_t fractal::key_depress_event(
    v8_int_t key_code
    ) {
    key_stats[key_code] = false;
    return true;
}

void fractal::on_input(const v8::input_event& ev_input) {
    using namespace v8;

    if (ev_input.type == InputEventType::Mouse_Wheel) {
        mouse_wheel_event(ev_input.mouse_wheel_ev.delta, 
                          ev_input.mouse_wheel_ev.x_pos,
                          ev_input.mouse_wheel_ev.y_pos);
    } else if (ev_input.type == InputEventType::Key) {
        if (ev_input.key_ev.down) {
            key_press_event(ev_input.key_ev.key);
        } else {
            key_depress_event(ev_input.key_ev.key);
        }
    }
}

void fractal::on_resize(const v8::resize_event& ev_resize) {
    set_width(ev_resize.width);
    set_height(ev_resize.height);
}