#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <array>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <windows.h>

#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include <v8/base/count_of.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/fixed_array.hpp>
#include <v8/base/scoped_handle.hpp>
#include <v8/base/handle_traits/win32_device_context.hpp>
#include <v8/base/handle_traits/win32_gdiobject.hpp>
#include <v8/base/handle_traits/win32_opengl_context.hpp>
#include <v8/base/handle_traits/win32_window.hpp>
#include <v8/base/shims/scoped_handle.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/base/win32_utils.hpp>
#include <v8/event/window_event.hpp>
#include <v8/event/input_event.hpp>
#include <v8/event/window_event.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/input/key_syms.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/math/color.hpp>

class scoped_wgl_context_changer {
public :
    scoped_wgl_context_changer(HDC dev_context, HGLRC wgl_context)
        :       dev_context_(dev_context)
            ,   wgl_context_(wgl_context)
            ,   succeeded_(wglMakeCurrent(dev_context_, wgl_context_))
    {}

    ~scoped_wgl_context_changer() {
        if (succeeded_) {
            wglMakeCurrent(dev_context_, nullptr);
        }
    }

    operator int v8::base::operator_bool::*() const {
        return succeeded_ ? &v8::base::operator_bool::a_member : nullptr;
    }

private :
    HDC         dev_context_;
    HGLRC       wgl_context_;
    v8_bool_t   succeeded_;
};

///
/// \brief Parameters for rrender device initialization.
struct render_init_params {
    ///< Destination output window.
    void*       target_window;

    ///< Window width.
    v8_int_t    width;

    ///< Window height.
    v8_int_t    height;

    ///< True if rendering full screen.
    v8_bool_t   full_screen;

    ///< True if pressing of ALT-ENTER is automatically handled. Set to false
    ///< to handle it in application.
    v8_bool_t   handle_full_screen;

    ///< Enable support for antialiasing (not yet implemented).
    v8_bool_t   antialiasing;

    ///< Element type for the back buffer (see ElementType).
    v8_int_t    buffer_element_type;

    ///< Number of components of a single backbuffer entry.
    v8_int_t    buffer_element_count;

    ///< Number of render targets (only 1 atm).
    v8_int_t    render_targets_count;

    ///< Number of bits for depth testing.
    v8_int_t    depth_bits;

    ///< Number of bits for stenciling.
    v8_int_t    stencil_bits;

    ///< Backbuffer clear color.
    v8::math::color_rgb clear_color;
};

struct wgl {
    static PFNWGLCHOOSEPIXELFORMATARBPROC       ChoosePixelFormatARB;
    static PFNWGLCREATECONTEXTATTRIBSARBPROC    CreateContextAttribsARB;
    static PFNWGLSWAPINTERVALEXTPROC            SwapIntervalEXT;

    static v8_bool_t load_extensions();
};

PFNWGLCHOOSEPIXELFORMATARBPROC wgl::ChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wgl::CreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC wgl::SwapIntervalEXT;

v8_bool_t
wgl::load_extensions() {
    assert(!wgl::ChoosePixelFormatARB);
    assert(!wgl::CreateContextAttribsARB);

    ChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
    if (!ChoosePixelFormatARB) {
        return false;
    }

    CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
    if (!CreateContextAttribsARB) {
        return false;
    }

    SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
    if (!SwapIntervalEXT) {
        return false;
    }

    return true;
}

namespace gl {

/// \name   Vertex arrays.
/// @{

    PFNGLBINDVERTEXARRAYPROC                glBindVertexArray;
    PFNGLDELETEVERTEXARRAYSPROC             glDeleteVertexArrays;
    PFNGLGENVERTEXARRAYSPROC                glGenVertexArrays;

/// @}

/// \name   Buffer functions.
/// @{

    PFNGLGENBUFFERSPROC                     glGenBuffers;
    PFNGLBINDBUFFERPROC                     glBindBuffer;
    PFNGLBUFFERDATAPROC                     glBufferData;
    PFNGLDELETEBUFFERSPROC                  glDeleteBuffers;

/// @}

/// \name   Vertex attributes.
/// @{

    PFNGLENABLEVERTEXATTRIBARRAYPROC        glEnableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC            glVertexAttribPointer;
    PFNGLBINDATTRIBLOCATIONPROC             glBindAttribLocation;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC       glDisableVertexAttribArray;

/// @}

/// \name   Shader related functions.
/// @{

    PFNGLATTACHSHADERPROC                   glAttachShader;
    PFNGLDETACHSHADERPROC                   glDetachShader;
    PFNGLCREATESHADERPROC                   glCreateShader;
    PFNGLCOMPILESHADERPROC                  glCompileShader;
    PFNGLCREATEPROGRAMPROC                  glCreateProgram;
    PFNGLDELETEPROGRAMPROC                  glDeleteProgram;
    PFNGLDELETESHADERPROC                   glDeleteShader;
    PFNGLGETATTRIBLOCATIONPROC              glGetAttribLocation;
    PFNGLGETPROGRAMINFOLOGPROC              glGetProgramInfoLog;
    PFNGLGETPROGRAMIVPROC                   glGetProgramiv;
    PFNGLGETSHADERINFOLOGPROC               glGetShaderInfoLog;
    PFNGLGETSHADERIVPROC                    glGetShaderiv;
    PFNGLLINKPROGRAMPROC                    glLinkProgram;
    PFNGLSHADERSOURCEPROC                   glShaderSource;
    PFNGLUSEPROGRAMPROC                     glUseProgram;
    PFNGLGETUNIFORMLOCATIONPROC             glGetUniformLocation;
    PFNGLUNIFORMMATRIX4FVPROC               glUniformMatrix4fv;
    PFNGLUNIFORM1IPROC                      glUniform1i;
    PFNGLUNIFORM3FVPROC                     glUniform3fv;
    PFNGLUNIFORM4FVPROC                     glUniform4fv;
    PFNGLCREATESHADERPROGRAMVPROC           glCreateShaderProgramv;

/// @}

/// \name Texture functions.
/// @{

    PFNGLACTIVETEXTUREPROC                  glActiveTexture;
    PFNGLGENERATEMIPMAPPROC                 glGenerateMipmap;

/// @}

    v8_bool_t load_extentions();
}

v8_bool_t
gl::load_extentions() {
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    if(!glAttachShader) {
        return false;
    }

    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    if(!glBindBuffer) {
        return false;
    }

    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    if(!glBindVertexArray) {
        return false;
    }

    glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData");
    if(!glBufferData) {
        return false;
    }

    glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader");
    if(!glCompileShader) {
        return false;
    }

    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    if(!glCreateProgram) {
        return false;
    }

    glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader");
    if(!glCreateShader) {
        return false;
    }

    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress("glDeleteBuffers");
    if(!glDeleteBuffers) {
        return false;
    }

    glDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
    if(!glDeleteProgram) {
        return false;
    }

    glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
    if(!glDeleteShader) {
        return false;
    }

    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) wglGetProcAddress("glDeleteVertexArrays");
    if(!glDeleteVertexArrays) {
        return false;
    }

    glDetachShader = (PFNGLDETACHSHADERPROC) wglGetProcAddress("glDetachShader");
    if(!glDetachShader) {
        return false;
    }

    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray");
    if(!glEnableVertexAttribArray) {
        return false;
    }

    glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers");
    if(!glGenBuffers) {
        return false;
    }

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) wglGetProcAddress("glGenVertexArrays");
    if(!glGenVertexArrays) {
        return false;
    }

    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress("glGetAttribLocation");
    if(!glGetAttribLocation) {
        return false;
    }

    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog");
    if(!glGetProgramInfoLog) {
        return false;
    }

    glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv");
    if(!glGetProgramiv) {
        return false;
    }

    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");
    if(!glGetShaderInfoLog) {
        return false;
    }

    glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv");
    if(!glGetShaderiv) {
        return false;
    }

    glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram");
    if(!glLinkProgram) {
        return false;
    }

    glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource");
    if(!glShaderSource) {
        return false;
    }

    glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram");
    if(!glUseProgram) {
        return false;
    }

    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress("glVertexAttribPointer");
    if(!glVertexAttribPointer) {
        return false;
    }

    glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) wglGetProcAddress("glBindAttribLocation");
    if(!glBindAttribLocation) {
        return false;
    }

    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation");
    if(!glGetUniformLocation) {
        return false;
    }

    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
    if(!glUniformMatrix4fv) {
        return false;
    }

    glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");
    if(!glActiveTexture) {
        return false;
    }

    glUniform1i = (PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
    if(!glUniform1i) {
        return false;
    }

    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
    if(!glGenerateMipmap) {
        return false;
    }

    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray");
    if(!glDisableVertexAttribArray) {
        return false;
    }

    glUniform3fv = (PFNGLUNIFORM3FVPROC) wglGetProcAddress("glUniform3fv");
    if(!glUniform3fv) {
        return false;
    }

    glUniform4fv = (PFNGLUNIFORM4FVPROC) wglGetProcAddress("glUniform4fv");
    if(!glUniform4fv) {
        return false;
    }

    return true;
}

namespace {

LRESULT
simple_window_procedure(HWND        wnd,
                        UINT        msg,
                        WPARAM      w_param,
                        LPARAM      l_param)
{
    return DefWindowProc(wnd, msg, w_param, l_param);
}

HWND
create_simple_window() {
    const char* const kWindowClassName = "__##@@TemporaryWindowClass@@##__$$__";

    WNDCLASS wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = simple_window_procedure;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    wc.lpszMenuName  = 0;
    wc.lpszClassName = kWindowClassName;

    if (!RegisterClass(&wc)) {
        return nullptr;
    }

    return CreateWindow(kWindowClassName,
                        "",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        nullptr,
                        nullptr,
                        GetModuleHandle(nullptr),
                        nullptr);
}

} // anonymous namespace

class opengl_renderer {

/// \name   Construction/init.
/// @{

public :

    opengl_renderer()
        :       output_window_(nullptr)
            ,   gl_context_(nullptr)
            ,   win_width_(0)
            ,   win_height_(0)
    {}

    ~opengl_renderer() {
        using namespace v8::base;

        if (gl_context_) {
            wglMakeCurrent(raw_handle(device_context_), nullptr);
        }
    }

    v8_bool_t
    initialize(const render_init_params& init_params);

private :

    v8_bool_t
    load_opengl_function_pointers();

    v8_bool_t
    initialize_opengl_context(const render_init_params& init_params);

/// @}

/// \name   External events.
/// @{

public :

    void on_viewport_resized(const v8::resize_event& ev_resize);

/// @}

/// \name   Attributes.
/// @{

public :

    void set_clear_color(const v8::math::color_rgb& clear_color) {
        clear_color_ = clear_color;
    }

    void clear_backbuffer() {
        assert(is_valid());
        glClearColor(clear_color_.Red, clear_color_.Green, clear_color_.Blue, clear_color_.Alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void clear_depth_stencil() {
        assert(is_valid());
        glClearDepth(1.0f);
        glClearStencil(0);
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

/// @}

/// \name   Operations.
/// @{

public :

    void present_frame() {
        assert(is_valid());
        SwapBuffers(v8::base::raw_handle(device_context_));
    }

/// @}
private :

    v8_bool_t is_valid() const noexcept {
        return device_context_ && gl_context_;
    }

private :

    HWND                                                output_window_;

    v8::base::scoped_handle
    <
        v8::base::win_device_context
    >                                                   device_context_;

    v8::base::scoped_handle
    <
        v8::base::wgl_context
    >                                                   gl_context_;

    v8_uint32_t                                         win_width_;
    v8_uint32_t                                         win_height_;
    v8::math::color_rgb                                 clear_color_;

private :

    NO_CC_ASSIGN(opengl_renderer);
};

void
opengl_renderer::on_viewport_resized(const v8::resize_event& ev_resize) {
    assert(is_valid());
    assert(ev_resize.width > 0);
    assert(ev_resize.height > 0);

    win_width_ = static_cast<v8_uint32_t>(ev_resize.width);
    win_height_ = static_cast<v8_uint32_t>(ev_resize.height);
    glViewport(0, 0, win_width_, win_height_);
}

v8_bool_t
opengl_renderer::load_opengl_function_pointers() {
    using namespace v8::base;

    scoped_handle<win_window> temp_window(create_simple_window());
    if (!temp_window) {
        return false;
    }

    scoped_handle<win_device_context> device_context(GetDC(raw_handle(temp_window)));
    if (!device_context) {
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0,
        0,
        0
    };

    const int pixel_format_idx = ChoosePixelFormat(raw_handle(device_context), &pfd);
    if (pixel_format_idx == 0) {
        return false;
    }

    if (!SetPixelFormat(raw_handle(device_context), pixel_format_idx, &pfd)) {
        return false;
    }

    //
    // Create context to load OpenGL function pointers.
    scoped_handle<wgl_context> temp_wgl_context(
        wglCreateContext(raw_handle(device_context)));

    if (!temp_wgl_context) {
        return false;
    }

    //
    // OpenGL context must be set as current before loading anything.
    scoped_wgl_context_changer context_change { raw_handle(device_context),
                                                raw_handle(temp_wgl_context) };
    if (!context_change) {
        return false;
    }

    if (!wgl::load_extensions()) {
        return false;
    }

    if (!gl::load_extentions()) {
        return false;
    }

    return true;
}

v8_bool_t
opengl_renderer::initialize_opengl_context(const render_init_params& init_params) {
    if (gl_context_) {
        return true;
    }

    output_window_  = static_cast<HWND>(init_params.target_window);
    win_width_      = init_params.width;
    win_height_     = init_params.height;
    clear_color_    = init_params.clear_color;

    device_context_ = GetDC(output_window_);
    if (!device_context_) {
        return false;
    }

    using namespace v8::base;

    const v8_int_t pixel_format_attribs[] = {
        WGL_SUPPORT_OPENGL_ARB, true,
        WGL_DRAW_TO_WINDOW_ARB, true,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
        WGL_DOUBLE_BUFFER_ARB, true,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0
    };

    fixed_array<v8_int32_t, 16> supported_pixel_formats;
    v8_uint32_t num_supp_formats = 0;

    const BOOL ret_code = wgl::ChoosePixelFormatARB(raw_handle(device_context_),
                                                    pixel_format_attribs,
                                                    nullptr,
                                                    supported_pixel_formats.size(),
                                                    supported_pixel_formats.data(),
                                                    &num_supp_formats);

    if ((ret_code == FALSE) || (num_supp_formats == 0)) {
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd;
    BOOL ret_val;
    WIN32_CHK_FNCALL(&ret_val, SetPixelFormat(raw_handle(device_context_),
                                              supported_pixel_formats[0],
                                              &pfd));
    if (!ret_val) {
        return false;
    }

    const v8_int_t gl_context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,  4,
        WGL_CONTEXT_MINOR_VERSION_ARB,  3,
        WGL_CONTEXT_FLAGS_ARB,          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        WGL_CONTEXT_PROFILE_MASK_ARB,   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    gl_context_ = wgl::CreateContextAttribsARB(raw_handle(device_context_),
                                               nullptr,
                                               gl_context_attribs);
    if (!gl_context_) {
        return false;
    }

    wglMakeCurrent(raw_handle(device_context_), raw_handle(gl_context_));
    return true;
}

v8_bool_t
opengl_renderer::initialize(const render_init_params &init_params) {
    if (!load_opengl_function_pointers()) {
        return false;
    }

    if (!initialize_opengl_context(init_params)) {
        return false;
    }

    glViewport(0, 0, win_width_, win_height_);
    return true;
}

GLuint create_compute_shader(const char** source_code, const v8_size_t cnt) {
    GLuint shader = gl::glCreateShaderProgramv(GL_COMPUTE_SHADER, cnt, source_code);
}

class compute_shader {
private :
    GLuint  shader_handle_;

public :
    compute_shader() : shader_handle_(0) {}

    v8_bool_t initialize(const char* filename) {
        if (shader_handle_ != 0) {
            return true;
        }

        platformstl::memory_mapped_file mmfile(filename);
        if (!filename) {
            return false;
        }

        const char* shader_code[] { static_cast<const char*>(mmfile.data()) };
        shader_handle_ = gl::glCreateShaderProgramv(GL_COMPUTE_SHADER,
                                                    1,
                                                    shader_code);
        return shader_handle_ != 0;
    }

    GLuint get_handle() const noexcept {
        return shader_handle_;
    }
};

struct app_context_t {
    app_context_t(opengl_renderer* renderer) : gl_renderer_(renderer) {}

    void handle_drawing_request() {
        gl_renderer_->clear_depth_stencil();
        gl_renderer_->clear_backbuffer();
        gl_renderer_->present_frame();
    }

    opengl_renderer*    gl_renderer_;
};

int WINAPI WinMain(HINSTANCE,
                   HINSTANCE,
                   LPSTR,
                   int)
{
    v8::gui::basic_window main_window;
    if (!main_window.initialize({ 1024, 768, "OpenGL Window" })) {
        v8::utility::formatted_msg_box("Error",
                                       MB_ICONERROR,
                                       "Failed to create main window!");
        return EXIT_FAILURE;
    }

    render_init_params init_params {
        main_window.get_handle(),
        1024,
        768,
        false,
        false,
        false,
        0,
        4,
        1,
        24,
        8,
        v8::math::color_rgb::C_OrangeRed
    };

    opengl_renderer renderer;
    if (!renderer.initialize(init_params)) {
        v8::utility::formatted_msg_box("Error",
                                       MB_ICONERROR,
                                       "Failed to initialize renderer!");
        return EXIT_FAILURE;
    }

    app_context_t app_ctx(&renderer);

    main_window.Delegates_DrawEvent += fastdelegate::MakeDelegate(
                &app_ctx, &app_context_t::handle_drawing_request);
    main_window.message_loop();

    return 0;
}
