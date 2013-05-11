#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/count_of.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_pn.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/utility/win_util.hpp>
#include <third_party/fast_delegate/fast_delegate.hpp>

struct DrawingContext {};

struct InitContext {
    v8::rendering::renderer*    Renderer;
    v8::filesys*                FileSystem;

    InitContext() {
        memset(this, 0, sizeof(*this));
    }
};

template<typename T>
struct DefaultDeleter {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

class IGeometryShape {
public :
    virtual ~IGeometryShape() {}

    virtual v8_bool_t Initialize(const InitContext* init_context) = 0;

    virtual void Update(const float delta) = 0;

    virtual void Draw(const DrawingContext* draw_context) = 0;
};

class Triangle : public IGeometryShape {
public :
    Triangle();

    ~Triangle();

public :
    v8_bool_t Initialize(const InitContext* init_context);

    void Update(const float delta);

    void Draw(const DrawingContext* draw_context);

private :
    v8_bool_t is_valid() const {
        return is_valid_;
    }

private :
    v8::rendering::index_buffer     indexbuffer_;
    v8::rendering::vertex_buffer    vertexbuffer_;
    v8::rendering::vertex_shader    vertexshader_;
    v8::rendering::fragment_shader  fragshader_;
    v8_bool_t                       is_valid_;

private :
    NO_CC_ASSIGN(Triangle);
};

Triangle::Triangle() : is_valid_(false) {}

Triangle::~Triangle() {
}

void Triangle::Update(const float /*delta*/) {
}

void Triangle::Draw(const DrawingContext*) {
}

v8_bool_t Triangle::Initialize(const InitContext* init_context) {
    if (is_valid()) {
        return true;
    }

    using v8::rendering::vertex_pn;
    const vertex_pn kTriangleVertices[] = {
        vertex_pn(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
        vertex_pn(10.0f, 10.0f, 0.0f, 0.0f, 0.0f, -1.0f),
        vertex_pn(10.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f)
    };

    v8_bool_t ret_code = vertexbuffer_.initialize(
        init_context->Renderer, dimension_of(kTriangleVertices),
        sizeof(kTriangleVertices[0]), kTriangleVertices
        );
    if (!ret_code) {
        return false;
    }

    const v8_uint16_t kTriangleIndices[] = { 0, 1, 2 };
    ret_code = indexbuffer_.initialize(
        init_context->Renderer, dimension_of(kTriangleIndices),
        sizeof(kTriangleIndices[0]), kTriangleIndices
        );
    if (!ret_code) {
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
        init_context->FileSystem->make_shader_path("fractals");
    shader_info.shader_root_directory = init_context->FileSystem->get_dir_path(
        v8::filesys::Dir::Shaders
        );
    shader_info.entrypoint            = "ps_julia";
    shader_info.shader_model          = "ps_5_0";

    if (!fragshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    //
    // Mark object as valid.
    is_valid_ = true;
    return true;
}

class basic_drawing_app {
public :
    v8_bool_t initialize();

    void run() {
        assert(is_valid());
        window_->message_loop(v8::base::scoped_pointer_get(rendersys_));
    }

private :
    void update(const float delta_tm);

    void draw();

    void initialize_objects();

    v8_bool_t is_valid() const {
        return window_ && rendersys_;
    }

private :
    typedef DefaultDeleter<IGeometryShape>          IGeometryShapeDeleter;
    typedef std::vector<IGeometryShape*>            IGeometryShapeList;
    typedef v8::base::sequence_container_veneer
    <
        IGeometryShapeList,
        IGeometryShapeDeleter
    >                                               ScopedIGeometryShapeList;

private :
    v8::base::scoped_ptr<v8::gui::basic_window>         window_;
    v8::base::scoped_ptr<v8::rendering::renderer>       rendersys_;
    v8::base::scoped_ptr<v8::filesys>                   filesys_;
    ScopedIGeometryShapeList                            geometric_objects_;
};

v8_bool_t basic_drawing_app::initialize() {
    assert(!is_valid());
    if (is_valid()) {
        return true;
    }

    //
    // Initialize windowing system.
    window_ = new v8::gui::basic_window();
    v8::gui::window_init_params_t win_init_params = {
            1024, 1024, "Basic Drawing with D3D"
    };

    if (!window_->initialize(win_init_params)) {
        return false;
    }

    //
    // Initialize rendering system.
    rendersys_ = new v8::rendering::renderer();

    v8::rendering::render_init_params graphics_init_params;
    graphics_init_params.target_window        = window_->get_handle();
    graphics_init_params.antialiasing         = false;
    graphics_init_params.buffer_element_type  = 
        v8::rendering::ElementType::Unorm8;
    graphics_init_params.buffer_element_count = 4;
    graphics_init_params.depth_bits           = 24;
    graphics_init_params.stencil_bits         = 8;
    graphics_init_params.full_screen          = false;
    graphics_init_params.handle_full_screen   = true;
    graphics_init_params.render_targets_count = 1;
    graphics_init_params.width                = 1024;
    graphics_init_params.height               = 1024;
    graphics_init_params.clear_color          = 
        v8::math::color_rgb::C_Darkorange;

    if (!rendersys_->initialize(graphics_init_params)) {
        return false;
    }

    //
    // Rendering system must receive window resize events for proper viewport 
    // setup.
    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(rendersys_), 
        &v8::rendering::renderer::on_viewport_resized
        );

    //
    // Setup filesystem manager.
    filesys_ = new v8::filesys();
    filesys_->initialize("D:\\games\\fractals");

    //
    // Register for update and draw notifications.
    window_->Delegates_UpdateEvent += fastdelegate::MakeDelegate(
        this, &basic_drawing_app::update
        );
    window_->Delegates_DrawEvent += fastdelegate::MakeDelegate(
        this, &basic_drawing_app::draw
        );

    //
    // Create objects.
    initialize_objects();
    return true;
}

void basic_drawing_app::update(const float) {
}

void basic_drawing_app::draw() {
    assert(is_valid());
    
    rendersys_->reset_blending_state();
    rendersys_->reset_depth_stencil_state();
    rendersys_->clear_depth_stencil();
    rendersys_->clear_backbuffer();
}

void basic_drawing_app::initialize_objects() {
    const int kNumObjects = 1;
    geometric_objects_.reserve(kNumObjects);

    InitContext obj_init_context;
    obj_init_context.Renderer   = v8::base::scoped_pointer_get(rendersys_);
    obj_init_context.FileSystem = v8::base::scoped_pointer_get(filesys_);

    v8::base::scoped_ptr<Triangle> triangle(new Triangle());
    if (!triangle->Initialize(&obj_init_context)) {
        return;
    }
    geometric_objects_.push_back(v8::base::scoped_pointer_release(triangle));
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int) {
    v8::utility::win32::scoped_mem_leak_checker memchecker;
    {
        basic_drawing_app application;
        if (!application.initialize()) {
            return -1;
        }
        application.run();
    }
    return 0;
}