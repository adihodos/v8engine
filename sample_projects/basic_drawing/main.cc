#include <algorithm>
#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/count_of.hpp>
#include <v8/event/window_event.hpp>
#include <v8/event/input_event.hpp>
#include <v8/input/key_syms.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/fragment_shader.hpp>
#include <v8/rendering/index_buffer.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_pn.hpp>
#include <v8/rendering/vertex_pc.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/math/depth_mapping.hpp>
#include <v8/math/math_constants.hpp>
#include <v8/math/projection.hpp>
#include <v8/math/quaternion.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix3X3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/null_camera_controller.hpp>
#include <third_party/fast_delegate/fast_delegate.hpp>

template<typename T>
struct zero_initialize : public T {
    static_assert(std::is_pod<T>::value == true, "Type must be POD!");

    zero_initialize() {
        memset(this, 0, sizeof(*this));
    }
};

struct cam_init_data {
    v8::gui::basic_window* window;
};

class basic_camera_controller : public v8::scene::camera_controller {
public :
    typedef v8::scene::camera_controller                        base_class;

public :
    basic_camera_controller(v8::math::camera* cam = nullptr)
        :       base_class(cam)
    {
        memset(keystats_, 0, sizeof(keystats_));
    }

    v8_bool_t initialize(const void* params);

public :
    void update(const float delta_ms);

    void on_viewport_resized(const v8::resize_event& ev_resize);

    void on_input_event(const v8::input_event& ev_input);

private :

    void move_x_axis(const float amount);

    void move_y_axis(const float amount);

    void move_z_axis(const float amount);

private :
    struct cam_params_t {
        float   speed_x;
        float   speed_y;
        float   speed_z;

        float   speed_rot_z;
        float   angle_rot_z;

        void set_defaults() {
            speed_x = 0.025f;
            speed_y = 0.025f;
            speed_z = 0.025f;

            angle_rot_z = 0.0f;
            speed_rot_z = 0.0025f;
        }
    };

private :
    cam_params_t                        cam_params_;
    v8_bool_t                           keystats_[v8::input::Key_Sym_t::Last];
};

v8_bool_t basic_camera_controller::initialize(const void* params) {
    assert(params);
    assert(cam_ptr_);

    const cam_init_data* init_data = static_cast<const cam_init_data*>(params);
    cam_ptr_->set_projection_matrix(v8::math::projectionF::perspective(
        init_data->window->get_aspect_ratio(), 45.0f, 1.0f, 1000.0f, 
        v8::math::z_ndc_D3DF::min(), v8::math::z_ndc_D3DF::max()
        ));

    using v8::math::vector3F;
    cam_ptr_->set_view_frame(vector3F::zero, vector3F::unit_z, vector3F::unit_y,
                             vector3F::unit_x);

    cam_params_.set_defaults();
    return true;
}

void basic_camera_controller::update(const float) {
    using v8::input::Key_Sym_t;

    //v8::math::matrix_3X3F rotation_z;
    //rotation_z.make_rotation_z(cam_params_.speed_rot_z);

    //v8::math::matrix_4X4F xform_rotate(rotation_z);

    //v8::math::vector3F vec_right(cam_ptr_->get_right_vector());
    //xform_rotate.transform_affine_vector(&vec_right);
    //vec_right.normalize();

    //v8::math::vector3F vec_up(
    //    v8::math::cross_product(cam_ptr_->get_direction_vector(),
    //                            vec_right));

    //cam_ptr_->set_axes(cam_ptr_->get_direction_vector(),
    //                   vec_up,
    //                   vec_right);
    if (keystats_[Key_Sym_t::Key_A]) {
        move_x_axis(cam_params_.speed_x);
    }
    if (keystats_[Key_Sym_t::Key_D]) {
        move_x_axis(-cam_params_.speed_x);
    }
    if (keystats_[Key_Sym_t::Key_W]) {
        move_y_axis(-cam_params_.speed_y);
    }
    if (keystats_[Key_Sym_t::Key_S]) {
        move_y_axis(cam_params_.speed_z);
    }
    if (keystats_[Key_Sym_t::Key_X]) {
        move_z_axis(cam_params_.speed_z);
    }
    if (keystats_[Key_Sym_t::Key_C]) {
        move_z_axis(-cam_params_.speed_z);
    }

    if (keystats_[Key_Sym_t::PageUp]) {
        cam_ptr_->set_view_frame(v8::math::vector3F::zero,
                                 v8::math::vector3F::unit_z,
                                 v8::math::vector3F::unit_y,
                                 v8::math::vector3F::unit_x);
    }
}

void basic_camera_controller::on_viewport_resized(
    const v8::resize_event& ev_resize
    ) {
    const float aspect_ratio = static_cast<float>(ev_resize.width)
                               / static_cast<float>(ev_resize.height);
    cam_ptr_->set_projection_matrix(v8::math::projectionF::perspective(
        aspect_ratio, 45.0f, 1.0f, 1000.0f, 
        v8::math::z_ndc_D3DF::min(),
        v8::math::z_ndc_D3DF::max()
        ));
}

void basic_camera_controller::on_input_event(const v8::input_event& evt) {
    using namespace v8;

    if (evt.type != v8::InputEventType::Key) {
        return;
    }

    keystats_[evt.key_ev.key] = evt.key_ev.down;

    switch (evt.key_ev.key) {
    case v8::input::Key_Sym_t::Key_A :
        move_x_axis(cam_params_.speed_x);
        break;

    case v8::input::Key_Sym_t::Key_D :
        move_x_axis(-cam_params_.speed_x);
        break;

    case v8::input::Key_Sym_t::Key_W :
        move_y_axis(-cam_params_.speed_y);
        break;

    case v8::input::Key_Sym_t::Key_S :
        move_y_axis(cam_params_.speed_y);
        break;

    default :
        break;
    }
}

void basic_camera_controller::move_x_axis(const float amount) {
    cam_ptr_->set_origin(cam_ptr_->get_origin() 
                         + amount * cam_ptr_->get_right_vector());
}

void basic_camera_controller::move_y_axis(const float amount) {
    cam_ptr_->set_origin(cam_ptr_->get_origin()
                         + amount * cam_ptr_->get_up_vector());
}

void basic_camera_controller::move_z_axis(const float amount) {
    cam_ptr_->set_origin(cam_ptr_->get_origin() 
                         + amount * cam_ptr_->get_direction_vector());
}

struct DrawingContext {
    v8::rendering::renderer*        Renderer;
    v8::math::matrix_4X4F           ProjectionViewXForm;
};

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

v8_bool_t Triangle::Initialize(const InitContext* init_context) {
    if (is_valid()) {
        return true;
    }

    using v8::rendering::vertex_pc;
    using v8::math::vector3F;
    using v8::math::color_rgb;
    const vertex_pc kTriangleVertices[] = {
        vertex_pc(vector3F(0.0f, 0.0f, 3.0f), color_rgb::C_Red),
        vertex_pc(vector3F(1.0f, 1.0f, 3.0f), color_rgb::C_Green),
        vertex_pc(vector3F(1.0f, 0.0f, 3.0f), color_rgb::C_Blue)
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
        init_context->FileSystem->make_shader_path("basic_coloring");
    shader_info.shader_root_directory = init_context->FileSystem->get_dir_path(
        v8::filesys::Dir::Shaders
        );
    shader_info.entrypoint            = "color_vertex_pc";
    shader_info.shader_model          = "ps_5_0";

    if (!fragshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    shader_info.name_or_source =
        init_context->FileSystem->make_shader_path("basic_transform");
    shader_info.entrypoint = "xform_vertex_pc";
    shader_info.shader_model = "vs_5_0";
    if (!vertexshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    //
    // Mark object as valid.
    is_valid_ = true;
    return true;
}

void Triangle::Update(const float /*delta*/) {
}

void Triangle::Draw(const DrawingContext* draw_ctx) {
    assert(is_valid());

    indexbuffer_.bind_to_pipeline(draw_ctx->Renderer);
    vertexbuffer_.bind_to_pipeline(draw_ctx->Renderer);
    draw_ctx->Renderer->ia_stage_set_input_layout(
        vertexshader_.get_input_signature()
        );
    draw_ctx->Renderer->ia_stage_set_primitive_topology_type(
        v8::rendering::PrimitiveTopology::TriangleList
        );
    
    vertexshader_.set_uniform_by_name(
        "world_view_projection", draw_ctx->ProjectionViewXForm
        );

    vertexshader_.bind_to_pipeline(draw_ctx->Renderer);
    fragshader_.bind_to_pipeline(draw_ctx->Renderer);

    draw_ctx->Renderer->draw_indexed(indexbuffer_.get_element_count());
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
    v8::base::scoped_ptr<v8::math::camera>              camera_;
    v8::base::scoped_ptr<v8::scene::camera_controller>  cam_controller_;
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
    filesys_->initialize("D:\\games\\basic_drawing");

    //
    // Register for update and draw notifications.
    window_->Delegates_UpdateEvent += fastdelegate::MakeDelegate(
        this, &basic_drawing_app::update
        );
    window_->Delegates_DrawEvent += fastdelegate::MakeDelegate(
        this, &basic_drawing_app::draw
        );

    //
    // Setup camera and camera controller.
    camera_ = new v8::math::camera();
    cam_controller_ = new basic_camera_controller(
        v8::base::scoped_pointer_get(camera_));

    cam_init_data init_cam_data = { v8::base::scoped_pointer_get(window_) };
    if (!cam_controller_->initialize(&init_cam_data)) {
        return false;
    }

    //
    // Register the controller for window resize and input event notifications
    // so that the camera is properly set up.
    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(cam_controller_),
        &v8::scene::camera_controller::on_viewport_resized
        );
    window_->Subscribers_InputEvents += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(cam_controller_),
        &v8::scene::camera_controller::on_input_event
        );

    //
    // Create objects.
    initialize_objects();
    return true;
}

void basic_drawing_app::update(const float delta_tm) {
    using namespace std;

    cam_controller_->update(delta_tm);

    for_each(begin(geometric_objects_), end(geometric_objects_),
                   [delta_tm](IGeometryShape* geometry_shape) {
        geometry_shape->Update(delta_tm);
    });
}

void basic_drawing_app::draw() {
    assert(is_valid());
    
    rendersys_->reset_blending_state();
    rendersys_->reset_depth_stencil_state();
    rendersys_->clear_depth_stencil();
    rendersys_->clear_backbuffer();

    DrawingContext draw_context = {
        v8::base::scoped_pointer_get(rendersys_),
        camera_->get_projection_wiew_transform()
    };

    const DrawingContext* kDrawCtx = &draw_context;

    using namespace std;
    for_each(begin(geometric_objects_), end(geometric_objects_),
             [kDrawCtx](IGeometryShape* shape) {
        shape->Draw(kDrawCtx);
    });
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