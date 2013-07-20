#include <d3d11.h>
#include <algorithm>
#include <v8/v8.hpp>
#include <v8/base/auto_buffer.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/sequence_container_veneer.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/count_of.hpp>
#include <v8/base/string_util.hpp>
#include <v8/base/shims/raw_ptr.hpp>
#include <v8/event/window_event.hpp>
#include <v8/event/input_event.hpp>
#include <v8/input/key_syms.hpp>
#include <v8/gui/basic_window.hpp>
#include <v8/io/filesystem.hpp>
#include <v8/rendering/constants.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/rendering/texture.hpp>
#include <v8/utility/win_util.hpp>
#include <v8/math/camera.hpp>
#include <v8/math/color.hpp>
#include <v8/graphis_api_traits.hpp>
#include <v8/math/light.hpp>
#include <v8/math/math_constants.hpp>
#include <v8/math/projection.hpp>
#include <v8/scene/null_camera_controller.hpp>
#include <v8/scene/cam_controller_spherical_coordinates.hpp>
#include <v8/fast_delegate/fast_delegate.hpp>
#include <v8/utility/win_util.hpp>
#include <third_party/stlsoft/stlsoft/string/static_string.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/directx_tex/DirectXTex.h>

#include <v8/rendering/vertex_p.hpp>

v8_bool_t
tex_initialize(const v8::rendering::texture_info_t&      tex_info,
               v8::directx::renderer*                    rsys) 
{
    platformstl::memory_mapped_file mmfile_texture(tex_info.tex_filename.c_str());

    platformstl::path texture_file_path(tex_info.tex_filename.c_str());

    v8::utility::win32::scoped_wide_string_t path_widestr(
        v8::utility::win32::multibyte_string_to_wide_string(tex_info.tex_filename.c_str()));

    HRESULT ret_code;
    DirectX::TexMetadata tex_metadata;

    if (!strcmp(texture_file_path.get_ext(), "dds")) {
        ret_code = DirectX::GetMetadataFromDDSMemory(mmfile_texture.memory(),
                                                     mmfile_texture.size(),
                                                     0,
                                                     tex_metadata);
    } else {
        ret_code = DirectX::GetMetadataFromWICMemory(mmfile_texture.memory(),
                                                     mmfile_texture.size(),
                                                     0,
                                                     tex_metadata);
    }

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to get texture metadata, %#08x", ret_code);
        return false;
    }

    //v8_uint32_t width_ = tex_metadata.width;
    //v8height_ = tex_metadata.height;
    v8_uint32_t depth_ = tex_metadata.depth;
    //array_size_ = tex_metadata.arraySize;
    //format_ = tex_metadata.format;
    typedef v8::base::com_exclusive_pointer<ID3D11Resource>::type rst_t;
    rst_t resource_;
    typedef v8::base::com_exclusive_pointer<ID3D11ShaderResourceView>::type srv_t; 
    srv_t tex_srv_;

    using namespace v8::rendering;

    UINT bind_flags = 0;
    if (tex_info.tex_bindflags & BindingFlag::ShaderResource) {
        bind_flags |= D3D11_BIND_SHADER_RESOURCE;
    }

    if (tex_info.tex_bindflags & BindingFlag::RenderTarget) {
        bind_flags |= D3D11_BIND_RENDER_TARGET;
    }

    if (tex_info.tex_bindflags & BindingFlag::UnorderedAccess) {
        bind_flags |= D3D11_BIND_UNORDERED_ACCESS;
    }

    if (depth_ == 0) {

    } else if (depth_ == 1) {

        D3D11_TEXTURE2D_DESC tex_desc = {
            tex_metadata.width,
            tex_metadata.height,
            tex_metadata.mipLevels,
            tex_metadata.arraySize,
            tex_metadata.format,
            { 1, 0 },
            D3D11_USAGE_IMMUTABLE,
            bind_flags,
            0,
            0
        };

        size_t row_pitch;
        size_t slice_pitch;

        DirectX::ComputePitch(tex_metadata.format, 
                              tex_metadata.width,
                              tex_metadata.height,
                              row_pitch,
                              slice_pitch,
                              DirectX::CP_FLAGS_NONE);

        D3D11_SUBRESOURCE_DATA init_data = {
            mmfile_texture.memory(),
            row_pitch,
            slice_pitch
        };

        ret_code = 
            rsys->internal_np_get_device()->CreateTexture2D(
                &tex_desc,
                &init_data,
                (ID3D11Texture2D**) v8::base::scoped_pointer_get_impl(resource_));

    } else if (depth_ == 2) {
    } else {
        return false;
    }

    if (FAILED(ret_code)) {
        return false;
    }

    if (tex_info.tex_bindflags & BindingFlag::ShaderResource) {
        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
        srv_desc.Format = tex_metadata.format;

        if (depth_ == 2) {
            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MipLevels = tex_metadata.mipLevels;
            srv_desc.Texture2D.MostDetailedMip = 0;

                ret_code =
                rsys->internal_np_get_device()->CreateShaderResourceView(
                    v8::base::scoped_pointer_get(resource_),
                    &srv_desc,
                    v8::base::scoped_pointer_get_impl(tex_srv_));

        } else {
            return false;
        }

        if (FAILED(ret_code)) {
            return false;
        }

        return true;
    }

    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    CoInitialize(nullptr);
    
    v8::gui::basic_window window;
    v8::gui::window_init_params_t wnd_params = { 1024, 1024, "YTralala" };

    if (!window.initialize(wnd_params)) {
        return -1;
    }

    v8::rendering::renderer rsys;

    v8::rendering::render_init_params graphics_init_params;
    graphics_init_params.target_window        = window.get_handle();
    graphics_init_params.antialiasing         = false;
    graphics_init_params.buffer_element_type  = 
        v8::rendering::ElementType::Unorm8;
    graphics_init_params.buffer_element_count = 4;
    graphics_init_params.depth_bits           = 24;
    graphics_init_params.stencil_bits         = 8;
    graphics_init_params.full_screen          = false;
    graphics_init_params.handle_full_screen   = true;
    graphics_init_params.render_targets_count = 1;
    graphics_init_params.width                = 1024;
    graphics_init_params.height               = 1024;
    graphics_init_params.clear_color          = v8::math::color_rgb::C_Darkorange;

    if (!rsys.initialize(graphics_init_params)) {
        return 1;
    }

    v8::rendering::texture_info_t tinfo("D:\\games\\basic_drawing\\data\\textures\\f4_texture.jpg", v8::rendering::BindingFlag::ShaderResource);
    tex_initialize(tinfo, &rsys);

    //
    // Rendering system must receive window resize events for proper viewport 
    // setup.
    window.Subscribers_ResizeEvent += fastdelegate::MakeDelegate(
        &rsys, 
        &v8::rendering::renderer::on_viewport_resized
        );

    window.message_loop();

    return 0;
}

