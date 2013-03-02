#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/fixed_pod_vector.hpp>

#include "v8/global_state.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/renderer.hpp"

#include "v8/rendering/directx/effect_pass.hpp"

void v8::directx::effect_pass::on_pass_loaded() {
    assert(is_valid());

    HRESULT ret_code;
    CHECK_D3D(&ret_code, pass_handle_->GetDesc(&pass_info_));

    if (FAILED(ret_code)) {
        pass_handle_ = nullptr;
        return;
    }

    //
    // No input signature present.
    if (!pass_info_.pIAInputSignature) {
        return;
    }

    D3DX11_PASS_SHADER_DESC shader_desc;
    CHECK_D3D(&ret_code, pass_handle_->GetVertexShaderDesc(&shader_desc));

    if (FAILED(ret_code)) {
        CHECK_D3D(&ret_code, pass_handle_->GetGeometryShaderDesc(&shader_desc));
        if (FAILED(ret_code)) {
            return;
        }
    }
    if (!shader_desc.pShaderVariable->IsValid()) {
        OUTPUT_DBG_MSGA("Shader variable is not valid!");
        return;
    }

    ID3DX11EffectShaderVariable*    k_shader = shader_desc.pShaderVariable;
    D3DX11_EFFECT_SHADER_DESC       shader_attributes;
    CHECK_D3D(&ret_code, k_shader->GetShaderDesc(shader_desc.ShaderIndex, 
                                                 &shader_attributes));
    if (FAILED(ret_code)) {
        return;
    }

    v8::base::fixed_pod_vector<D3D11_INPUT_ELEMENT_DESC, 16> input_element_attrs;

    for (v8_uint_t element_index = 0; 
         element_index < shader_attributes.NumInputSignatureEntries;
         ++element_index) {
 D3D11_SIGNATURE_PARAMETER_DESC input_param_info;
        CHECK_D3D(&ret_code, k_shader->GetInputSignatureElementDesc(
                                0, element_index, &input_param_info));
        if (FAILED(ret_code)) {
            return;
        }

        D3D11_INPUT_ELEMENT_DESC input_element;
        if (!input_param_description_to_element_description(input_param_info, &input_element)) {
            return;
        }        
        input_element_attrs.push_back(input_element);
    }

    ID3D11Device* rdev = state->render_sys()->internal_np_get_device();
    CHECK_D3D(&ret_code,
              rdev->CreateInputLayout(input_element_attrs.data(),
              static_cast<UINT>(input_element_attrs.size()), 
              shader_attributes.pBytecode, shader_attributes.BytecodeLength,
              v8::base::scoped_pointer_get_impl(input_layout_)));
}

void v8::directx::effect_pass::apply() {
    assert(is_valid());

    if (input_layout_) {
        state->render_sys()->internal_np_get_device_context()->IASetInputLayout(
            base::scoped_pointer_get(input_layout_));       
    }

    pass_handle_->Apply(0, state->render_sys()->internal_np_get_device_context());
}