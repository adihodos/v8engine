#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/base/debug_helpers.hpp"
#include "v8/global_state.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/io/config_file_reader.hpp"
#include "v8/scene/scene_loading_info.hpp"

#include "v8/scene/scene_config_reader.hpp"

v8::scene::scene_config_reader::scene_config_reader()
{}


v8_bool_t v8::scene::scene_config_reader::read_config_file(
    const char* config_file_path
    ) {
    io::V8ConfigFile config_file(config_file_path);
    if (!config_file.CheckIfValid()) {
        return false;
    }

    io::V8ConfigFile::iterator itr_sections = config_file.begin();
    io::V8ConfigFile::iterator itr_end = config_file.end();

    while (itr_sections != itr_end) {
        auto itr_sec_reader = sec_readers_.find(itr_sections->name.GetString());

        if (itr_sec_reader != std::end(sec_readers_)) {
            //
            // process this section
            io::V8ConfigFile_ReadOnlySection section(&*itr_sections);
            section_reader* sec_reader = itr_sec_reader->second;

            for (v8_size_t elem_idx = 0; ; ++elem_idx) {
                const io::V8ConfigFile_ReadOnlySection se_entry(section.GetSectionAt(elem_idx));
                if (!se_entry.CheckIfValid()) {
                    break;
                }
                sec_reader->read_section_entry(se_entry);
            }
        }
        ++itr_sections;
    }
    return true;
}

void v8::scene::section_reader_lights::read_section_entry(
    const io::V8ConfigFile_ReadOnlySection& se_entry
    ) {
    assert(se_entry.CheckIfValid());
    light_loading_info_t llinfo;

    bool is_enabled;
    se_entry.GetValue("enabled", is_enabled);
    llinfo.state = is_enabled;
    v8_int_t light_type;
    se_entry.GetValue("type", light_type);

    if (light_type == v8::math::light::type::directional) {
        llinfo.light.set_type(v8::math::light::type::directional);
        v8::math::vector3F light_dirvec;
        se_entry.GetValue("direction", light_dirvec.elements_);
        llinfo.light.set_direction(light_dirvec);
    } else if (light_type == v8::math::light::type::point) {
        llinfo.light.set_type(v8::math::light::type::point);
    } else if (light_type == v8::math::light::type::spot) {
        llinfo.light.set_type(v8::math::light::type::spot);
    } else {
        OUTPUT_DBG_MSGA("Unknown light type (%d)", light_type);
        return;
    }

    //
    // Data common to point and spot light sources.
    if (light_type != v8::math::light::type::directional) {
        v8::math::vector3F vec_data;
        se_entry.GetValue("position", vec_data.elements_);
        llinfo.light.set_position(vec_data);

        se_entry.GetValue("attenuation", vec_data.elements_);
        llinfo.light.set_attenuation_factors(vec_data);

        float max_range;
        se_entry.GetValue("max_range", max_range);
        llinfo.light.set_max_range(max_range);
    }

    //
    // Specific data for spots.
    if (light_type == v8::math::light::type::spot) {
        float flt_value;
        se_entry.GetValue("cone_phi", flt_value);
        llinfo.light.set_spot_cone_angle_phi(flt_value);

        se_entry.GetValue("cone_theta", flt_value);
        llinfo.light.set_spot_cone_angle_theta(flt_value);

        se_entry.GetValue("spot_power", flt_value);
        llinfo.light.set_spot_power(flt_value);
    }

    //
    // Anything else is common to all light types.
    v8::math::color_rgb color_vec;
    se_entry.GetValue("ambient", color_vec.components_);
    llinfo.light.set_ambient_color(color_vec);

    se_entry.GetValue("specular", color_vec.components_);
    llinfo.light.set_specular_color(color_vec);

    se_entry.GetValue("diffuse", color_vec.components_);
    llinfo.light.set_diffuse_color(color_vec);

    if (!light_proc_del_.empty()) {
        light_proc_del_(llinfo.light, is_enabled);
    }
}

void v8::scene::section_reader_std_object::read_section_entry(
    const io::V8ConfigFile_ReadOnlySection& se_entry
    ) {
    standard_object_loading_info_t std_obj_info;

    bool is_enabled;
    se_entry.GetValue("enabled", is_enabled);
    if (!is_enabled) {
        return;
    }

    const char* k_string_val = nullptr;
    se_entry.GetValue("mesh", k_string_val);
    std_obj_info.mesh = k_string_val;

    se_entry.GetValue("material", k_string_val);
    std_obj_info.material = k_string_val;

    se_entry.GetValue("scale", std_obj_info.scale);
    se_entry.GetValue("translation", std_obj_info.translate);
    se_entry.GetValue("rotate", std_obj_info.rotate);

    if (!std_obj_del_.empty()) {
        std_obj_del_(std_obj_info);
    }
}

void v8::scene::section_reader_userdefined_object::read_section_entry(
    const io::V8ConfigFile_ReadOnlySection& se_entry
    ) {
    userdefined_object_loading_info_t object_info;

    bool is_enabled = false;
    se_entry.GetValue("enabled", is_enabled);
    if (!is_enabled) {
        return;
    }

    const char* k_string_val = nullptr;
    se_entry.GetValue("name", k_string_val);
    object_info.name = k_string_val;

    se_entry.GetValue("config_file", k_string_val);
    object_info.config_file = k_string_val;

    se_entry.GetValue("type", object_info.type);

    if (!user_obj_del_.empty()) {
        user_obj_del_(object_info);
    }
}

void v8::scene::seaction_reader_standard_entity::read_section_entry(
    const io::V8ConfigFile_ReadOnlySection& se_entry
    ) {
    standard_entity_load_info_t ent_load_info;

    const char* string_entry = nullptr;
    se_entry.GetValue("name", string_entry);
    ent_load_info.ent_name = string_entry;

    se_entry.GetValue("config_file", string_entry);
    ent_load_info.ent_conf_file = string_entry;

    se_entry.GetValue("translate", ent_load_info.ent_translate);
    se_entry.GetValue("orientation", ent_load_info.ent_orientation);
    se_entry.GetValue("scale", ent_load_info.ent_scale);

    if (!std_ent_del_.empty()) {
        std_ent_del_(ent_load_info);
    }
}
