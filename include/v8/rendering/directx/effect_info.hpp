#pragma once

#include <string>

#include <v8/v8.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct effect_info_t {
    std::string     eff_filepath;
    std::string     eff_compile_macros;
    v8_uint32_t     eff_compile_flags;
    FX_Model::Type  eff_target;

    effect_info_t() 
        :       eff_compile_flags(0U)
            ,   eff_target(FX_Model::FX_4_1) {}

/*
    effect_info_t(
        const char* file_path,
        const char* compile_macros,
        v8_uint32_t compile_flags,
        FX_Model::Type model
        )
        :       eff_filepath(file_path)
            ,   eff_compile_macros(compile_macros)
            ,   eff_target(model)
            ,   eff_compile_flags(compile_flags)
    {
    }
*/    

    effect_info_t(
        const std::string& file_path,
        const std::string& compile_macros,
        const v8_uint32_t compile_flags,
        const FX_Model::Type model
        )
        :       eff_filepath(file_path)
            ,   eff_compile_macros(compile_macros)
            ,   eff_target(model)
            ,   eff_compile_flags(compile_flags)
    {
    }
};

inline bool operator==(const effect_info_t& lhs, const effect_info_t& rhs) {
    return  lhs.eff_target == rhs.eff_target 
            && lhs.eff_compile_flags == rhs.eff_compile_flags
            && lhs.eff_compile_macros == rhs.eff_compile_macros
            && lhs.eff_filepath == rhs.eff_filepath;
}

inline bool operator!=(const effect_info_t& lhs, const effect_info_t& rhs) {
    return !(lhs == rhs);
}

} // namespace 
}
