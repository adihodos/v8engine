#define _CRTDBG_MAP_ALLOC

#include <cstdlib>
#include <crtdbg.h>
#include <ctime>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <v8/v8.hpp>

#include <v8/base/auto_buffer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/scoped_resource.hpp>

#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/math/light.hpp>
#include <v8/math/vector3.hpp>
#include <v8/math/matrix4X4.hpp>

#include <v8/global_state.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/effect.hpp>
#include <v8/rendering/effect_info.hpp>
#include <v8/rendering/effect_pass.hpp>
#include <v8/rendering/effect_technique.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/render_assets_cache.hpp>
#include <v8/scene/cam_controller_spherical_coordinates.hpp>
#include <v8/scene/standard_entity.hpp>
#include <v8/scene/custom_entity.hpp>
#include <v8/scene/scene_config_reader.hpp>
#include <v8/scene/scene_system.hpp>
#include <v8/scene/scene_loading_info.hpp>
#include <v8/scene/cam_controller_spherical_coordinates.hpp>
#include <v8/utility/win_util.hpp>

#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/rangelib/rangelib.hpp>
#include <third_party/stlsoft/rangelib/sequence_range.hpp>

#include "grid.hpp"

namespace {

class object_factory {
public :
    struct ObjectTypes {
        enum {
            Grid = 0
        };
    };

public :
    object_factory(
        v8::scene::scene_system* ssys, 
        v8::rendering::effect* eff,
        const char* eff_technique
        )
        :       ssys_(ssys)
            ,   effect_(eff)
            ,   eff_tech_(eff_technique)
    {}

    void process_loaded_entity_info(
        const v8::scene::standard_entity_load_info_t& ent_data
        );

private :
    v8::scene::scene_system*    ssys_;
    v8::rendering::effect*      effect_;
    const char*                 eff_tech_;
};

void object_factory::process_loaded_entity_info(
    const v8::scene::standard_entity_load_info_t& ent_data
    ) {
    if (ent_data.ent_type == ObjectTypes::Grid) {
        v8::base::scoped_ptr<simple_grid>   grid_obj;
        if (!grid_obj->initialize(ent_data.ent_conf_file.c_str())) {
            return;
        }
        grid_obj->attach_effect(effect_, eff_tech_);
        v8::math::transformF ent_xform;
        ent_xform.set_scale_component(ent_data.ent_scale);
        ent_xform.set_translation_component(v8::math::vector3F(ent_data.ent_translate));
        //
        // todo : add rotation support.
        ssys_->add_entity(v8::base::scoped_pointer_release(grid_obj));
        return;
    }

    OUTPUT_DBG_MSGA("Unknown enitity type %d", ent_data.ent_type);
}

class camera_node : public v8::scene::scene_entity {
public :
    camera_node(v8::scene::camera_controller* controller = nullptr)
        :       cam_controller_(controller)
    {
        if (cam_controller_) {
            cam_controller_->set_camera(&cam_);
        }
    }

    void on_input_event(const v8::input_event& in_evt);

    void on_viewport_resized(const v8::resize_event& resize_request);

    void set_camera_controller(v8::scene::camera_controller* controller) {
        cam_controller_ = controller;
        if (cam_controller_) {
            cam_controller_->set_camera(&cam_);
        }
    }

public :

    void pre_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void post_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void update(float delta);    

private :
    v8::math::camera                                        cam_;
    v8::base::scoped_ptr<v8::scene::camera_controller>      cam_controller_;
};

void camera_node::on_input_event(const v8::input_event& in_evt) {
    if (cam_controller_) {
        cam_controller_->on_input_event(in_evt);
    }
}

void camera_node::on_viewport_resized(const v8::resize_event& resize_request) {
    if (cam_controller_) {
        cam_controller_->on_viewport_resized(resize_request);
    } else {
        const float aspect_ratio = static_cast<float>(resize_request.width) 
                                    / static_cast<float>(resize_request.height);
        cam_.set_symmetric_frustrum_perspective(60.0f, aspect_ratio, 1.0f, 1000.0f);
    }
}

void camera_node::pre_draw(v8::scene::scene_system*, v8::rendering::renderer*) {
    if (attached_effect_) {
        attached_effect_->set_variable_by_semantic(
            "EyePosition", cam_.get_origin()
            );
    }
}

void camera_node::draw(v8::scene::scene_system*, v8::rendering::renderer*) {}

void camera_node::post_draw(v8::scene::scene_system*, v8::rendering::renderer*) {}

void camera_node::update(float delta) {
    if (cam_controller_) {
        cam_controller_->update(delta);
    }
}

class light_node : public v8::scene::scene_entity {
public :
    ///< Maximum number of lights that we support.
    static const v8_int32_t k_max_lights_count = 8U;    

/// \name Constructors
/// @{

public :
    
    light_node();    

/// @}    

/// \name Operations
/// @{

public :    
    void pre_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void post_draw(v8::scene::scene_system*, v8::rendering::renderer*);

    void update(float delta);

/// @}    

/// \name Lights management.
/// @{

public :

    /// \brief adds another light to the list of lights.
    void push_light(const v8::math::light& light_source, const v8_bool_t on) {
        assert(m_light_count < k_max_lights_count);
        m_scene_ligths[m_light_count++] = light_source;
        if (on) {
            set_light_on(m_light_count - 1);
        } else {
            set_light_off(m_light_count - 1);
        }
        m_list_needs_update = true;
    }

    /// \brief returns a reference to the light with the specified index.
    v8::math::light& get_light(const v8_int32_t light_index) {
        assert(light_index < m_light_count);
        return m_scene_ligths[m_light_count];
    }

    const v8::math::light& get_light(const v8_int32_t light_index) const {
        assert(light_index < m_light_count);
        return m_scene_ligths[m_light_count];
    }

    /// \brief removes the last light.
    void pop_light() {
        assert(m_light_count > 0);
        --m_light_count;
        m_list_needs_update = true;
    }

    v8_int32_t get_lights_count() const {
        return m_light_count;
    }

    v8_int32_t get_active_ligths_count() const {
        return m_active_light_count;
    }

    void toggle_light(const v8_int_t k_index) {
        m_active_list = !(m_active_list & (1 << k_index)) 
                        | (m_active_list & ~(1 << k_index));
        m_list_needs_update = true;
    }

    void set_light_on(const v8_int_t index) {
        m_active_list = (m_active_list & ~(1 << index)) | (1 << index);
        m_list_needs_update = true;
    }

    void set_light_off(const v8_int_t index) {
        m_active_list = (m_active_list & ~(1 << index));
        m_list_needs_update = true;
    }

    v8_bool_t is_light_on(const v8_int_t index) const {
        return m_active_list & (1 << index);
    }    

/// @}

private :
    ///< Stores light information.
    v8::math::light                 m_scene_ligths[k_max_lights_count];
    v8::math::light                 m_active_lights[k_max_lights_count];

    ///< Numbers of lights in the scene.
    v8_int32_t                      m_light_count;
    ///< Number of active lights.
    v8_int32_t                      m_active_light_count;
    ///< Byte array that identifies active lights.
    v8_byte_t                       m_active_list;
    ///< True if the list of active ligths needs to be rebuild.
    v8_bool_t                       m_list_needs_update;    
};

light_node::light_node()
    :       m_light_count(0)
        ,   m_active_light_count(0)
        ,   m_active_list(0)
        ,   m_list_needs_update(true)
{}        

void light_node::pre_draw(v8::scene::scene_system*, v8::rendering::renderer*) {
    if (!attached_effect_) {
        return;
    }
    attached_effect_->set_variable_by_semantic(
        "LightsCount", get_active_ligths_count()
        );
    attached_effect_->set_variable_by_semantic(
        "SceneLights", m_active_lights
        );
}

void light_node::draw(v8::scene::scene_system*, v8::rendering::renderer*) {}

void light_node::post_draw(v8::scene::scene_system*, v8::rendering::renderer*) {}

void light_node::update(float /* delta */) {
    if (m_list_needs_update) {
        m_active_light_count = 0;
        for (v8_int_t light_idx = 0; light_idx < m_light_count; ++light_idx) {
            if (is_light_on(light_idx)) {
                m_active_lights[m_active_light_count++] = m_scene_ligths[light_idx];
            }
        }
        m_list_needs_update = false;
    }
}

class app_state : public v8::application_state {
public :
    typedef v8::application_state                           base_class;

public :
    v8_bool_t initialize();

private :

    void load_std_object(const v8::scene::standard_object_loading_info_t& obj_info) {
        
    }
};

v8_bool_t app_state::initialize() {
    base_class::initialize();

    window_                                = new v8::gui::basic_window();
    const v8_uint32_t default_window_style = WS_OVERLAPPEDWINDOW;
    const v8_int32_t win_width             = 1024;
    const v8_int32_t win_height            = 1024;

    if (!window_->initialize(default_window_style, "BasicWindowClass@@__", 
                             "V8 Lighting demo", win_width, win_height)) {
        return false;
    }

    render_sys_ = new v8::rendering::renderer();
    if (!render_sys_->initialize(window_->get_handle(), 
                                 static_cast<float>(win_width), 
                                 static_cast<float>(win_height), 
                                 v8::rendering::ElementType::Unorm8,
                                 4, 
                                 24, // depth bits
                                 8 // stencil bits
                                 )) {
        return false;
    }

    window_->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        render_sys(), &v8::rendering::renderer::on_viewport_resized
        );

    asset_cache_ = new v8::rendering::render_assets_cache(render_sys());

    file_sys_ = new v8::filesys();
    file_sys_->initialize("D:\\games\\lighting_demo");

    scene_sys_ = new v8::scene::scene_system();

    //
    //  The following order must be preserved for entities that are added to
    //  a scene :
    //  1.  the light node
    //  2.  the camera node
    //  3.  anything else.

    //scene_sys_->add_entity(v8::base::scoped_pointer_release(l_node));

    //
    // Add camera + controller
    v8::base::scoped_ptr<camera_node> cam_node(new camera_node());
    v8::base::scoped_ptr<
        v8::scene::camera_controller_spherical_coords
    >                                                   cam_controller(
        new v8::scene::camera_controller_spherical_coords()
    );

    cam_controller->initialize();
    cam_node->set_camera_controller(
        v8::base::scoped_pointer_release(cam_controller)
        );
    scene_sys_->add_entity(v8::base::scoped_pointer_release(cam_node));

    //
    // Add lights
    v8::base::scoped_ptr<light_node> l_node(new light_node());
    v8::scene::section_reader_lights lights_reader(fastdelegate::MakeDelegate(
       v8::base::scoped_pointer_get(l_node), &light_node::push_light
       ));

    using v8::rendering::Compile_Options;
    const v8_uint32_t k_compile_options = Compile_Options::Generate_Debug_Info 
        | Compile_Options::Warnings_Are_Errors
        | Compile_Options::Skip_Optimization
        | Compile_Options::Optimization_L0;

    const v8::rendering::effect_info_t eff_info(
        file_sys_->make_effect_path("lighting"),
        "[__VERTEX_PN__]",
        k_compile_options,
        v8::rendering::FX_Model::FX_5_0
        );

    //
    // Add ents.
    object_factory obj_factory(scene(), )
    v8::scene::seaction_reader_standard_entity ent_reader;

    v8::scene::scene_config_reader config_reader;
    config_reader.add_section_reader("lights", &lights_reader);
    config_reader.add_section_reader("objects", &ent_reader);

}

}

int WINAPI WinMain(
    HINSTANCE inst,
    HINSTANCE,
    LPSTR,
    int
    ) {
    /*
    std::srand(static_cast<unsigned int>(time(nullptr)));
    {
        win32::scoped_mem_leak_checker mem_checker;

        global::application_state app_state;

        global::state->Window = new main_window();
        if (!global::state->Window->initialize(
            WS_OVERLAPPEDWINDOW, "DirectXWindowClass__@@", "Simple DX app", 
            1024, 1024)) {
            OUTPUT_DBG_MSGA("Failed to initialize window!");
            return -1;
        }

        global::state->Renderer = new v8_framework::render_engine::renderer();
        v8_bool_t ret_code = global::state->Renderer->initialize(
            global::state->Window->get_handle(), global::state->Window->get_width(),
            global::state->Window->get_height(), v8_framework::ElementType::Unorm8, 
            4, 24, 8);

        if (!ret_code) {
            OUTPUT_DBG_MSGA("Failed to initialize renderer!");
            return -1;
        }

        global::state->Window->Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
            v8::base::scoped_pointer_get(global::state->Renderer),
            &v8_framework::render_engine::renderer::on_viewport_resized);

        global::state->Renderer->set_clear_color(v8::math::color_rgb::C_Black);

        global::state->AssetCache = new v8_framework::render_engine::resource_manager(
            v8::base::scoped_pointer_get(global::state->Renderer));

        using namespace v8_framework;

        global::state->Filesys = new v8_framework::app_dirs();
        global::state->Filesys->initialize("D:\\games\\v8_assets");

        global::state->Scene = new simple_scene();
        if (!global::state->Scene->initialize()) {
            OUTPUT_DBG_MSGA("Failed to initialize the scene!");
            return -1;
        }

        global::state->Window->message_loop();
    }
*/
    return 0;
}
