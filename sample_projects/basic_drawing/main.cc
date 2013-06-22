#include <algorithm>
#include <v8/v8.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/count_of.hpp>
#include <v8/base/string_util.hpp>
#include <v8/event/window_event.hpp>
#include <v8/event/input_event.hpp>
#include <v8/input/key_syms.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/graphis_api_traits.hpp>
#include <v8/math/light.hpp>
#include <v8/math/math_constants.hpp>
#include <v8/math/projection.hpp>
#include <v8/scene/null_camera_controller.hpp>
#include <v8/scene/cam_controller_spherical_coordinates.hpp>
#include <third_party/fast_delegate/fast_delegate.hpp>
#include <third_party/stlsoft/stlsoft/string/static_string.hpp>

#include <v8/rendering/vertex_p.hpp>

#include "app_context.hpp"
#include "floor_plane.hpp"
#include "igeometry_shape.hpp"
#include "aircraft_f4.hpp"

struct cam_init_data {
    v8::gui::basic_window* window;
};

template<typename T>
struct DefaultDeleter {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

class sph_cam : public v8::scene::camera_controller_spherical_coords {
public :
    typedef v8::scene::camera_controller_spherical_coords   base_class;

    sph_cam(v8::math::camera* cam_ptr = nullptr);
public :
    v8_bool_t initialize(const void* init_params);

    void on_input_event(const v8::input_event& in_evt);

    void on_viewport_resized(const v8::resize_event& resize_evt);

    std::string cam_data_as_string() const;

private :
    void setup_camera();

private :
    v8::gui::basic_window*      app_window_;
    v8_uint32_t                 proj_type_;
    float                       fov_;
};

sph_cam::sph_cam(v8::math::camera* cam_ptr /*= nullptr*/)
    :       base_class(cam_ptr)
        ,   app_window_(nullptr)
        ,   proj_type_(v8::math::projection::type::perspective)
        ,   fov_(45.0f)
{}

v8_bool_t sph_cam::initialize(const void* init_params) {
    assert(init_params);

    const cam_init_data* init_ctx = static_cast<const cam_init_data*>(init_params);
    app_window_ = init_ctx->window;
    
    setup_camera();
    return true;
}

void sph_cam::setup_camera() {
    assert(app_window_);
    assert(cam_ptr_);

    using namespace v8::math;
    matrix_4X4F proj_xform;

    const float aspect_ratio = app_window_->get_aspect_ratio();

    if (proj_type_ == v8::math::projection::type::perspective) {
        proj_xform = projection::perspective(
            aspect_ratio, fov_, 1.0f, 1000.0f, 
            v8::graphics_traitsF::zmin(), v8::graphics_traitsF::zmax()
            );
    } else if (proj_type_ == v8::math::projection::type::perspective_oblique) {
        proj_xform = projection::perspective_oblique(
            -1.0f * aspect_ratio, 1.0f * aspect_ratio,
            -1.0f, 1.0f, 3.0f, 1000.0f,
            v8::graphics_traitsF::zmin(), v8::graphics_traitsF::zmax()
            );
    } else if (proj_type_ == v8::math::projection::type::perspective_infinity) {
        proj_xform = projection::perspective_infinity(
            aspect_ratio, 45.0f, 0.1f, 
            v8::graphics_traitsF::zmin(), v8::graphics_traitsF::zmax()
            );
    } else if (proj_type_ == v8::math::projection::type::parallel_orthographic) {
        proj_xform = projection::parallel_orthographic(
            -4.0f * aspect_ratio, 4.0f * aspect_ratio,
            -4.0f, 4.0f, 0.1f, 1000.0f, 
            v8::graphics_traitsF::zmin(), v8::graphics_traitsF::zmax()
            );
    }

    cam_ptr_->set_projection_matrix(proj_xform);
    cam_ptr_->set_projection_type(proj_type_);
}

void sph_cam::on_input_event(const v8::input_event& in_evt) {
    if (in_evt.type == v8::InputEventType::Key) {
        if (in_evt.key_ev.down 
            && in_evt.key_ev.key == v8::input::Key_Sym_t::PageDown) {
            proj_type_++;
            proj_type_ %= 4;
            setup_camera();
            return;
        }

        if (in_evt.key_ev.key == v8::input::Key_Sym_t::Insert) {
            fov_ += 1.0f;
            setup_camera();
            return;
        }

        if (in_evt.key_ev.key == v8::input::Key_Sym_t::Delete) {
            fov_ -= 1.0f;
            setup_camera();
            return;
        }

        if (in_evt.key_ev.key == v8::input::Key_Sym_t::Space) {
            fov_ = 1.0f;
            setup_camera();
            proj_type_ = v8::math::projection::type::perspective;
            return;
        }
    }

    base_class::on_input_event(in_evt);
}

void sph_cam::on_viewport_resized(const v8::resize_event&) {
    setup_camera();
}

std::string sph_cam::cam_data_as_string() const {
    const char* const kPerspTypeStrings[] = {
        "Perspective",
        "Perspective oblique",
        "Perspective infinite",
        "Parallel orthographic",
        "Parallel oblique"
    };

    v8::base::auto_buffer<char, 512> cam_stats;
    const v8::math::vector3F& cam_origin = cam_ptr_->get_origin();
    const v8::math::vector3F& x_axis = cam_ptr_->get_right_vector();
    const v8::math::vector3F& y_axis = cam_ptr_->get_up_vector();
    const v8::math::vector3F& z_axis = cam_ptr_->get_direction_vector();

    v8::base::snprintf(
        &cam_stats[0], cam_stats.size(), 
        "Projection type [%s]\n"
        "Camera origin  [%3.3f, %3.3f, %3.3f]\n"
        "Camera X axis  [%3.3f, %3.3f, %3.3f]\n"
        "Camera Y axis  [%3.3f, %3.3f, %3.3f]\n"
        "Camera Z axis  [%3.3f, %3.3f, %3.3f]\n"
        "Field of view [%3.3f]",
        kPerspTypeStrings[proj_type_],
        cam_origin.x_, cam_origin.y_, cam_origin.z_,
        x_axis.x_, x_axis.y_, x_axis.z_,
        y_axis.x_, y_axis.y_, y_axis.z_,
        z_axis.x_, z_axis.y_, z_axis.z_,
        fov_
        );

    return cam_stats.get_buffer();
};

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
    cam_controller_ = new sph_cam(v8::base::scoped_pointer_get(camera_));

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

    cam_controller_->update(delta_tm);

    for_each(begin(geometric_objects_), end(geometric_objects_),
                   [delta_tm](IGeometryShape* geometry_shape) {
        geometry_shape->Update(delta_tm);
    });
}

void basic_drawing_app::draw() {
    assert(is_valid());
    
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

    const std::string cam_stats = cam_controller_->cam_data_as_string();

    rendersys_->draw_string(cam_stats.c_str(), 14.0f, 5.0f, 5.0f, 
                            v8::math::color_rgb::C_Black);
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

    v8::base::scoped_ptr<FloorPlane> floor_plane(new FloorPlane());
    if (floor_plane->Initialize(&obj_init_context)) {
        geometric_objects_.push_back(v8::base::scoped_pointer_release(
            floor_plane));
    }

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
