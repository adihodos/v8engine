#include "v8/base/debug_helpers.hpp"
#include "v8/base/fixed_pod_vector.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/input_layout.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"

namespace {

D3D11_INPUT_ELEMENT_DESC input_layout_type_to_d3d_layout_type(
    const v8::directx::input_element_descriptor& desc 
    ) {
    D3D11_INPUT_ELEMENT_DESC ret_val;

    ret_val.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    ret_val.InputSlot = desc.slot_num;
    ret_val.InstanceDataStepRate = 0;
    ret_val.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    ret_val.SemanticIndex = 0;
    ret_val.SemanticName = desc.semantic;
    ret_val.Format = v8::directx::element_type_to_dxgi_format(
        desc.type, desc.count);

    return ret_val;
}

} // anonymous namespace

v8::directx::input_layout::input_layout() {}

v8::directx::input_layout::~input_layout() {}

v8_bool_t v8::directx::input_layout::initialize(
    const input_element_descriptor* descriptors, 
    const v8_uint32_t num_descriptors,
    renderer* rsys,
    const void* shader_bytecode,
    const v8_uint32_t bytecode_size
    ) {
    if (check_if_valid()) {
        return true;
    }

    v8::base::fixed_pod_vector<D3D11_INPUT_ELEMENT_DESC, 16u> layout_dcl;
    for (v8_uint32_t idx = 0; idx < num_descriptors; ++idx) {
        const D3D11_INPUT_ELEMENT_DESC e_desc = input_layout_type_to_d3d_layout_type(
            descriptors[idx]);
        if (e_desc.Format == DXGI_FORMAT_UNKNOWN) {
            OUTPUT_DBG_MSGA("Unknow format for element %d", idx);
            return false;
        }

        layout_dcl.push_back(e_desc);
    }

    HRESULT ret_code;
    ID3D11Device* graphics_device = rsys->internal_np_get_device();
    ID3D11InputLayout* new_layout = nullptr;
    CHECK_D3D(&ret_code, graphics_device->CreateInputLayout(layout_dcl.data(), 
                                                            (UINT) layout_dcl.size(), 
                                                            shader_bytecode, 
                                                            bytecode_size,
                                                            &new_layout
                                                            ));
    handle_ = new_layout;
    return handle_ != nullptr;
}
