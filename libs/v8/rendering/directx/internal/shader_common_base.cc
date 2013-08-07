#include <d3dcompiler.h>
#include <d3d11shader.h>

#include <algorithm>

#include "v8/base/debug_helpers.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/shader_common_base.hpp"

///
///
/// enumerate all cbuffers and compute total number of global vars
/// reserve space for the total number of global vars (uniforms)
/// for each cbuffer reflect each uniform

///
/// \brief Contains data for a constant buffer used in a shader.
struct ConstBufferInfo_t {
    ///< Pointer to reflector interface.
    ID3D11ShaderReflectionConstantBuffer*   cbuff_reflect;
    ///< Info about the constant buffer.
    D3D11_SHADER_BUFFER_DESC                cbuff_desc;
    ///< Register (binding point) in the shader.
    v8_uint32_t                             cbuff_idx;
};

///
/// \brief Collects information about the constant buffers used by a shader.
/// \param device Pointer to a ID3D11Device interface.
/// \param shader_ref Pointer to a ID3D11ShaderReflection interface.
/// \param num_buffs Number of constant buffers in the shader's code.
/// \param shader_cbuff_data When the function returns, will contain data
/// on each of the constant buffers in the shader's code.
/// \return Total number of global variables (uniforms) in the shader.
v8_uint32_t reflect_shader_const_buffers(
    ID3D11Device*                       device,
    ID3D11ShaderReflection*             shader_ref,
    const v8_uint32_t                   num_buffs,
    std::vector<ConstBufferInfo_t>*     shader_cbuff_data
    ) {
    assert(device);
    assert(shader_ref);
    assert(shader_cbuff_data);

    shader_cbuff_data->clear();
    shader_cbuff_data->reserve(num_buffs);

    v8_uint32_t         num_global_vars = 0;
    ConstBufferInfo_t   cbuff_info;

    for (v8_uint32_t idx = 0; idx < num_buffs; ++idx) {
        cbuff_info.cbuff_reflect = shader_ref->GetConstantBufferByIndex(idx);
        if (!cbuff_info.cbuff_reflect) {
            OUTPUT_DBG_MSGA("Warning : failed to reflect constant buffer @ slot %ud",
                            idx);
            continue;
        }

        cbuff_info.cbuff_idx = idx;
        HRESULT ret_code;
        CHECK_D3D(
            &ret_code, 
            cbuff_info.cbuff_reflect->GetDesc(&cbuff_info.cbuff_desc)
            );
        if (FAILED(ret_code)) {
            continue;
        }

        num_global_vars += cbuff_info.cbuff_desc.Variables;
        shader_cbuff_data->push_back(cbuff_info);
    }
    return num_global_vars;
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
        OUTPUT_DBG_MSGW(L"Unknown resource type, index %u", index);
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

    //
    // Collect info about the constant buffers used by the shader.
    std::vector<ConstBufferInfo_t> shader_cbuff_data;
    const v8_uint32_t num_global_vars = reflect_shader_const_buffers(
        device, 
        scoped_pointer_get(shader_reflector), 
        shader_description.ConstantBuffers,
        &shader_cbuff_data
        );

    UNREFERENCED_PARAMETER(num_global_vars);

    //
    // Need to reserve space in advance, otherwise iterators might become invalid.
    uniform_blocks.reserve(shader_cbuff_data.size());

    for (v8_size_t idx = 0; idx < shader_cbuff_data.size(); ++idx) {
        const ConstBufferInfo_t& cb_info = shader_cbuff_data[idx];
        
        //
        // Each constant buffer in the shader is represented by a
        // shader_uniform_block_t object.
        scoped_ptr<shader_uniform_block_t> uniform_blk(new shader_uniform_block_t());
        //shader_uniform_block_t uniform_blk;
        if (!uniform_blk->initialize(device, cb_info.cbuff_desc, cb_info.cbuff_idx)) {
            OUTPUT_DBG_MSGA("Warning : failed to initialize uniform block for "
                            "constant buffer with index %u", cb_info.cbuff_idx);
            continue;
        }

        uniform_blocks.push_back(scoped_pointer_release(uniform_blk));
        uniform_block_binding_list.push_back(uniform_blocks.back()->get_handle());

        //
        // Reflect each global variable defined in the constant buffer.
        for (v8_uint32_t var_idx = 0; var_idx < cb_info.cbuff_desc.Variables;
             ++var_idx) {
            auto var_reflect = cb_info.cbuff_reflect->GetVariableByIndex(var_idx);
            if (!var_reflect) {
                OUTPUT_DBG_MSGA("Warning : failed to reflect variable @ %ud "
                                "for constant buffer %s",
                                var_idx, cb_info.cbuff_desc.Name);
                continue;
            }

            D3D11_SHADER_VARIABLE_DESC  var_description;
            HRESULT                     ret_code;
            CHECK_D3D(&ret_code, var_reflect->GetDesc(&var_description));
            if (FAILED(ret_code)) {
                continue;
            }

            //
            // Each global variable is represented by an object of type
            // shader_uniform_t.
            uniforms.push_back(
                shader_uniform_t(var_description, uniform_blocks.back())
                );
        }
    }
    
    /*
    for (v8_uint32_t i = 0; i < shader_description.ConstantBuffers; ++i) {
        reflect_constant_buffer(device, 
                                scoped_pointer_get(shader_reflector), i);
    }
    */

    //
    // Reflect any other resources the shader uses (samplers, textures).
    for (v8_uint32_t i = 0; i < shader_description.BoundResources; ++i) {
        reflect_bound_resource(scoped_pointer_get(shader_reflector), i);
    }

    //
    // Obtain input parameter signature for the shader.
    get_input_signature(device,
                        v8::base::scoped_pointer_get(shader_reflector),
                        compiled_bytecode, shader_description.InputParameters);

    //
    // 
    // Sort all resources by name, so we can binary search them.
    std::sort(
        std::begin(uniform_blocks),
        std::end(uniform_blocks),
        [](const shader_uniform_block_t* left, const shader_uniform_block_t* right) {
            return left->name_ < right->name_;
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
