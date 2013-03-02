#include <framework/app_dirs.hpp>
#include <framework/global_state.hpp>
#include <framework/rendering/renderer.hpp>
#include <framework/rendering/resource_manager.hpp>
#include <framework/win_util.hpp>
#include <framework/win_util.hpp>
#include <third_party/fast_delegate/fast_delegate.hpp>
#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>
#include <v8/math/color.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>

#include <framework/directx/directx_gpu_shader.hpp>

#include "fractal.hpp"
#include "fractal_window.hpp"

void test_gpushader() {
    v8_framework::directx::VertexShader_t vsh;

}

int WINAPI WinMain(
    HINSTANCE inst,
    HINSTANCE,
    LPSTR,
    int
    ) {
    {
        win32::scoped_mem_leak_checker enable_leak_checks;

        using namespace v8_framework;

        global::application_state global_app_state;

        global::state->Window = new fractal_window();
        v8_bool_t ret_code = global::state->Window->initialize(
            WS_OVERLAPPEDWINDOW, "FractalClass", "Fractal Explorer", 1024, 1024);
        if (!ret_code) {
            return EXIT_FAILURE;
        }

        global::state->Renderer = new render_engine::renderer();
        ret_code = global::state->Renderer->initialize(
            global::state->Window->get_handle(),
            static_cast<float>(global::state->Window->get_width()),
            static_cast<float>(global::state->Window->get_height()),
            ElementType::Unorm8, 4, 24, 8
            );
        if (!ret_code) {
            return EXIT_FAILURE;
        }

        global::state->Window->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
            v8::base::scoped_pointer_get(global::state->Renderer),
            &render_engine::renderer::on_viewport_resized);

        global::state->AssetCache = new render_engine::resource_manager(
            v8::base::scoped_pointer_get(global::state->Renderer));

        global::state->Filesys = new app_dirs();
        global::state->Filesys->initialize("D:\\games\\fractals");

        fractal f;
        g_fractal = &f;
        if (!g_fractal->initialize()) {
            return EXIT_FAILURE;
        }

        global::state->Window->Subscribers_InputEvents += 
            fastdelegate::MakeDelegate(g_fractal, &fractal::on_input);
        global::state->Window->Subscribers_ResizeEvent +=
            fastdelegate::MakeDelegate(g_fractal, &fractal::on_resize);

        global::state->Window->message_loop();
    }

    return 0;
}
