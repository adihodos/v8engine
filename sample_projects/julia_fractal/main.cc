#include <v8/v8.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>
#include <v8/base/string_util.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/fast_delegate/fast_delegate.hpp>
#include <v8/math/color.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/scene_system.hpp>

#include "fractal.hpp"
#include "fractal_window.hpp"

namespace {

class fractal_application {
public :

    v8_bool_t initialize();

    void run() {
        assert(is_valid());
        window_->message_loop();
    }

private :
    void update_scene(const float delta);

    void draw_scene();    

    bool is_valid() const {
        return window_ && rendersys_ && filesys_ && julia_;
    }

private :
    v8::base::scoped_ptr<fractal_window>                  window_;
    v8::base::scoped_ptr<v8::rendering::renderer>         rendersys_;
    v8::base::scoped_ptr<v8::filesys>                     filesys_;
    v8::base::scoped_ptr<fractal>                         julia_;
    fractal_app_context                                   app_context_;
};

v8_bool_t fractal_application::initialize() {
    assert(!is_valid());

    //
    // Initialize windowing system.
    window_ = new fractal_window();
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
        window_->get_height(),
        false);

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
        fractal_stats_str.begin(), 12.0f, 5.0f, 5.0f, v8::math::color_rgb::C_White
        );

    app_context_.Renderer->present_frame(v8::rendering::FramePresent::All);
}

}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int) {
    //
    // Must be the first constructed object to report any leaked memory.
    v8::utility::win32::scoped_mem_leak_checker leak_check_obj;
    {
        fractal_application fractal_app;
        if (!fractal_app.initialize()) {
            return -1;
        }
        fractal_app.run();
    }
    return 0;
}
