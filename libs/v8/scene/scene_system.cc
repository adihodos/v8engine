#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/rangelib/sequence_range.hpp>

#include "v8/base/debug_helpers.hpp"
#include "v8/global_state.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/io/config_file_reader.hpp"
#include "v8/rendering/material.hpp"
#include "v8/rendering/render_assets_cache.hpp"
#include "v8/scene/camera_controller.hpp"
#include "v8/scene/scene_entity.hpp"
#include "v8/scene/scene_loading_info.hpp"

#include "v8/scene/scene_system.hpp"

v8::scene::scene_system::scene_system()
    :       m_light_count(0)
        ,   m_active_light_count(0)
        ,   m_active_list(0)
{}

v8::scene::scene_system::~scene_system() {}

v8_bool_t v8::scene::scene_system::initialize() {
    platformstl::path scene_cfg_file_path(state->file_sys()->get_dir_path(
        filesys::Dir::Config
        ));
    const char* const k_conf_file_name = "scene_conf.json";
    scene_cfg_file_path.push(k_conf_file_name);

    io::V8ConfigFile config_file(scene_cfg_file_path.c_str());
    if (!config_file.CheckIfValid()) {
        return false;
    }

    SectionHandlersTable_t sec_handlers_tbl;
    fill_section_handlers_table(&sec_handlers_tbl);
    rangelib::sequence_range<const SectionHandlersTable_t> handler_range(sec_handlers_tbl);

    for (; handler_range; ++handler_range) {
        const SectionHandlerTableEntry_t& k_sec_handler = *handler_range;
        const char* k_section_hier[] = { k_sec_handler.section_name };

        const io::V8ConfigFile_ReadOnlySection k_section(
            config_file.GetSection(k_section_hier, dimension_of(k_section_hier)));

        if (!k_section.CheckIfValid()) {
            OUTPUT_DBG_MSGA("Warning : section %s does not exists", k_section_hier[0]);
            continue;
        }

        //
        // Read an process section's elements.
        for (v8_size_t elem_idx = 0;; ++elem_idx) {
            const io::V8ConfigFile_ReadOnlySection k_section_elem(
                k_section.GetSectionAt(elem_idx));
            if (!k_section_elem.CheckIfValid()) {
                break;
            }

            const v8_bool_t k_ret_code = k_sec_handler.read_element_fn(
                this, k_section_elem, k_sec_handler.element_data);
                    
            if (k_ret_code) {
                k_sec_handler.process_element_fn(
                    this, k_sec_handler.element_data);
            }
        }
    }
    return true;
}

v8_bool_t v8::scene::scene_system::read_light(
    const io::V8ConfigFile_ReadOnlySection& lsection, 
    void* light_data
    ) {
    assert(light_data);
    assert(lsection.CheckIfValid());

    light_loading_info_t* llinfo = static_cast<light_loading_info_t*>(light_data);

    bool is_enabled;
    lsection.GetValue("enabled", is_enabled);
    llinfo->state = is_enabled;
    v8_int_t light_type;
    lsection.GetValue("type", light_type);

    if (light_type == v8::math::light::light_type_directional) {
        llinfo->light.set_type(v8::math::light::light_type_directional);
        v8::math::vector3F light_dirvec;
        lsection.GetValue("direction", light_dirvec.elements_);
        llinfo->light.set_direction(light_dirvec);
    } else if (light_type == v8::math::light::light_type_point) {
        llinfo->light.set_type(v8::math::light::light_type_point);
    } else if (light_type == v8::math::light::light_type_spot) {
        llinfo->light.set_type(v8::math::light::light_type_spot);
    } else {
        OUTPUT_DBG_MSGA("Unknown light type (%d)", light_type);
        return false;
    }

    if (light_type != v8::math::light::light_type_directional) {
        v8::math::vector3F vec_data;
        lsection.GetValue("position", vec_data.elements_);
        llinfo->light.set_position(vec_data);

        lsection.GetValue("attenuation", vec_data.elements_);
        llinfo->light.set_attenuation_factors(vec_data);

        float max_range;
        lsection.GetValue("max_range", max_range);
        llinfo->light.set_max_range(max_range);
    }

    if (light_type == v8::math::light::light_type_spot) {
        float flt_value;
        lsection.GetValue("cone_phi", flt_value);
        llinfo->light.set_spot_cone_angle_phi(flt_value);

        lsection.GetValue("cone_theta", flt_value);
        llinfo->light.set_spot_cone_angle_theta(flt_value);

        lsection.GetValue("spot_power", flt_value);
        llinfo->light.set_spot_power(flt_value);
    }

    v8::math::color_rgb color_vec;
    lsection.GetValue("ambient", color_vec.components_);
    llinfo->light.set_ambient_color(color_vec);

    lsection.GetValue("specular", color_vec.components_);
    llinfo->light.set_specular_color(color_vec);

    lsection.GetValue("diffuse", color_vec.components_);
    llinfo->light.set_diffuse_color(color_vec);
    return true;
}

void v8::scene::scene_system::process_light(
    const void* light_data
    ) {
    assert(light_data);
    const light_loading_info_t* llinfo = 
        static_cast<const light_loading_info_t*>(light_data);
    if (get_lights_count() < scene_system::k_max_lights_count) {
        push_light(llinfo->light, llinfo->state);
    }
}

v8_bool_t v8::scene::scene_system::read_material( 
    const io::V8ConfigFile_ReadOnlySection& material_entry, 
    void* material_data
    ) {
    assert(material_data);

    material_loading_info_t* mtinfo = 
        static_cast<material_loading_info_t*>(material_data);

    const char* k_mat_name;
    material_entry.GetValue("name", k_mat_name);
    mtinfo->mat_name = k_mat_name;
    
    return true;
}

void v8::scene::scene_system::process_material( 
    const void* material_data
    ) {
    assert(material_data);
    const material_loading_info_t* mat_load_info = 
        static_cast<const material_loading_info_t*>(material_data);

    //global::state->AssetCache->get_material(mat_load_info->mat_name.c_str());
}

v8_bool_t v8::scene::scene_system::read_standard_object(
    const io::V8ConfigFile_ReadOnlySection& object_entry, 
    void* object_data
    ) {
    assert(object_data);

    standard_object_loading_info_t* std_obj_info = 
        static_cast<standard_object_loading_info_t*>(object_data);

    bool is_enabled;
    object_entry.GetValue("enabled", is_enabled);
    if (!is_enabled) {
        return false;
    }

    const char* k_string_val = nullptr;
    object_entry.GetValue("mesh", k_string_val);
    std_obj_info->mesh = k_string_val;

    object_entry.GetValue("material", k_string_val);
    std_obj_info->material = k_string_val;

    object_entry.GetValue("scale", std_obj_info->scale);
    object_entry.GetValue("translation", std_obj_info->translate);
    object_entry.GetValue("rotate", std_obj_info->rotate);
    return true;
}

v8_bool_t v8::scene::scene_system::read_userdefined_object(
    const io::V8ConfigFile_ReadOnlySection& object_entry, 
    void* object_data
    ) {
    assert(object_data);

    userdefined_object_loading_info_t* userobject_info = 
        static_cast<userdefined_object_loading_info_t*>(object_data);

    bool is_enabled = false;
    object_entry.GetValue("enabled", is_enabled);
    if (!is_enabled) {
        return false;
    }

    const char* k_string_val = nullptr;
    object_entry.GetValue("name", k_string_val);
    userobject_info->name = k_string_val;

    object_entry.GetValue("config_file", k_string_val);
    userobject_info->config_file = k_string_val;

    object_entry.GetValue("type", userobject_info->type);
    return true;
}

void v8::scene::scene_system::update(
    float delta_ms
    ) {
    assert(check_valid());

    m_cam_controller->update(delta_ms);

    using namespace std;
    for_each(begin(m_entity_list), end(m_entity_list), [delta_ms](scene_entity* s_ent) {
        s_ent->update(delta_ms);
    });
}

void v8::scene::scene_system::pre_draw(
    v8::rendering::renderer*
    ) {
    m_active_light_count = 0;
    for (v8_int_t light_idx = 0; light_idx < m_light_count; ++light_idx) {
        if (is_light_on(light_idx)) {
            m_active_lights[m_active_light_count++] = m_scene_ligths[light_idx];
        }
    }
}

void v8::scene::scene_system::post_draw(
    v8::rendering::renderer*
    ) {
}

void v8::scene::scene_system::draw(
    v8::rendering::renderer* p_render
    ) {
    assert(check_valid());

    pre_draw(p_render);

    using namespace std;

    scene_system* smgr = this;
    for_each(
        begin(m_entity_list),
        end(m_entity_list),
        [smgr, p_render](scene_entity* s_ent) {
            s_ent->pre_draw(smgr, p_render);
            s_ent->draw(smgr, p_render);
            s_ent->post_draw(smgr, p_render);
    });

    post_draw(p_render);
}

void v8::scene::scene_system::depth_sort_all() {
    assert(check_valid());
}
