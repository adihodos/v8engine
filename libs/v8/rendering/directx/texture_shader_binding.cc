#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/constants.hpp"
#include "v8/rendering/directx/internal/constants_mappings.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/texture.hpp"

#include "v8/rendering/directx/texture_shader_binding.hpp"

namespace {

v8::base::com_exclusive_pointer<ID3D11ShaderResourceView>::type
make_srv(ID3D11Resource*                            resource,
         ID3D11Device*                              device,
         const D3D11_SHADER_RESOURCE_VIEW_DESC&     srv_data) 
{
    using namespace v8::base;

    com_exclusive_pointer<ID3D11ShaderResourceView>::type tex_srv;
    HRESULT ret_code;

    CHECK_D3D(&ret_code,
              device->CreateShaderResourceView(resource,
                                               &srv_data,
                                               raw_ptr_ptr(tex_srv)));

    return tex_srv;
}

} // anonymous namespace

v8_bool_t
v8::directx::texture_shader_binding::
initialize(const texture&         bound_texture,
           const renderer&        rsys,
           const v8_uint32_t      most_detailed_mip) 
{
    if (binding_ptr_) {
        return true;
    }

    using namespace v8::base;

    texture_ = &bound_texture;

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    srv_desc.Format = static_cast<DXGI_FORMAT>(bound_texture.format());

    if (texture_->type() == rendering::textureType_t::Tex1D) {
        //
        // TODO : handle this case
    } else if (texture_->type() == rendering::textureType_t::Tex2D) {

        if (texture_->is_array()) {

            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            srv_desc.Texture2DArray.ArraySize = texture_->array_size();
            srv_desc.Texture2DArray.FirstArraySlice = 0;
            srv_desc.Texture2DArray.MipLevels = texture_->mip_levels();
            srv_desc.Texture2DArray.MostDetailedMip = most_detailed_mip;

        } else {

            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MipLevels = texture_->mip_levels();
            srv_desc.Texture2D.MostDetailedMip = most_detailed_mip;

        }

        binding_ptr_ = make_srv(texture_->handle(),
                                rsys.internal_np_get_device(),
                                srv_desc);

    } else if (texture_->type() == rendering::textureType_t::Tex3D) {
        //
        // TODO : handle this case
    } else {
        //
        // TODO : handle this case
    }

    return binding_ptr_ != nullptr;
}

v8_bool_t
v8::directx::texture_shader_binding::initialize(
    const char*                         texture_path,
    const v8::directx::renderer&        rsys)
{
    if (binding_ptr_) {
        return true;
    }

    texture tex(texture_path, rsys);
    if (!tex) {
        return false;
    }

    return initialize(tex, rsys);
}
