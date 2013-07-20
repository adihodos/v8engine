#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/constants.hpp"
#include "v8/rendering/directx/internal/constants_mappings.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/texture.hpp"

#include "v8/rendering/directx/texture_unordered_access_binding.hpp"

namespace {

v8::base::com_exclusive_pointer<ID3D11UnorderedAccessView>::type
make_uav(ID3D11Resource*                            resource,
         ID3D11Device*                              device,
         const D3D11_UNORDERED_ACCESS_VIEW_DESC&    uav_data) 
{
    using namespace v8::base;

    com_exclusive_pointer<ID3D11UnorderedAccessView>::type tex_uav;
    HRESULT ret_code;

    CHECK_D3D(&ret_code,
              device->CreateUnorderedAccessView(resource,
                                                &uav_data,
                                                raw_ptr_ptr(tex_uav)));

    return tex_uav;
}

} // anonymous namespace

v8_bool_t 
v8::directx::texture_uav_binding::initialize(
    const texture&        bound_texture,
    const renderer&       rsys,
    const v8_uint32_t     mip_slice,
    const v8_uint32_t     first_array_slice)
{
    if (binding_ptr_) {
        return true;
    }

    using namespace v8::base;

    texture_ = &bound_texture;

    D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
    uav_desc.Format = static_cast<DXGI_FORMAT>(bound_texture.format());

    if (texture_->type() == rendering::textureType_t::Tex1D) {
        //
        // TODO : handle this case
    } else if (texture_->type() == rendering::textureType_t::Tex2D) {

        if (texture_->is_array()) {

            uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
            uav_desc.Texture2DArray.MipSlice = mip_slice;
            uav_desc.Texture2DArray.FirstArraySlice = first_array_slice;
            uav_desc.Texture2DArray.ArraySize = bound_texture.array_size();

        } else {

            uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
            uav_desc.Texture2D.MipSlice = mip_slice;
        }

        binding_ptr_ = make_uav(texture_->handle(),
                                rsys.internal_np_get_device(),
                                uav_desc);

    } else if (texture_->type() == rendering::textureType_t::Tex3D) {
        //
        // TODO : handle this case
    } else {
        //
        // TODO : handle this case
    }

    return binding_ptr_ != nullptr;

}
