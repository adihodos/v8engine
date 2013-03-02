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
};

} // namespace 
}
