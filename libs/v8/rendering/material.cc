#include <v8/base/debug_helpers.hpp>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>

#include "v8/global_state.hpp"
#include "v8/io/config_file_reader.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/constants.hpp"
#include "v8/rendering/effect.hpp"
#include "v8/rendering/render_assets_cache.hpp"
#include "v8/rendering/texture_info.hpp"
#include "v8/rendering/texture.hpp"
#include "v8/utility/hash_string.hpp"

#include "v8/rendering/material.hpp"

//const v8::rendering::material 
//v8::rendering::material::k_default_material(
//    v8::math::color_rgb::C_Black,
//    v8::math::color_rgb(0.0f, 0.498f, 1.0f, 1.0f),
//    v8::math::color_rgb(0.0f, 0.67f, 0.84f, 1.0f),
//    v8::math::color_rgb(0.0f, 1.0f, 1.0f, 95.2f));
//
//const v8::rendering::material 
//v8::rendering::material::k_polished_copper(
//    v8::math::color_rgb::C_Black,
//    v8::math::color_rgb(0.2295f, 0.08825f, 0.0275f, 1.0f),
//    v8::math::color_rgb(0.5508f, 0.2118f, 0.066f, 1.0f),
//    v8::math::color_rgb(0.580594f, 0.223257f, 0.0695701f, 51.2f));
//
//const v8::rendering::material 
//v8::rendering::material::k_polished_gold(
//    v8::math::color_rgb::C_Black,
//    v8::math::color_rgb(0.24725f, 0.2245f, 0.0645f, 1.0f),
//    v8::math::color_rgb(0.34615f, 0.3143f, 0.0903f, 1.0f),
//    v8::math::color_rgb(0.797357f, 0.723991f, 0.208006f, 83.2f));


v8_bool_t v8::rendering::material::initialize(const hash_string& mat_file) {
    platformstl::path mtl_conf_path(state->file_sys()->get_dir_path(
        filesys::Dir::Config));
    mtl_conf_path.push(mat_file.c_str());
    mtl_conf_path.push_ext("json");

    const io::V8ConfigFile mtl_conf_file(mtl_conf_path.c_str());
    io::V8ConfigFile_ReadOnlySection conf_main(mtl_conf_file.GetSection());

    conf_main.GetValue("type", type);
    if (type == Type_Color) {
        conf_main.GetValue("emissive", color.emissive);
        conf_main.GetValue("ambient", color.ambient);
        conf_main.GetValue("specular", color.specular);
        conf_main.GetValue("diffuse", color.diffuse);
    } else if (type == Type_Texture) {
        const char* tex_filename = nullptr;
        conf_main.GetValue("emissive", tex_filename);

        texture_info_t tex_info(tex_filename, BindingFlag::ShaderResource);

        tex.emissive = state->asset_cache()->get_texture(tex_info);
        if (!tex.emissive) {
            return false;
        }

        conf_main.GetValue("ambient", tex_filename);
        tex_info.tex_filename = tex_filename;
        tex.ambient = state->asset_cache()->get_texture(tex_info);

        conf_main.GetValue("diffuse", tex_filename);
        tex_info.tex_filename = tex_filename;
        tex.diffuse = state->asset_cache()->get_texture(tex_info);
        if (!tex.diffuse) {
            return false;
        }
        if (tex.ambient) {
            tex.ambient = tex.diffuse;
        }

        conf_main.GetValue("specular", tex_filename);
        tex_info.tex_filename = tex_filename;
        tex.specular = state->asset_cache()->get_texture(tex_info);
        if (!tex.specular) {
            return false;
        }
    } else {
        OUTPUT_DBG_MSGA("Unknown material type (%d)", type);
        return false;
    }

    return true;
}

void v8::rendering::material::bind_to_effect(effect* eff) {
    assert(eff);

    if (type == Type_Texture) {
        eff->set_variable_by_semantic("SpecularMap", tex.specular);
        eff->set_variable_by_semantic("DiffuseMap", tex.diffuse);
        eff->set_variable_by_semantic("AmbientMap", tex.ambient);
    } else {
        eff->set_variable_by_semantic("EmissiveColor", color.emissive);
        eff->set_variable_by_semantic("AmbientColor", color.ambient);
        eff->set_variable_by_semantic("DiffuseColor", color.diffuse);
        eff->set_variable_by_semantic("SpecularColor", color.specular);
    }
}

v8::rendering::material 
v8::rendering::material::default() {
    material mtl;
    mtl.type = Type_Color;

    using v8::math::color_rgb;
    *(color_rgb*) mtl.color.emissive = color_rgb::C_Black;
    *(color_rgb*) mtl.color.ambient = color_rgb(0.0f, 0.498f, 1.0f, 1.0f);
    *(color_rgb*) mtl.color.diffuse = color_rgb(0.0f, 0.67f, 0.84f, 1.0f);
    *(color_rgb*) mtl.color.specular = color_rgb(0.0f, 1.0f, 1.0f, 95.2f);

    return mtl;
}
