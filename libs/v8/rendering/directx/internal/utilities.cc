#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <third_party/stlsoft/rangelib/sequence_range.hpp>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>

#include "v8/base/com_exclusive_pointer.hpp"
#include "v8/base/fixed_pod_vector.hpp"
#include "v8/base/fixed_pod_stack.hpp"
#include "v8/utility/hash_spooky.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/shader_info.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/include_handler.hpp"

namespace {

const DXGI_FORMAT kTypeless32Mappings[] = {
    DXGI_FORMAT_R32_TYPELESS,
    DXGI_FORMAT_R32G32_TYPELESS,
    DXGI_FORMAT_R32G32B32_TYPELESS,
    DXGI_FORMAT_R32G32B32A32_TYPELESS
};

const DXGI_FORMAT kFloat32Mappings[] = { 
    DXGI_FORMAT_R32_FLOAT,
    DXGI_FORMAT_R32G32_FLOAT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32A32_FLOAT
};

const DXGI_FORMAT kUint32Mappings[] = {
    DXGI_FORMAT_R32_UINT,
    DXGI_FORMAT_R32G32_UINT,
    DXGI_FORMAT_R32G32B32_UINT,
    DXGI_FORMAT_R32G32B32A32_UINT
};

const DXGI_FORMAT kSint32Mappings[] = {
    DXGI_FORMAT_R32_SINT,
    DXGI_FORMAT_R32G32_SINT,
    DXGI_FORMAT_R32G32B32_SINT,
    DXGI_FORMAT_R32G32B32A32_FLOAT
};

const DXGI_FORMAT kTypeless16Mappings[] = {
    DXGI_FORMAT_R16_TYPELESS,
    DXGI_FORMAT_R16G16_TYPELESS,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R16G16B16A16_TYPELESS
};

const DXGI_FORMAT kFloat16Mappings[] = { 
    DXGI_FORMAT_R16_FLOAT,
    DXGI_FORMAT_R16G16_FLOAT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R16G16B16A16_FLOAT
};

const DXGI_FORMAT kUint16Mappings[] = {
    DXGI_FORMAT_R16_UINT,
    DXGI_FORMAT_R16G16_UINT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R16G16B16A16_UINT
};

const DXGI_FORMAT kSint16Mappings[] = {
    DXGI_FORMAT_R16_SINT,
    DXGI_FORMAT_R16G16_SINT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R16G16B16A16_SINT
};

const DXGI_FORMAT kUnorm16Mappings[] = {
    DXGI_FORMAT_R16_UNORM,
    DXGI_FORMAT_R16G16_UNORM,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R16G16B16A16_UNORM
};

const DXGI_FORMAT kTypeless8Mappings[] = {
    DXGI_FORMAT_R8_TYPELESS,
    DXGI_FORMAT_R8G8_TYPELESS,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R8G8B8A8_TYPELESS
};

const DXGI_FORMAT kUnorm8Mappings[] = {
    DXGI_FORMAT_R8_UNORM,
    DXGI_FORMAT_R8G8_UNORM,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R8G8B8A8_UNORM
};

const DXGI_FORMAT kUint8Mappings[] = {
    DXGI_FORMAT_R8_UINT,
    DXGI_FORMAT_R8G8_UINT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R8G8B8A8_UINT
};

const DXGI_FORMAT kSint8Mappings[] = {
    DXGI_FORMAT_R8_SINT,
    DXGI_FORMAT_R8G8_SINT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R8G8B8A8_SINT
};

inline DXGI_FORMAT get_format_of_index(
    v8_int_t index,
    const DXGI_FORMAT* fmt_values,
    v8_size_t max_val
    ) { 
    assert((index >= 1) && (index <= static_cast<int>(max_val)));
    return fmt_values[index - 1];
}    

const DXGI_FORMAT k_comp_type_to_dxgi_map[4][3] = {
    { DXGI_FORMAT_R32_UINT, DXGI_FORMAT_R32_SINT, DXGI_FORMAT_R32_FLOAT },
    { DXGI_FORMAT_R32G32_UINT, DXGI_FORMAT_R32G32_SINT, DXGI_FORMAT_R32G32_FLOAT },
    { DXGI_FORMAT_R32G32B32_UINT, DXGI_FORMAT_R32G32B32_SINT, DXGI_FORMAT_R32G32B32_FLOAT },
    { DXGI_FORMAT_R32G32B32A32_UINT, DXGI_FORMAT_R32G32B32A32_SINT, DXGI_FORMAT_R32G32B32A32_FLOAT }
};

static_assert((D3D_REGISTER_COMPONENT_UINT32 == 1), 
    "Enum value is wrong, will likely cause access violation "
    "when used to map D3D_REGISTER_TYPE to DXGI_FORMAT");

static_assert((D3D_REGISTER_COMPONENT_SINT32 == 2),
    "Enum value is wrong, will likely cause access violation "
    "when used to map D3D_REGISTER_TYPE to DXGI_FORMAT");

static_assert((D3D_REGISTER_COMPONENT_FLOAT32 == 3),
    "Enum value is wrong, will likely cause access violation "
    "when used to map D3D_REGISTER_TYPE to DXGI_FORMAT");

} // anonymous namespace

v8_bool_t v8::directx::parse_macro_string(
    const char* str_ptr, std::vector<user_defined_macro_t>* extracted_macros 
    ) {
    v8::base::fixed_pod_stack<char>     p_stk;
    v8::base::fixed_pod_vector<char>    p_tokens;

    const char kSquareBracketLeft = '[';
    const char kSquareBracketRight = ']';
    const char kRoundParenLeft = '(';
    const char kRoundParenRight = ')';

    const char* p = str_ptr;
    user_defined_macro_t current_macro;

    while (*p) {

        if (*p == kSquareBracketLeft) {
            if (!p_stk.empty()) {
                //
                // error, '[' nt allowed here
                return false;
            }

            p_stk.push(*p++);
            continue;
        }

        if (*p == kSquareBracketRight) {
            if (p_stk.top() != kSquareBracketLeft) {
                //
                // error, mismatched ...
                return false;
            }

            p_stk.pop();

            if (!p_tokens.empty()) {
                //
                // extra chars after () are not allowed
                if (!current_macro.define.empty()) {
                    return false;
                }

                p_tokens.push_back('\0');
                current_macro.define.assign(&p_tokens[0]);
                p_tokens.clear();
            }

            extracted_macros->push_back(current_macro);
            current_macro.clear();
            ++p;
            continue;
        }

        if (isspace(*p)) {
            ++p;
            continue;
        }

        if (*p == kRoundParenLeft) {

            const char last_token = p_stk.top();

            if ((last_token != kSquareBracketLeft)) {
                //
                // error, () can olny appear inside []
                return false;
            }

            p_stk.push(*p);

            p_tokens.push_back('\0');
            current_macro.define.assign(&p_tokens[0]);
            p_tokens.clear();
            ++p;
            continue;
        }

        if (*p == kRoundParenRight) {
            if (p_stk.top() != kRoundParenLeft) {
                //
                // err, not allowed here
                return false;
            }

            p_stk.pop();
            p_tokens.push_back('\0');


            //
            // empty value allowed
            current_macro.value.assign(&p_tokens[0]);
            p_tokens.clear();
            ++p;
            continue;
        }

        p_tokens.push_back(*p++);
    }
    return true;
}

v8::base::duo<v8_bool_t, ID3D10Blob*> v8::directx::compile_shader(
    const shader_info_t& shdr_info
    ) {
    typedef v8::base::duo<v8_bool_t, ID3D10Blob*> returned_type;
    returned_type ret_val(false, nullptr);

    assert(shdr_info.entrypoint);
    assert(shdr_info.shader_model);

    std::vector<user_defined_macro_t> compile_macros;
    if (shdr_info.compile_macros) {
        if (!parse_macro_string(shdr_info.compile_macros,&compile_macros)) {
            return ret_val;
        }
    }

    std::vector<D3D10_SHADER_MACRO> macro_list;
    macro_list.reserve(compile_macros.size() + 1);
    rangelib::sequence_range<
        const std::vector<user_defined_macro_t>
    > r(compile_macros);

    while (r) {
        const D3D10_SHADER_MACRO macro = { 
            (*r).define.c_str(), (*r).value.c_str() 
        };
        macro_list.push_back(macro);
        ++r;
    }

    const D3D10_SHADER_MACRO macrolist_terminator = { nullptr, nullptr };
    macro_list.push_back(macrolist_terminator);

    const v8_uint32_t k_compile_flags = 
        translate_compiler_options_to_directx_value(shdr_info.compile_flags);

    v8::base::com_exclusive_pointer<ID3D10Blob>::type   bytecode;
    v8::base::com_exclusive_pointer<ID3D10Blob>::type   err_msg;
    internal::compiler_include_handler                  include_handler(
        shdr_info.shader_root_directory.c_str());
    HRESULT                                             ret_code;

    if (shdr_info.is_filename) {
        const platformstl::memory_mapped_file mmfile(shdr_info.name_or_source);

        CHECK_D3D(
            &ret_code,
            D3DCompile(mmfile.memory(), mmfile.size(), nullptr, &macro_list[0],
                       &include_handler, shdr_info.entrypoint, 
                       shdr_info.shader_model, k_compile_flags, 0,
                       v8::base::scoped_pointer_get_impl(bytecode),
                       v8::base::scoped_pointer_get_impl(err_msg)));
    } else {
        CHECK_D3D(
            &ret_code,
            D3DCompile(shdr_info.name_or_source.c_str(), 
                       static_cast<UINT>(shdr_info.name_or_source.length()),
                       nullptr, &macro_list[0], &include_handler, 
                       shdr_info.entrypoint, shdr_info.shader_model,
                       k_compile_flags, 0,
                       v8::base::scoped_pointer_get_impl(bytecode),
                       v8::base::scoped_pointer_get_impl(err_msg)));
    }

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to compile shader [%s]", 
                        shdr_info.name_or_source.c_str());
        OUTPUT_DBG_MSGA("Compiler error [%s]", err_msg->GetBufferPointer() ?
            static_cast<const char*>(err_msg->GetBufferPointer()) : "not available"
            );
        return ret_val;
    }

    ret_val.First()     = true;
    ret_val.Second()    = v8::base::scoped_pointer_release(bytecode);
    return ret_val;
}

v8_bool_t v8::directx::input_param_description_to_element_description(
    const D3D11_SIGNATURE_PARAMETER_DESC&   input_parm_signature,
    D3D11_INPUT_ELEMENT_DESC*               element_desc
    ) {
    assert(element_desc);

    element_desc->AlignedByteOffset     = D3D11_APPEND_ALIGNED_ELEMENT;
    element_desc->InputSlot             = 0;
    element_desc->InputSlotClass        = D3D11_INPUT_PER_VERTEX_DATA;
    element_desc->InstanceDataStepRate  = 0;
    element_desc->SemanticIndex         = input_parm_signature.SemanticIndex;
    element_desc->SemanticName          = input_parm_signature.SemanticName;

    const D3D_REGISTER_COMPONENT_TYPE k_comp_type = input_parm_signature.ComponentType;
    if (input_parm_signature.Mask == 0x01) {
        //
        // single register is used
        element_desc->Format = k_comp_type_to_dxgi_map[0][k_comp_type - 1];
    } else if (input_parm_signature.Mask == 0x03) {
        //
        // Two registers
        element_desc->Format = k_comp_type_to_dxgi_map[1][k_comp_type - 1];
    } else if (input_parm_signature.Mask == 0x07) {
        //
        // Three registers used
        element_desc->Format = k_comp_type_to_dxgi_map[2][k_comp_type - 1];
    } else if (input_parm_signature.Mask == 0x0F) {
        //
        // Four registers used
        element_desc->Format = k_comp_type_to_dxgi_map[3][k_comp_type - 1];
    } else {
        //
        // Should not get here
        OUTPUT_DBG_MSGA("Component uses %u registers ???", 
            input_parm_signature.Mask);
        return false;
    }

    return true;
}

DXGI_FORMAT v8::directx::string_code_to_dxgi_format(
    const char* string,
    v8_int_t num_elements
    ) {
    return hash_code_to_dxgi_format(
        utility::SpookyHash::Hash32(string, strlen(string), 0),
        num_elements
        );
}

DXGI_FORMAT v8::directx::hash_code_to_dxgi_format(
    v8_uint32_t hash_code_element_type,
    v8_int_t num_elements
    ) {
    switch(hash_code_element_type) {
        default : 
        break;

    case 0x214ec143 : // Typeless32 
        return get_format_of_index(num_elements, kTypeless32Mappings, 4);
        break;

    case 0x7a39c679 : // Float32 
        return get_format_of_index(num_elements, kFloat32Mappings, 4);
        break;

    case 0xa2708b69 : // Uint32 
        return get_format_of_index(num_elements, kUint32Mappings, 4);
        break;

    case 0xfa847acf : // Sint32 
        return get_format_of_index(num_elements, kSint32Mappings, 4);
        break;

    case 0xf1bee0ea : // Typeless16 
        return get_format_of_index(num_elements, kTypeless16Mappings, 4);
        break;

    case 0xddee0a2b : // Float16 
        return get_format_of_index(num_elements, kFloat16Mappings, 4);
        break;

    case 0xa26171b2 : // Unorm16 
        return get_format_of_index(num_elements, kUnorm16Mappings, 4);
        break;

    case 0x74d3af77 : // Uint16 
        return get_format_of_index(num_elements, kUint16Mappings, 4);
        break;

    case 0x85c4a0a7 : // Typeless8 
        return get_format_of_index(num_elements, kTypeless8Mappings, 4);
        break;

    case 0xcd62cbea : // Unorm8 
        return get_format_of_index(num_elements, kUnorm8Mappings, 4);
        break;

    case 0x91d547c6 : // Uint8 
        return get_format_of_index(num_elements, kUint8Mappings, 4);
        break;

    case 0x107745ed : // Sint8 
        return get_format_of_index(num_elements, kSint8Mappings, 4);
        break;
    }

    return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT v8::directx::element_type_to_dxgi_format(
    v8_int_t element_type,
    v8_int_t num_elements
    ) {
    switch (element_type) {
        default :
        break;
    case ElementType::Typeless32 :
        return get_format_of_index(num_elements, kTypeless32Mappings, 4);
        break;
    case ElementType::Float32 :
        return get_format_of_index(num_elements, kFloat32Mappings, 4);
        break;
    case ElementType::Uint32 :
        return get_format_of_index(num_elements, kUint32Mappings, 4);
        break;
    case ElementType::Sint32 :
        return get_format_of_index(num_elements, kSint32Mappings, 4);
        break;
    case ElementType::Typeless16 :
        return get_format_of_index(num_elements, kTypeless16Mappings, 4);
        break;
    case ElementType::Float16 :
        return get_format_of_index(num_elements, kFloat16Mappings, 4);
        break;
    case ElementType::Unorm16 :
        return get_format_of_index(num_elements, kUnorm16Mappings, 4);
        break;
    case ElementType::Uint16 :
        return get_format_of_index(num_elements, kUint16Mappings, 4);
        break;
    case ElementType::Typeless8 :
        return get_format_of_index(num_elements, kTypeless8Mappings, 4);
        break;
    case ElementType::Unorm8 :
        return get_format_of_index(num_elements, kUnorm8Mappings, 4);
        break;
    case ElementType::Uint8 :
        return get_format_of_index(num_elements, kUint8Mappings, 4);
        break;
    case ElementType::Sint8 :
        return get_format_of_index(num_elements, kSint8Mappings, 4);
        break;
    }

    return DXGI_FORMAT_UNKNOWN;
}

v8_uint32_t 
v8::directx::translate_compiler_options_to_directx_value(
    const v8_uint32_t options
    ) {
    v8_uint32_t dx_flag_value = 0;

    if (options & Compile_Options::Avoid_Flow_Control) {
        dx_flag_value |= D3D10_SHADER_AVOID_FLOW_CONTROL;
    }

    if (options & Compile_Options::Enable_Backwards_Compatibility) {
        dx_flag_value |= D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
    }

    if (options & Compile_Options::Generate_Debug_Info) {
        dx_flag_value |= D3D10_SHADER_DEBUG;
    }

    if (options & Compile_Options::IEEE_Strictness) {
        dx_flag_value |= D3D10_SHADER_IEEE_STRICTNESS;
    }

    if (options & Compile_Options::Matrix_Packing_Column_Major) {
        dx_flag_value |= D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
    }

    if (options & Compile_Options::Matrix_Packing_Row_Major) {
        dx_flag_value |= D3D10_SHADER_PACK_MATRIX_ROW_MAJOR;
    }

    if (options & Compile_Options::No_Preshader) {
        dx_flag_value |= D3D10_SHADER_NO_PRESHADER;
    }

    if (options & Compile_Options::Optimization_L0) {
        dx_flag_value |= D3D10_SHADER_OPTIMIZATION_LEVEL0;
    }

    if (options & Compile_Options::Optimization_L1) {
        dx_flag_value |= D3D10_SHADER_OPTIMIZATION_LEVEL1;
    }

    if (options & Compile_Options::Optimization_L2) {
        dx_flag_value |= D3D10_SHADER_OPTIMIZATION_LEVEL2;
    }

    if (options & Compile_Options::Optimization_L3) {
        dx_flag_value |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
    }

    if (options & Compile_Options::PS_Software) {
        dx_flag_value |= D3D10_SHADER_FORCE_PS_SOFTWARE_NO_OPT;
    }

    if (options & Compile_Options::Skip_Optimization) {
        dx_flag_value |= D3D10_SHADER_SKIP_OPTIMIZATION;
    }

    if (options & Compile_Options::Skip_Validation) {
        dx_flag_value |= D3D10_SHADER_SKIP_VALIDATION;
    }

    if (options & Compile_Options::Use_Partial_Precision) {
        dx_flag_value |= D3D10_SHADER_PARTIAL_PRECISION;
    }

    if (options & Compile_Options::Use_Strict_Syntax) {
        dx_flag_value |= D3D10_SHADER_ENABLE_STRICTNESS;
    }

    if (options & Compile_Options::VS_Software) {
        dx_flag_value |= D3D10_SHADER_FORCE_VS_SOFTWARE_NO_OPT;
    }

    if (options & Compile_Options::Warnings_Are_Errors) {
        dx_flag_value |= D3D10_SHADER_WARNINGS_ARE_ERRORS;
    }

    if (options & Compile_Options::Prefer_Flow_Control) {
        dx_flag_value |= D3D10_SHADER_PREFER_FLOW_CONTROL;
    }

    return dx_flag_value;
    /*return options;*/
}
