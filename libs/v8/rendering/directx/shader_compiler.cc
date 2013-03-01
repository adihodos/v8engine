#include <d3dcompiler.h>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include "v8/base/scoped_pointer.hpp"

#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/include_handler.hpp"

#include "v8/rendering/directx/shader_compiler.hpp"

ID3D10Blob* v8::directx::compile_shader_from_file(
    const char* shader_file_path,
    const char* shader_entry_point,
    const char* shader_target_profile,
    v8_uint_t shader_compile_flags
    ) {

    try {
        platformstl::memory_mapped_file mmfile(shader_file_path);
        return compile_shader_from_memory(static_cast<const char*>(mmfile.memory()),
                                          mmfile.size(),
                                          shader_entry_point,
                                          shader_target_profile,
                                          shader_compile_flags);
    } catch (const std::exception&) {}
    
    return nullptr;
}

ID3D10Blob* v8::directx::compile_shader_from_memory(
    const char* shader_source_code,
    v8_size_t source_code_len,
    const char* shader_entry_point,
    const char* shader_target_profile,
    v8_uint_t shader_compile_flags
    ) {

    using namespace v8::base;

    scoped_ptr<ID3D10Blob, com_storage> compiled_bytecode;
    scoped_ptr<ID3D10Blob, com_storage> error_msg;
    HRESULT ret_code;
    internal::compiler_include_handler include_handler;

    CHECK_D3D(
        &ret_code,
        D3DCompile(shader_source_code, source_code_len, nullptr, nullptr, 
                   &include_handler, shader_entry_point, 
                   shader_target_profile, shader_compile_flags, 0,
                   scoped_pointer_get_impl(compiled_bytecode),
                   scoped_pointer_get_impl(error_msg)));

    if (FAILED(ret_code) && error_msg && error_msg->GetBufferPointer()) {
        OUTPUT_DBG_MSGA("Shader compilation error %s", 
                        error_msg->GetBufferPointer());
    }

    return scoped_pointer_release(compiled_bytecode);
}
