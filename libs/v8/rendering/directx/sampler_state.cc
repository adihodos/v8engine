#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/sampler_state.hpp"

namespace {

const D3D11_FILTER kFilterMappings[] = {
    D3D11_FILTER_MIN_MAG_MIP_POINT,
    D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,
    D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,
    D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
    D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
    D3D11_FILTER_MIN_MAG_MIP_LINEAR,
    D3D11_FILTER_ANISOTROPIC
};

const D3D11_TEXTURE_ADDRESS_MODE kAddressModeMappings[] = {
    D3D11_TEXTURE_ADDRESS_WRAP,
    D3D11_TEXTURE_ADDRESS_MIRROR,
    D3D11_TEXTURE_ADDRESS_CLAMP,
    D3D11_TEXTURE_ADDRESS_BORDER,
    D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

const D3D11_COMPARISON_FUNC kComparisonFuncMapping[] = {
    D3D11_COMPARISON_NEVER,
    D3D11_COMPARISON_LESS,
    D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_LESS_EQUAL,
    D3D11_COMPARISON_GREATER,
    D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL,
    D3D11_COMPARISON_ALWAYS
};

} // anonymous state

v8::directx::samplerstate_traits::handle_type*
v8::directx::samplerstate_traits::create_state(
    const samplerDescriptor_t& descriptor, renderer* rsys
    ) {
    D3D11_SAMPLER_DESC sampler_desc;
    sampler_desc.Filter = kFilterMappings[descriptor.filter_type];
    sampler_desc.AddressU = kAddressModeMappings[descriptor.tex_address_u];
    sampler_desc.AddressV = kAddressModeMappings[descriptor.tex_address_v];
    sampler_desc.AddressW = kAddressModeMappings[descriptor.tex_address_w];
    sampler_desc.MipLODBias = descriptor.mip_lod_bias;
    sampler_desc.MaxAnisotropy = descriptor.max_aniso_level;
    sampler_desc.ComparisonFunc = kComparisonFuncMapping[descriptor.compare_func];

    memcpy(sampler_desc.BorderColor, descriptor.border_color.Elements,
            sizeof(sampler_desc.BorderColor));

    sampler_desc.MinLOD = descriptor.min_lod;
    sampler_desc.MaxLOD = descriptor.max_lod;

    HRESULT ret_code;
    handle_type* p_smp_state = nullptr;
    ID3D11Device* device = rsys->internal_np_get_device();
    CHECK_D3D(
        &ret_code,
        device->CreateSamplerState(&sampler_desc, &p_smp_state)
        );
    return p_smp_state;
}
