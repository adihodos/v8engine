#pragma once

#include <string>
#include <vector>
#include <D3D11.h>
#include <D3D11Shader.h>
#include <v8/base/duo.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 { namespace directx {

struct shader_info_t;

v8::base::duo<v8_bool_t, ID3D10Blob*> compile_shader(
    const shader_info_t& shdr_info
    );

struct user_defined_macro_t {

    void clear() {
        define.clear();
        value.clear();
    }

    std::string define;
    std::string value;
};

v8_bool_t parse_macro_string(
    const char* str_ptr, std::vector<user_defined_macro_t>* macros
    );

v8_bool_t input_param_description_to_element_description(
    const D3D11_SIGNATURE_PARAMETER_DESC& input_parm_signature,
    D3D11_INPUT_ELEMENT_DESC* element_desc
    );

DXGI_FORMAT element_type_to_dxgi_format(
    v8_int_t element_type,
    v8_int_t num_elements
    );

DXGI_FORMAT hash_code_to_dxgi_format(
    v8_uint32_t hash_code_element_type,
    v8_int_t num_elements
    );

DXGI_FORMAT string_code_to_dxgi_format(
    const char* string,
    v8_int_t num_elements
    );

v8_uint32_t 
translate_compiler_options_to_directx_value(
    const v8_uint32_t options
    );

} // namespace directx
} // namespace v8
