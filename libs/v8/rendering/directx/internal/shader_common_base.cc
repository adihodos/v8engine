#include <d3dcompiler.h>
#include <algorithm>

#include "v8/base/debug_helpers.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/shader_common_base.hpp"

void v8::directx::internal::shader_common_base::reflect_constant_buffer(
    ID3D11Device*   device,
    ID3D11ShaderReflection* shader_reflect,
    v8_uint32_t buff_index
    ) {

    //
    // Get a constant buffer reflection interface pointer based on
    // the buffer's index.
    ID3D11ShaderReflectionConstantBuffer* constbuffer_reflector = 
        shader_reflect->GetConstantBufferByIndex(buff_index);
    if (!constbuffer_reflector) {
        OUTPUT_DBG_MSGW(L"Warning : failed to reflect constant buffer @ slot %ud",
                        buff_index);
        return;
    }

    //
    // Reflect the buffer and build the corresponding shader uniform block.
    D3D11_SHADER_BUFFER_DESC sbd;
    HRESULT ret_code;
    CHECK_D3D(&ret_code, constbuffer_reflector->GetDesc(&sbd));
    if (FAILED(ret_code)) 
        return;

    uniform_blocks.push_back(shader_uniform_block_t());

    if (!uniform_blocks.back().initialize(device, sbd, buff_index)) {
        uniform_blocks.pop_back();
        return;
    }

    //
    // Push handle of constant buffer into list. 
    uniform_block_binding_list.push_back(uniform_blocks.back().get_handle());

    //
    // Reflect each individual variable in the cbuffer.
    for (v8_uint32_t i = 0; i < sbd.Variables; ++i) {
        auto V_reflector = constbuffer_reflector->GetVariableByIndex(i);
        if (!V_reflector) {
            OUTPUT_DBG_MSGA("Warning : failed to reflect variable @ %ud "
                            "for constant buffer %s",
                            i, sbd.Name);
            return;
        }

        D3D11_SHADER_VARIABLE_DESC var_desc;
        CHECK_D3D(&ret_code, V_reflector->GetDesc(&var_desc));
        if (FAILED(ret_code))
            continue;

        uniforms.push_back(shader_uniform_t(&uniform_blocks.back(), var_desc));
    }
}

void v8::directx::internal::shader_common_base::reflect_bound_resource(
    ID3D11ShaderReflection* shader_reflector,
    v8_uint32_t index
    ) {

    D3D11_SHADER_INPUT_BIND_DESC bound_res_desc;
    HRESULT ret_code;

    CHECK_D3D(&ret_code, shader_reflector->GetResourceBindingDesc(
        index, &bound_res_desc));

    if (FAILED(ret_code))
        return;

    if (bound_res_desc.Type == D3D10_SIT_SAMPLER) {
        sampler_states.push_back(sampler_state_t(bound_res_desc.Name,
                                                 bound_res_desc.BindPoint));
    } else if (bound_res_desc.Type == D3D10_SIT_TEXTURE) {
        resource_views.push_back(
            resource_view_t(bound_res_desc.Name, bound_res_desc.BindPoint, 
                            bound_res_desc.Dimension, bound_res_desc.BindCount));
    } else {
        OUTPUT_DBG_MSGW(L"Unknown resource, index %u", index);
    }
}

void v8::directx::internal::shader_common_base::get_input_signature( 
    ID3D11Device*               k_device,
    ID3D11ShaderReflection*     k_reflector, 
    ID3D10Blob*                 k_bytecode,
    const v8_uint32_t           k_num_input_params
    ) {
    assert(k_device);
    assert(k_reflector);
    assert(k_bytecode);
    assert(!input_signature);

    std::vector<D3D11_INPUT_ELEMENT_DESC> input_elem_desc;
    input_elem_desc.reserve(k_num_input_params);

    for (v8_uint32_t param_idx = 0; param_idx < k_num_input_params; ++param_idx) {
        D3D11_INPUT_ELEMENT_DESC        curr_elem_desc;
        D3D11_SIGNATURE_PARAMETER_DESC  curr_param_desc;
        HRESULT                         ret_code;

        CHECK_D3D(
            &ret_code,
            k_reflector->GetInputParameterDesc(param_idx, &curr_param_desc));
        if (FAILED(ret_code)) {
            break;
        }

        v8_bool_t translation_success = input_param_description_to_element_description(
            curr_param_desc, &curr_elem_desc);
        if (!translation_success) {
            break;
        }
        input_elem_desc.push_back(curr_elem_desc);
    }

    if (input_elem_desc.empty()) {
        return;
    }

    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        k_device->CreateInputLayout(
            &input_elem_desc[0], static_cast<UINT>(input_elem_desc.size()),
            k_bytecode->GetBufferPointer(), k_bytecode->GetBufferSize(),
            v8::base::scoped_pointer_get_impl(input_signature)));
}

v8_bool_t v8::directx::internal::shader_common_base::reflect_shader(
    ID3D10Blob* compiled_bytecode,
    ID3D11Device* device
    ) {
    assert(compiled_bytecode);
    assert(device);
    assert(!bytecode);
    assert(!input_signature);

    bytecode = compiled_bytecode;

    using namespace v8::base;

    com_exclusive_pointer<ID3D11ShaderReflection>::type shader_reflector;
    HRESULT ret_code;

    CHECK_D3D(
        &ret_code,
        D3DReflect(compiled_bytecode->GetBufferPointer(), 
                   compiled_bytecode->GetBufferSize(),
                   IID_ID3D11ShaderReflection,
                   reinterpret_cast<void**>(scoped_pointer_get_impl(
                           shader_reflector))));

    if (FAILED(ret_code)) {
        return false;
    }

    D3D11_SHADER_DESC shader_description;
    CHECK_D3D(&ret_code, shader_reflector->GetDesc(&shader_description));

    if (FAILED(ret_code)) {
        return false;
    }

    for (v8_uint32_t i = 0; i < shader_description.ConstantBuffers; ++i) {
        reflect_constant_buffer(device, 
                                scoped_pointer_get(shader_reflector), i);
    }

    for (v8_uint32_t i = 0; i < shader_description.BoundResources; ++i) {
        reflect_bound_resource(scoped_pointer_get(shader_reflector), i);
    }

    get_input_signature(device,
                        v8::base::scoped_pointer_get(shader_reflector),
                        compiled_bytecode, shader_description.InputParameters);

    //
    // 
    // Sort all resources by name, so we can binary search them.
    std::sort(
        std::begin(uniform_blocks),
        std::end(uniform_blocks),
        [](const shader_uniform_block_t& left, const shader_uniform_block_t& right) {
            return left.name_ < right.name_;
    });

    std::sort(
        std::begin(uniforms),
        std::end(uniforms),
        [](const shader_uniform_t& lhs, const shader_uniform_t& rhs) {
            return lhs.name_ < rhs.name_;
    });

    std::sort(
        std::begin(sampler_states),
        std::end(sampler_states),
        [](const sampler_state_t& lhs, const sampler_state_t& rhs) {
            return lhs.name_ < rhs.name_;
    });

    std::sort(
        std::begin(resource_views),
        std::end(resource_views),
        [](const resource_view_t& lhs, const resource_view_t& rhs) {
            return lhs.rvt_name < rhs.rvt_name;
    });
    
    sampler_state_binding_list.resize(sampler_states.size());
    resource_view_binding_list.resize(resource_views.size());
    return true;
}
