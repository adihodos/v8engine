#include <vector>

#include <D3D11.h>
#include <D3Dcompiler.h>

#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <v8/v8.hpp>
#include <v8/base/array_proxy.hpp>
#include <v8/base/count_of.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/event/input_event.hpp>
#include <v8/event/window_event.hpp>
#include <v8/math/color.hpp>
#include <v8/math/color_palette_generator.hpp>
#include <v8/math/vector2.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/depth_stencil_state.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/input_layout.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/texture.hpp>
#include <v8/rendering/texture_descriptor.hpp>
#include <v8/rendering/texture_shader_binding.hpp>
#include <v8/rendering/sampler_state.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/rendering/vertex_pt.hpp>
#include <v8/input/key_syms.hpp>

#include "fractal.hpp"

using namespace std;
using namespace v8::math;
using namespace v8::rendering;

namespace {

v8_bool_t key_stats[v8::input::Key_Sym_t::Last];

///
/// \brief Predefined constants that make nice shapes for Julia.
const fractal::complex_type C_shape_constants[] = {
        fractal::complex_type(-0.7f, +0.27015f)
    ,   fractal::complex_type(+0.400f, +0.0f)
    ,   fractal::complex_type(-0.8f, +0.156f)
    ,   fractal::complex_type(+0.285f, +0.0f)
    ,   fractal::complex_type(-0.4f, +0.6f) 
    ,   fractal::complex_type(+0.285f, +0.01f)
    ,   fractal::complex_type(-0.70176f, -0.3842f)
    ,   fractal::complex_type(-0.835f, -0.2321f)
    ,   fractal::complex_type(-0.74543f, +0.11301f)
    ,   fractal::complex_type(-0.75f, +0.11f)
    ,   fractal::complex_type(-0.1f, +0.651f)
    ,   fractal::complex_type(-1.36f, +0.11f)
    ,   fractal::complex_type(+0.32f, +0.043f)
};

///
/// \brief Fractal parameters passed to the pixel shader.
struct fractal_params_t {
    v8_int_t                                                    is_mandelbrot;
    v8_int_t                                                    width;
    v8_int_t                                                    height;
    v8_int_t                                                    max_iterations;
    float                                                       zoom_factor;
    float                                                       offset_x;
    float                                                       offset_y;
    ///< Shape constant, real part.
    float                                                       C_real;
    ///< Shape constant, imaginary part.
    float                                                       C_imag;
    v8::math::vector2<v8_int_t>                                 C_origin;

    ~fractal_params_t() {
    }
};

} // anonymous namespace

struct fractal::implementation {
    implementation(const v8_int_t width = 1024, 
                   const v8_int_t height = 1024,
                   const v8_int_t iterations = 256, 
                   const float zoom = 1.0f);

    fractal_params_t                                        frac_params;
    v8_bool_t                                               initialized;
    v8_bool_t                                               size_is_current;
    v8_bool_t                                               solution_is_current;
    v8_size_t                                               shape_idx;
    v8::math::vector2<v8_int_t>                             origin;
    v8::rendering::index_buffer                             indexbuffer;
    v8::rendering::vertex_buffer                            vertexbuffer;
    v8::rendering::depth_stencil_state                      no_depth_test;
    v8::rendering::fragment_shader                          frag_shader;
    v8::rendering::vertex_shader                            vert_shader;
    v8::rendering::texture_shader_binding                   color_table;
    v8::rendering::sampler_state                            sampler;
};

fractal::implementation::implementation(
    const v8_int_t width,
    const v8_int_t height,
    const v8_int_t iter,
    const float zoom
    ) 
    :       initialized(false)
        ,   size_is_current(false)
        ,   solution_is_current(false)
        ,   shape_idx(0)
        ,   origin(v8::math::vector2<v8_int_t>::zero) 
{
    frac_params.width = width;
    frac_params.height = height;
    frac_params.max_iterations = iter;
    frac_params.zoom_factor = zoom;
    frac_params.offset_x = frac_params.offset_y = 0.0f;
    frac_params.C_real = -0.835f;
    frac_params.C_imag = -0.2321f;
    frac_params.is_mandelbrot = false;
    frac_params.C_origin = v8::math::vector2<v8_int_t>::zero;
}

fractal::fractal(
    const v8_int_t width, 
    const v8_int_t height, 
    const v8_int_t iter, 
    const float zoom
    )
    :       impl_(new implementation(width, height, iter, zoom)) 
{}

fractal::~fractal() {}

v8_bool_t fractal::initialize(fractal_app_context& app_context) {
    if (impl_->initialized) {
        return true;
    }

    using namespace v8;
    const v8::rendering::vertex_pt quad_vertices[] = {
        v8::rendering::vertex_pt(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f),
        v8::rendering::vertex_pt(-1.0f, +1.0f, 0.0f, 0.0f, 0.0f),
        v8::rendering::vertex_pt(+1.0f, +1.0f, 0.0f, 1.0f, 0.0f),
        v8::rendering::vertex_pt(+1.0f, -1.0f, 0.0f, 1.0f, 1.0f)
    };

    impl_->vertexbuffer.initialize(app_context.Renderer, dimension_of(quad_vertices),
                                   sizeof(quad_vertices[0]), quad_vertices);
    if (!impl_->vertexbuffer) {
        return false;
    }

    const v8_short_t indices[] = { 0, 1, 2, 0, 2, 3 };
    impl_->indexbuffer.initialize(app_context.Renderer, dimension_of(indices), 
                                  sizeof(indices[0]), indices);

    if (!impl_->indexbuffer) {
        return false;
    }

    rendering::depthstencil_descriptor_t state_nodepth;
    state_nodepth.depth_enable = false;
    impl_->no_depth_test.initialize(state_nodepth, app_context.Renderer);
    if (!impl_->no_depth_test) {
        return false;
    }

    using v8::rendering::Compile_Options;
    const v8_uint32_t compile_flags = 
        Compile_Options::Generate_Debug_Info | Compile_Options::IEEE_Strictness |
        Compile_Options::Skip_Optimization | Compile_Options::Optimization_L0 |
        Compile_Options::Matrix_Packing_Row_Major;

    v8::rendering::shader_info_t shader_info;
    shader_info.name_or_source  = 
        app_context.Filesys->make_shader_path("vertex_to_ndc");
    shader_info.shader_root_directory = 
        app_context.Filesys->get_dir_path(v8::filesys::Dir::Shaders);
    shader_info.is_filename     = true;
    shader_info.entrypoint      = "vertex_pt_to_ndc";
    shader_info.compile_flags   = compile_flags;
    shader_info.shader_model    = "vs_5_0";

    impl_->vert_shader.initialize(shader_info, app_context.Renderer);
    if (!impl_->vert_shader) {
        return false;
    }

    shader_info.name_or_source  = 
        app_context.Filesys->make_shader_path("fractals");
    shader_info.entrypoint      = "ps_julia";
    shader_info.shader_model    = "ps_5_0";

    impl_->frag_shader.initialize(shader_info, app_context.Renderer);
    if (!impl_->frag_shader) {
        return false;
    }

    if (!create_color_table(app_context)) {
        return false;
    }

    sampler_descriptor_t sampler_desc;
    if (!impl_->sampler.initialize(sampler_desc, app_context.Renderer)) {
        return false;
    }

    return true;
}

v8_bool_t fractal::create_color_table(fractal_app_context& app_context) {
    const int num_colors = 128;

    vector<color_rgb> color_palette(num_colors);

    //auto color_check_fn = [](const color_rgb& rgb) -> bool {
    //    color_hcl hcl;
    //    rgb_to_hcl(rgb, &hcl);

    //    return hcl.Elements[0] >= 20.0f && hcl.Elements[0] <= 60.0f
    //        && hcl.Elements[1] >= 0.3f && hcl.Elements[1] <= 1.6f
    //        && hcl.Elements[2] >= 0.5f && hcl.Elements[2] <= 1.5f;
    //};

    v8::base::array_proxy<color_rgb> arr_proxy(&color_palette [0],
                                               &color_palette[0] + color_palette.size());

    //procedural_palette::generate_color_palette(
    //    color_check_fn, true, 50, false, arr_proxy);
    procedural_palette::gen_uniform_colors(arr_proxy);

    textureDescriptor_t tex_desc(num_colors, 
                                 1, 
                                 textureType_t::Tex1D, 
                                 1,
                                 4, 
                                 ElementType::Float32, 
                                 BindingFlag::ShaderResource,
                                 ResourceUsage::Default,
                                 CPUAccess::None,
                                 false);

    const void* tex_data = arr_proxy.base();
    texture lookup_tex(tex_desc, *app_context.Renderer, &tex_data);

    if (!lookup_tex) {
        return false;
    }

    lookup_tex.write_to_file("C:\\temp\\lookuptex.dds", *app_context.Renderer);
    return impl_->color_table.initialize(lookup_tex, *app_context.Renderer);
}

v8_int_t fractal::get_width() const {
    return impl_->frac_params.width;
}

v8_int_t fractal::get_height() const {
    return impl_->frac_params.height;
}

void fractal::set_width(const v8_int_t w) {
    assert(w > 0);
    impl_->frac_params.width = w;
    impl_->solution_is_current = false;
    impl_->size_is_current = false;
}

void fractal::set_height(const v8_int_t h) {
    assert(h > 0);
    impl_->frac_params.height = h;
    impl_->solution_is_current = false;
    impl_->size_is_current = false;
}

v8_int_t fractal::get_iteration_count() const {
    return impl_->frac_params.max_iterations;
}

void fractal::set_iteration_count(const v8_int_t iterations) {
    assert(iterations > 0);
    impl_->frac_params.max_iterations = iterations;
    impl_->solution_is_current = false;
}

float fractal::get_zoom_factor() const {
    return impl_->frac_params.zoom_factor;
}

void fractal::set_zoom_factor(const float z) {
    impl_->frac_params.zoom_factor = z;
    impl_->solution_is_current = false;
}

void fractal::set_x_offset(const float off_x) {
    impl_->frac_params.offset_x = off_x;
    impl_->solution_is_current = false;
}

float fractal::get_x_offset() const {
    return impl_->frac_params.offset_x;
}

void fractal::set_y_offset(const float off_y) {
    impl_->frac_params.offset_y = off_y;
    impl_->solution_is_current = false;
}

float fractal::get_y_offset() const {
    return impl_->frac_params.offset_y;
}

void fractal::set_constant(const fractal::complex_type& const_factor) {
    impl_->frac_params.C_real  = const_factor.real();
    impl_->frac_params.C_imag  = const_factor.imag();
    impl_->solution_is_current = false;
}

fractal::complex_type fractal::get_constant() const {
    return complex_type(impl_->frac_params.C_real, impl_->frac_params.C_imag);
}

void fractal::evaluate(const float delta_ms) {
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

    impl_->solution_is_current = true;
}

void fractal::draw(v8::rendering::renderer* draw_context) {
    const wchar_t* const key_settings = 
        L"Keys :\n"
        L"Zoom in/out : mouse wheel\n"
        L"Numeric + : change shape\n"
        L"Numeric * : doubles the iteration count\n"
        L"Numeric / : halve the iteration count\n"
        L"Arrow keys : move left, right, up, down\n"
        L"Q : Toggle between Julia/Mandelbrot sets\n"
        L"Home : reset and center position";

    impl_->vertexbuffer.bind_to_pipeline(draw_context);
    impl_->indexbuffer.bind_to_pipeline(draw_context);
    draw_context->ia_stage_set_input_layout(
        impl_->vert_shader.get_input_signature()
        );
    draw_context->ia_stage_set_primitive_topology_type(
        v8::rendering::PrimitiveTopology::TriangleList
        );

    impl_->frag_shader.set_uniform_block_by_name("globals", impl_->frac_params);
    impl_->frag_shader.set_resource_view("colorTable", impl_->color_table.handle());
    impl_->frag_shader.set_sampler("samplerState", impl_->sampler.internal_np_get_handle());

    impl_->vert_shader.bind_to_pipeline(draw_context);
    impl_->frag_shader.bind_to_pipeline(draw_context);

    draw_context->set_depth_stencil_state(impl_->no_depth_test);
    draw_context->draw_indexed(impl_->indexbuffer.get_element_count());
    draw_context->draw_string(
        key_settings, 16.0f, 5.0f, 60.0f, v8::math::color_rgb::C_Yellow);
}

v8_bool_t fractal::mouse_wheel_event(
    const v8_int_t rotations, const v8_int_t /*xpos*/, const v8_int_t /*ypos*/
    ) {
    float zoom_val          = get_zoom_factor();
    const float C_Exponent  = 0.55f;
    const float C_Base      = 1.5f;

    if (rotations < 0) {
        zoom_val *= pow(C_Base, C_Exponent);
    } else {
        zoom_val /= pow(C_Base, C_Exponent);
    }
    set_zoom_factor(zoom_val);
    return true;
}

v8_bool_t fractal::key_press_event(const v8_int_t key_code) {
    using namespace v8::input;

    if (Key_Sym_t::KP_Add == key_code && !impl_->frac_params.is_mandelbrot) {

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

    } else if (Key_Sym_t::Home == key_code) {

        impl_->frac_params.zoom_factor    = 1.0f;
        impl_->frac_params.offset_x       = 0.0f;
        impl_->frac_params.offset_y       = 0.0f;
        impl_->frac_params.max_iterations = 256;
        impl_->solution_is_current        = false;
        impl_->frac_params.C_origin.make_zero();

    } else if (Key_Sym_t::Key_Q  == key_code) {

        impl_->frac_params.is_mandelbrot = !impl_->frac_params.is_mandelbrot;
        impl_->solution_is_current = false;

    }

    key_stats[key_code] = true;
    return true;
}

v8_bool_t fractal::key_depress_event(const v8_int_t key_code) {
    key_stats[key_code] = false;
    return true;
}

void fractal::on_input(const v8::input_event& ev_input) {
    using namespace v8;

    if (ev_input.type == InputEventType::Mouse_Wheel) {

        mouse_wheel_event(ev_input.mouse_wheel_ev.delta, 
                          ev_input.mouse_wheel_ev.x_pos,
                          ev_input.mouse_wheel_ev.y_pos);
        return;
    }

    if (ev_input.type == InputEventType::Mouse_Button) {
        mouse_button_event(ev_input.mouse_button_ev);
        return;
    }

    if (ev_input.type == InputEventType::Key) {
        if (ev_input.key_ev.down) {
            key_press_event(ev_input.key_ev.key);
        } else {
            key_depress_event(ev_input.key_ev.key);
        }
        return;
    }
}

void fractal::mouse_button_event(const v8::mouse_button_event_t& evt) {
    if (!evt.down) {
        return;
    }

    if (evt.id == v8::MouseButton::Left) {
        const int x_pos = evt.x_pos - get_width() / 2;
        const int y_pos = -(evt.y_pos - get_height() / 2);

        impl_->frac_params.C_origin.x_ = evt.x_pos;
        impl_->frac_params.C_origin.y_ = evt.y_pos;

        mouse_wheel_event(-1, 0, 0);
        return;
    }

    if (evt.id == v8::MouseButton::Right) {
        mouse_wheel_event(1, 0, 0);
        return;
    }
}

void fractal::on_resize(const v8::resize_event& ev_resize) {
    set_width(ev_resize.width);
    set_height(ev_resize.height);
}
