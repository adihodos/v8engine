#include <algorithm>
#include <v8/v8.hpp>
#include <v8/base/array_proxy.hpp>
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
#include <v8/rendering/material.hpp>
#include <v8/rendering/shader_info.hpp>
#include <v8/rendering/vertex_buffer.hpp>
#include <v8/rendering/vertex_pn.hpp>
#include <v8/rendering/vertex_pc.hpp>
#include <v8/rendering/vertex_shader.hpp>
#include <v8/utility/geometry_importer.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/math/depth_mapping.hpp>
#include <v8/math/light.hpp>
#include <v8/math/math_constants.hpp>
#include <v8/math/projection.hpp>
#include <v8/math/quaternion.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix3X3.hpp>
#include <v8/math/matrix4X4.hpp>
#include <v8/scene/null_camera_controller.hpp>
#include <v8/scene/cam_controller_spherical_coordinates.hpp>
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

struct DrawingContext {
    DrawingContext(const v8::math::light* lights, const v8_size_t num_lights)
        : ActiveLights(lights, num_lights)
    {}

    v8::rendering::renderer*                        Renderer;
    v8::math::matrix_4X4F                           ProjectionViewXForm;
    v8::base::array_proxy<const v8::math::light>    ActiveLights;
    v8::math::vector3F                              EyePosition;
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


class F4Phantom : public IGeometryShape {
/// \name Init
/// @{

public :

    F4Phantom();

    ~F4Phantom();

    v8_bool_t Initialize(const InitContext* init_context);

private :

    /// \brief Resets aricraft parameters to their default values.
    void SetDefaultAicraftDefaultParameters();

/// @}


/// \name Updating, events and drawing.
/// @{

public :

    void Update(const float delta);

    void Draw(const DrawingContext* draw_context);

    void InputEvent(const v8::input_event& in_evt);

/// @}


/// \name Movement & rotation
/// @{

private :

    inline void MoveX(const float modifier);

    inline void MoveY(const float modifier);

    inline void MoveZ(const float modifier);

    /// \brief Rotates the aircraft around the Z axis.
    inline void Roll(const float modifier);

    /// \brief Rotates the aircraft around the X axis.
    inline void Pitch(const float modifier);

    /// \brief Rotates the aircraft around the Y axis.
    inline void Yaw(const float modifier);

    inline void UpdateRotationAngle(const float amount, float* angle);

/// @}


private :
    v8_bool_t is_valid() const {
        return is_valid_;
    }

private :

    ///
    /// \brief Parameters that control the flight characteristics of an aicraft.
    struct F4PhantomFlightCharacteristics_t {
        F4PhantomFlightCharacteristics_t() {
            SetDefaultValues();
        }

        float   roll_speed;
        float   yaw_speed;
        float   pitch_speed;

        void SetDefaultValues() {
            roll_speed = 0.003f;
            yaw_speed = 0.0015f;
            pitch_speed = 0.0045f;
        }
    };

private :
    v8::rendering::index_buffer         indexbuffer_;
    v8::rendering::vertex_buffer        vertexbuffer_;
    v8::rendering::vertex_shader        vertexshader_;
    v8::rendering::fragment_shader      fragshader_;
    v8::math::vector3F                  position_;
    v8_bool_t                           keystates_[v8::input::Key_Sym_t::Last];
    v8_bool_t                           is_valid_;
    ///< Z axis rotation (radians)
    float                               roll_angle_;
    ///< X axis rotation (radians)
    float                               pitch_angle_;
    ///< Y axis rotation (radians)
    float                               yaw_angle_;
    ///< Flight parameters.
    F4PhantomFlightCharacteristics_t    flight_characteristics_;

private :
    NO_CC_ASSIGN(F4Phantom);
};

F4Phantom::F4Phantom() 
    : is_valid_(false), position_(v8::math::vector3F::zero) {}

F4Phantom::~F4Phantom() {
}

v8_bool_t F4Phantom::Initialize(const InitContext* init_context) {
    if (is_valid()) {
        return true;
    }

    memset(keystates_, 0, sizeof(keystates_));
    SetDefaultAicraftDefaultParameters();

    using namespace v8::base;
    using namespace v8::rendering;

    scoped_ptr<vertex_pn, default_array_storage>    vertices;
    scoped_ptr<v8_uint32_t, default_array_storage>  indices;
    v8_uint32_t num_vertices;
    v8_uint32_t num_indices;
    const std::string model_path = init_context->FileSystem->make_model_path(
        "f4_phantom.obj");

    const v8_bool_t load_succeeded = v8::utility::import_geometry(
        model_path.c_str(), scoped_pointer_get_impl(vertices),
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
    shader_info.compile_macros        = "[__VERTEX_PN__]";

    if (!fragshader_.initialize(shader_info, init_context->Renderer)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shader_info.name_or_source.c_str());
        return false;
    }

    shader_info.name_or_source =
        init_context->FileSystem->make_shader_path("basic_transform");
    shader_info.entrypoint = "xform_vertex_pn";
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

void F4Phantom::SetDefaultAicraftDefaultParameters() {
    yaw_angle_ = 0.0f;
    pitch_angle_ = 0.0f;
    roll_angle_ = 0.0f;
    position_ = v8::math::vector3F::zero;
    flight_characteristics_.SetDefaultValues();
}

void F4Phantom::Update(const float /*delta*/) {
    using v8::input::Key_Sym_t;

    v8::math::vector3F translate(v8::math::vector3F::zero);

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
        Roll(1.0f);
    }

    if (keystates_[Key_Sym_t::Key_E]) {
        Roll(-1.0f);
    }

    if (keystates_[Key_Sym_t::Key_U]) {
        Pitch(+1.0f);
    }

    if (keystates_[Key_Sym_t::Key_J]) {
        Pitch(-1.0f);
    }

    if (keystates_[Key_Sym_t::Key_N]) {
        Yaw(+1.0f);
    }

    if (keystates_[Key_Sym_t::Key_M]) {
        Yaw(-1.0f);
    }
}

struct Material {
    v8::math::color_rgb emissive;
    v8::math::color_rgb specular;
    v8::math::color_rgb ambient;
    v8::math::color_rgb diffuse;
};

void F4Phantom::Draw(const DrawingContext* draw_ctx) {
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

    quaternionF quat_roll(roll_angle_, vector3F::unit_z);
    quaternionF quat_pitch(pitch_angle_, vector3F::unit_x);
    quaternionF quat_yaw(yaw_angle_, vector3F::unit_y);

    quaternionF quat_rotation(quat_roll * quat_pitch * quat_yaw);

    matrix_4X4F rotation_xform;
    quat_rotation.extract_rotation_matrix(&rotation_xform);

    matrix_4X4F translation_xform;
    translation_xform.make_identity();
    translation_xform.set_column(4, position_.x_, position_.y_, position_.z_, 1.0f);

    matrix_4X4F model_to_world = translation_xform * rotation_xform;
    
    vertexshader_.set_uniform_by_name(
        "world_view_projection", draw_ctx->ProjectionViewXForm * model_to_world
        );

    vertexshader_.set_uniform_by_name(
        "normal_transform", model_to_world.transpose()
        );

    fragshader_.set_uniform_by_name("k_lighting_model", 0);

    fragshader_.set_uniform_by_name(
        "k_scene_lights", draw_ctx->ActiveLights.base(),
        8 * sizeof(draw_ctx->ActiveLights[0])
        );

    fragshader_.set_uniform_by_name("k_eye_pos", draw_ctx->EyePosition);
    fragshader_.set_uniform_by_name("k_light_count", 1);

    Material mtl;
    mtl.diffuse = color_rgb::C_DarkBlue;
    mtl.specular = color_rgb::C_White;
    mtl.ambient = color_rgb::C_LightBlue;

    //fragshader_.set_uniform_block_by_name("material_globals", mtl);
    fragshader_.set_uniform_by_name("mat_diffuse", mtl.diffuse);
    fragshader_.set_uniform_by_name("mat_specular", mtl.specular);
    fragshader_.set_uniform_by_name("mat_ambient", mtl.ambient);

    vertexshader_.bind_to_pipeline(draw_ctx->Renderer);
    fragshader_.bind_to_pipeline(draw_ctx->Renderer);

    draw_ctx->Renderer->draw_indexed(indexbuffer_.get_element_count());
}

void F4Phantom::InputEvent(const v8::input_event& in_evt) {
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
    position_.x_ += modifier * 0.025f;
}

inline void F4Phantom::MoveY(const float modifier) {
    position_.y_ += modifier * 0.025f;
}

inline void F4Phantom::MoveZ(const float modifier) {
    position_.z_ += modifier * 0.025f;
}

inline void F4Phantom::Roll(const float modifier) {
    UpdateRotationAngle(modifier * flight_characteristics_.roll_speed,
                        &roll_angle_);
}

inline void F4Phantom::Yaw(const float modifier) {
    UpdateRotationAngle(modifier * flight_characteristics_.yaw_speed,
                        &yaw_angle_);
}

inline void F4Phantom::Pitch(const float modifier) {
    UpdateRotationAngle(modifier * flight_characteristics_.pitch_speed,
                        &pitch_angle_);
}

inline void F4Phantom::UpdateRotationAngle(const float amount, float* angle) {
    *angle += amount;
    if (*angle >= v8::math::numericsF::two_pi()) {
        *angle -= v8::math::numericsF::two_pi();
        return;
    }

    if (*angle <= -v8::math::numericsF::two_pi()) {
        *angle += v8::math::numericsF::two_pi();
    }
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

    void setup_camera() {
        assert(is_valid());
        camera_->set_projection_matrix(v8::math::projectionF::perspective(
            window_->get_aspect_ratio(), 45.0f, 0.1f, 1000.0f, 
            v8::math::z_ndc_D3DF::min(), v8::math::z_ndc_D3DF::max()
            ));
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
    v8::math::light                                     lights_[8];
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
    filesys_->initialize("E:\\games\\basic_drawing");

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
    setup_camera();
    cam_controller_ = new v8::scene::camera_controller_spherical_coords(
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
    window_->Delegates_UpdateEvent += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(cam_controller_),
        &v8::scene::camera_controller::update
        );

    //
    // Create objects.
    initialize_objects();

    return true;
}

void basic_drawing_app::update(const float delta_tm) {
    using namespace std;

    setup_camera();
    cam_controller_->update(delta_tm);

    for_each(begin(geometric_objects_), end(geometric_objects_),
                   [delta_tm](IGeometryShape* geometry_shape) {
        geometry_shape->Update(delta_tm);
    });
}

void basic_drawing_app::draw() {
    assert(is_valid());
    
    //rendersys_->reset_blending_state();
    //rendersys_->reset_depth_stencil_state();
    rendersys_->clear_depth_stencil();
    rendersys_->clear_backbuffer();

    DrawingContext draw_context(lights_, dimension_of(lights_));
    draw_context.Renderer = v8::base::scoped_pointer_get(rendersys_);
    draw_context.ProjectionViewXForm = camera_->get_projection_wiew_transform();
    draw_context.EyePosition = camera_->get_origin();

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

    v8::base::scoped_ptr<F4Phantom> f4(new F4Phantom());
    if (!f4->Initialize(&obj_init_context)) {
        return;
    }

    window_->Subscribers_InputEvents += fastdelegate::MakeDelegate(
        v8::base::scoped_pointer_get(f4), &F4Phantom::InputEvent
        );
    geometric_objects_.push_back(v8::base::scoped_pointer_release(f4));

    //
    // add some lights
    using namespace v8::math;
    lights_[0] = light::make_directional_light(
        color_rgb(0.1f, 0.1f, 0.1f, 1.0f),
        color_rgb(1.0f, 1.0f, 1.0f, 1.0f), 
        color_rgb(1.0f, 1.0f, 1.0f, 1.0f),
        normal_of(vector3F(1.0f, -1.0f, 1.0f))
        );
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
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