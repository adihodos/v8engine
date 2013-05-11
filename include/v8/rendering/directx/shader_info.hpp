#pragma once

#include <string>
#include <v8/v8.hpp>

namespace v8 { namespace directx {

struct shader_info_t {

    shader_info_t()
        :       is_filename(false)
            ,   compile_macros(nullptr)
            ,   entrypoint(nullptr)
            ,   compile_flags(0)
            ,   shader_model(nullptr)
    {
    }

    ///< Either the name of a shader file or the source code.
    std::string                 name_or_source;

    ///< True if name_or_source represents a file name.
    v8_bool_t                   is_filename;

    ///< Path of the directory containing the shader. If the shader contains
    ///< #include directives, the #included files will be searched 
    ///< in this directory.
    std::string                 shader_root_directory;

    ///< List of macros to be passed to the compiler.
    const char*                 compile_macros;

    ///< Entry point function, in the shader's code.
    const char*                 entrypoint;

    ///< Compilation flags for the compiler.
    v8_uint32_t                 compile_flags;

    ///< Shader model to target (3_0, 4_0, 4_1, 5_0).
    const char*                 shader_model;
};

inline bool operator==(const shader_info_t& lhs, const shader_info_t& rhs) {
    if (lhs.compile_flags != rhs.compile_flags) {
        return false;
    }

    if (strcmp(lhs.entrypoint, rhs.entrypoint)) {
        return false;
    }

    if (strcmp(lhs.compile_macros, rhs.compile_macros)) {
        return false;
    }

    if (lhs.shader_model != rhs.shader_model) {
        return false;
    }
    return true;
}

inline bool operator!=(const shader_info_t& lhs, const shader_info_t& rhs) {
    return !(lhs == rhs);
}

} // namespace directx
} // namespace v8
