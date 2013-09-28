#include <cstdlib>
#include <ctime>

#include <v8/v8.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/string_util.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/rendering/render_init_params.hpp>
#include <v8/rendering/texture_descriptor.hpp>
#include <v8/rendering/texture.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/fast_delegate/fast_delegate.hpp>
#include <v8/math/color.hpp>
#include <v8/math/color_palette_generator.hpp>
#include <v8/math/random/random.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/scene_system.hpp>

#include "fractal.hpp"

namespace {

using namespace v8::base;
//using namespace v8::math;
using namespace v8::rendering;

v8_bool_t write_palette_as_dds(
    const char*                                         file_name,
    const v8::base::array_proxy<v8::math::rgb_color>&   palette,
    const v8::rendering::renderer&                      rsys) 
{
    textureDescriptor_t tex_desc((v8_uint32_t) palette.length(),
                                 1,
                                 textureType_t::Tex1D,
                                 1,
                                 4,
                                 ElementType::Float32,
                                 BindingFlag::ShaderResource,
                                 ResourceUsage::Default,
                                 CPUAccess::None,
                                 false);

    const void* tex_data = palette.base();
    texture palette_texture(tex_desc, rsys, &tex_data);

    if (!palette_texture) {
        return false;
    }

    return palette_texture.write_to_file(file_name, rsys);
}

class fractal_application {
public :

    v8_bool_t initialize();

    void run() {
        assert(is_valid());
        window_->message_loop();
    }

    void boogie_boogie();

private :
    void update_scene(const float delta);

    void draw_scene();    

    bool is_valid() const {
        return window_ && rendersys_ && filesys_ && julia_;
    }

private :
    v8::base::scoped_ptr<v8::gui::basic_window>             window_;
    v8::base::scoped_ptr<v8::rendering::renderer>           rendersys_;
    v8::base::scoped_ptr<v8::filesys>                       filesys_;
    v8::base::scoped_ptr<fractal>                           julia_;
    v8::math::random                                        rng_;
    fractal_app_context                                     app_context_;
};

v8_bool_t fractal_application::initialize() {
    assert(!is_valid());

    //
    // Initialize windowing system.
    window_ = new v8::gui::basic_window;
    v8::gui::window_init_params_t win_init_params = {
        1024, 1024, "Julia Fractal Visualiser"
    };

    if (!window_->initialize(win_init_params)) {
        return false;
    }
    
    //
    //  Initialize the rendering system.
    rendersys_ = new v8::rendering::renderer();
    v8::rendering::renderOptions_t initialization_params(
        window_->get_handle(),
        window_->get_width(), 
        window_->get_height());

    if (!rendersys_->initialize(initialization_params)) {
        return false;
    }

    //
    // Rendering system must receive window resize events for proper viewport
    // resizing.
    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(rendersys_), 
        &v8::rendering::renderer::on_viewport_resized
        );

    //
    // Initialize file system manager.
    filesys_ = new v8::filesys();
    //
    // TODO : fix hard coded path, it sucks.
    const char* const app_data_dir = "D:\\games\\fractals";
    filesys_->initialize(app_data_dir);

    //
    // Setup application context data.
    app_context_.Window   = v8::base::scoped_pointer_get(window_);
    app_context_.Renderer = v8::base::scoped_pointer_get(rendersys_);
    app_context_.Filesys  = v8::base::scoped_pointer_get(filesys_);

    //
    // initialize the scene (its just the fractal object)
    julia_ = new fractal();
    if (!julia_->initialize(app_context_)) {
        return false;
    }

    //
    // The fractal responds to certain key/mouse input events and
    // resize events, so register the fractal object for those.
    window_->Subscribers_InputEvents += 
        fastdelegate::MakeDelegate(v8::base::scoped_pointer_get(julia_), 
                                   &fractal::on_input);
    window_->Subscribers_ResizeEvent +=
        fastdelegate::MakeDelegate(v8::base::scoped_pointer_get(julia_), 
                                   &fractal::on_resize);

    //
    // Since this class also acts as "the scene" for all the objects,
    // it is registered with the windowing system to receive update(tick) 
    // and draw events notifications, so that in turn can pass them to the
    // objects.
    window_->Delegates_DrawEvent += fastdelegate::MakeDelegate(
        this, &fractal_application::draw_scene
        );
    window_->Delegates_UpdateEvent += fastdelegate::MakeDelegate(
        this, &fractal_application::update_scene
        ) ;

    return true;
}

void fractal_application::update_scene(const float delta_tm) {
    assert(is_valid());
    julia_->evaluate(delta_tm);
}

void fractal_application::draw_scene() {
    assert(is_valid());

    app_context_.Renderer->clear_backbuffer();
    app_context_.Renderer->clear_depth_stencil();

    julia_->draw(app_context_.Renderer);

    const wchar_t* const C_fractal_stats_fmt_str = 
        L"Zoom : %4.6f \n"
        L"Iterations : %d \n"
        L"Offset : [%4.4f, %4.4f] \n"
        L"Constant : [%4.6f, %4.6f]";

    v8::base::auto_buffer<wchar_t> fractal_stats_str;
    v8::base::snwprintf(fractal_stats_str.begin(), 
                        fractal_stats_str.size(), 
                        C_fractal_stats_fmt_str, 
                        julia_->get_zoom_factor(),
                        julia_->get_iteration_count(),
                        julia_->get_x_offset(),
                        julia_->get_y_offset(),
                        julia_->get_constant().real(),
                        julia_->get_constant().imag());

    app_context_.Renderer->draw_string(
        fractal_stats_str.begin(), 12.0f, 5.0f, 5.0f, v8::math::rgb_color::C_White
        );

    app_context_.Renderer->present_frame(v8::rendering::FramePresent::All);
}

void fractal_application::boogie_boogie() {
    v8::math::rgb_color uniform_colors[16];
    array_proxy<v8::math::rgb_color> color_arr(uniform_colors);

    for (v8_int_t i = 0; i < 16; ++i) {
        v8::math::procedural_palette::gen_uniform_colors(color_arr);

        char file_path [256];
        snprintf(file_path, dimension_of(file_path), "C:\\temp\\palette_uniform%d.dds", i);
        write_palette_as_dds(file_path, color_arr, *rendersys_);
    }

    const v8::math::rgb_color base_colors [] = {
        v8::math::rgb_color(1.0f, 0.5f, 0.0f),
        v8::math::rgb_color(0.5f, 1.0f, 0.25f),
        v8::math::rgb_color(0.25f, 0.5f, 1.0f),
    };

    for (v8_size_t idx = 0; idx < dimension_of(base_colors); ++idx) {
        v8::math::procedural_palette::gen_random_walk_colors(
            base_colors[idx],
            0.2f,
            0.4f,
            color_arr);

        char file_path [256];
        snprintf(file_path, 
                 dimension_of(file_path), 
                 "C:\\temp\\palette_rand_walk%u.dds", 
                 idx);

        write_palette_as_dds(file_path, color_arr, *rendersys_);
    }

    v8::math::rgb_color palette2 [128];
    v8::base::array_proxy<v8::math::rgb_color> arrp(palette2);

    using v8::math::procedural_palette;

    auto color_check_fn = [](const v8::math::rgb_color& rgb) -> float {
        using namespace v8::math;
        color_hcl hcl;
        rgb_to_hcl(rgb, &hcl);

        return hcl.Elements[0] >= 0.0f && hcl.Elements[0] <= 250.0f
            && hcl.Elements[1] >= 2.0f && hcl.Elements[1] <= 3.0f
            && hcl.Elements[2] >= 0.0f && hcl.Elements[2] <= 1.5f;
    };

    procedural_palette::generate_color_palette(color_check_fn,
                                               true,
                                               50,
                                               false,
                                               arrp);

    write_palette_as_dds("C:\\temp\\color_palette.dds", arrp, *rendersys_);
}

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    //
    // Must be the first constructed object to report any leaked memory.
    v8::utility::win32::scoped_mem_leak_checker leak_check_obj;
    {
        fractal_application fractal_app;
        if (!fractal_app.initialize()) {
            return -1;
        }
        //fractal_app.boogie_boogie();
        fractal_app.run();
    }
    return 0;
}
