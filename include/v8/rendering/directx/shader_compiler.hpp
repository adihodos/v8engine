#pragma once

#include <d3d11.h>
#include <v8/v8.hpp>

namespace v8 { namespace directx {

//! \name Shader compilation functions.
//! @{

//!
//! \brief Compiles a shader file containing shader source code
//! into bytecode.
//! \param shader_file_path Pointer to a string that contains the path of
//! shader source file.
//! \param shader_entry_point Entry point function of the shader.
//! \param shader_target_profile Profile to compile for 
//! (shader model 3_0, 4_0, 4_1, 5_0).
//! \param shader_compile_flags Compilation flags that affect code generation.
//! \returns Pointer to compiled bytecode interface, null on failure.
ID3D10Blob* compile_shader_from_file(
    const char* shader_file_path,
    const char* shader_entry_point,
    const char* shader_target_profile,
    v8_uint_t shader_compile_flags
    );

//!
//! \brief Compiles shader source code, in memory, into bytecode.
//! \param shader_source_code Pointer to a string that contains the source
//! code of the shader.
//! \param source_code_len Length of the string.
//! \param shader_entry_point Entry point function of the shader.
//! \param shader_target_profile Profile to compile for 
//! (shader model 3_0, 4_0, 4_1, 5_0).
//! \param shader_compile_flags Compilation flags that affect code generation.
//! \returns Pointer to compiled bytecode interface, null on failure.
ID3D10Blob* compile_shader_from_memory(
    const char* shader_source_code,
    v8_size_t source_code_len,
    const char* shader_entry_point,
    const char* shader_target_profile,
    v8_uint_t shader_compile_flags
    );

} // namespace directx
} // namespace v8
