#include <v8/base/debug_helpers.hpp>
#include <v8/base/count_of.hpp>

#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/constants_mappings.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/event/window_event.hpp"

#include "v8/rendering/directx/renderer.hpp"

v8::directx::renderer::renderer() 
    :       m_target_window(nullptr)
        ,   m_clear_color(v8::math::color_rgb::C_LightSeaGreen)
        ,   m_target_width(1024.0f)
        ,   m_target_height(1024.0f)
        ,   m_fullscreen(false)
        ,   m_backbuffer_type(DXGI_FORMAT_R8G8B8A8_UNORM)
        ,   m_depth_stencil_type(DXGI_FORMAT_D24_UNORM_S8_UINT) {
    m_rtvs.push_back(nullptr);
}

v8::directx::renderer::~renderer() {}

void v8::directx::renderer::on_viewport_resized(
    const resize_event& ev_resize
    ) {
    assert(ev_resize.width);
    assert(ev_resize.height);

    if (is_initialized()) { 
        handle_render_target_resized(static_cast<float>(ev_resize.width),
                                     static_cast<float>(ev_resize.height));
    }
}

v8_bool_t v8::directx::renderer::initialize(
    const v8::directx::render_init_params& init_params
    ) {
    if (check_if_object_state_valid()) {
        return true;
    }

    //
    // TODO : add support for :
    //      - multiple render targets
    //      - antialiasing

    m_target_window = static_cast<HWND>(init_params.target_window);
    m_target_width = static_cast<float>(init_params.width);
    m_target_height = static_cast<float>(init_params.height);

    //
    // Translate and validate requested backbuffer format.
    m_backbuffer_type = element_type_to_dxgi_format(
        init_params.buffer_element_type, init_params.buffer_element_count
        );
    if (m_backbuffer_type == DXGI_FORMAT_UNKNOWN) {
        OUTPUT_DBG_MSGA("Invalid format requeste for the backbuffer");
        return false;
    }

    m_depth_stencil_type = DXGI_FORMAT_D24_UNORM_S8_UINT;
    
    if (!initialize_swap_chain(init_params.full_screen)) {
        return false;
    }
    if (!init_params.handle_full_screen) {
        disable_auto_alt_enter();
    }
    if (!initialize_font_engine()) {
        return false;
    }
    return handle_render_target_resized(m_target_width, m_target_height);
}

v8_bool_t v8::directx::renderer::handle_render_target_resized(
    float new_width, float new_height
    ) {
    assert(m_device);
    assert(m_device_context);
    assert(m_swap_chain);

    m_target_width = new_width;
    m_target_height = new_height;

    using namespace v8::base;
    m_device_context->OMSetRenderTargets(0, nullptr, nullptr);

    m_rendertarget_view = nullptr;
    m_depthstencil_view = nullptr;
    m_depth_stencil = nullptr;

    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        m_swap_chain->ResizeBuffers(1, static_cast<UINT>(m_target_width), 
                                    static_cast<UINT>(m_target_height), 
                                    m_backbuffer_type, 
                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
    if (FAILED(ret_code)) {
        return false;
    }
    
    scoped_ptr<ID3D11Texture2D, com_storage> backbuffer_texture;
    CHECK_D3D(
        &ret_code,
        m_swap_chain->GetBuffer(
            0, __uuidof(ID3D11Texture2D), 
            reinterpret_cast<void**>(scoped_pointer_get_impl(backbuffer_texture))));
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to get backbuffer reference.");
        return false;
    }

    CHECK_D3D(
        &ret_code,
        m_device->CreateRenderTargetView(
            scoped_pointer_get(backbuffer_texture),
            nullptr, scoped_pointer_get_impl(m_rendertarget_view)));

    if (FAILED(ret_code)) {
        return false;
    }
    m_rtvs[0] = scoped_pointer_get(m_rendertarget_view);

    D3D11_TEXTURE2D_DESC depth_stencil_tex_desc = {
        static_cast<UINT>(m_target_width),
        static_cast<UINT>(m_target_height),
        1,
        1,
        m_depth_stencil_type,
        { 1, 0 },
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_DEPTH_STENCIL,
        0,
        0
    };

    CHECK_D3D(
        &ret_code,
        m_device->CreateTexture2D(&depth_stencil_tex_desc, nullptr,
                                  scoped_pointer_get_impl(m_depth_stencil)));
    if (FAILED(ret_code)) {
        return false;
    }

    CHECK_D3D(
        &ret_code,
        m_device->CreateDepthStencilView(
            scoped_pointer_get(m_depth_stencil), nullptr, 
            scoped_pointer_get_impl(m_depthstencil_view)));
    if (FAILED(ret_code)) {
        return false;
    }
    m_device_context->OMSetRenderTargets(
        static_cast<UINT>(m_rtvs.size()), 
        &m_rtvs[0], scoped_pointer_get(m_depthstencil_view)
        );

    D3D11_VIEWPORT viewport_data = {
        0.0f,
        0.0f,
        m_target_width,
        m_target_height,
        0.0f,
        1.0f
    };
    m_device_context->RSSetViewports(1, &viewport_data);

    return true;
}

//void v8::directx::renderer::ia_stage_set_vertex_buffers(
//    const VertexBuffer_t* vx_buffers, 
//    v8_uint32_t buffer_count
//    ) const {
//    assert(check_if_object_state_valid());
//
//    std::vector<ID3D11Buffer*> buffer_list;
//    std::vector<uint32_t> strides;
//    std::vector<uint32_t> offsets;
//
//    buffer_list.reserve(buffer_count);
//    strides.reserve(buffer_count);
//    offsets.reserve(buffer_count);
//
//    for (v8_uint32_t i = 0; i < buffer_count; ++i) {
//        buffer_list.push_back(vx_buffers[i].internal_np_get_handle());
//        strides.push_back(vx_buffers[i].get_element_size());
//        offsets.push_back(0);
//    }
//
//    m_device_context->IASetVertexBuffers(
//        0, static_cast<UINT>(buffer_list.size()), 
//        &buffer_list[0], &strides[0], &offsets[0]
//    );
//}
//
//void v8::directx::renderer::ia_stage_set_index_buffer(
//    const IndexBuffer_t* ibuff, 
//    v8_uint_t offset /*= 0*/
//    ) const {
//
//    assert(check_if_object_state_valid());
//
//    const DXGI_FORMAT kIndexMapping[] = { 
//        DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R32_UINT
//    };
//
//    assert((ibuff->get_element_size() == sizeof(v8_uint16_t)) 
//           || (ibuff->get_element_size() == sizeof(v8_uint32_t)));
//
//    v8_bool_t mapping_type = ibuff->get_element_size() == sizeof(v8_uint32_t);
//
//    m_device_context->IASetIndexBuffer(
//        ibuff->internal_np_get_handle(),
//        kIndexMapping[mapping_type],
//        offset
//        );
//}

void v8::directx::renderer::ia_stage_set_primitive_topology_type(
    PrimitiveTopology::Type topology
    ) const {
    assert(check_if_object_state_valid());

    assert((static_cast<v8_size_t>(topology) 
           < dimension_of(internal::kTopologyMappings))
           && "Unknown constant for topology type");

    m_device_context->IASetPrimitiveTopology(
        internal::kTopologyMappings[topology]
    );
}


void v8::directx::renderer::draw_string(
    const wchar_t* text, 
    float font_size, 
    float xpos, 
    float ypos, 
    const v8::math::color_rgb& color
    ) {
    assert(check_if_object_state_valid());
    m_font_wrapper->DrawString(v8::base::scoped_pointer_get(m_device_context), 
                              text, font_size, xpos, ypos, 
                              color.to_uint32_abgr(), FW1_RESTORESTATE);
}

void v8::directx::renderer::draw_string(
    const char* text,
    float font_size, 
    float xpos, 
    float ypos, 
    const v8::math::color_rgb& color
    ) {

    std::vector<wchar_t> utf8_string;
    auto char_count = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (char_count) {
        utf8_string.resize((std::vector<wchar_t>::size_type) char_count + 1);
        char_count = MultiByteToWideChar(CP_UTF8, 0, text, -1, &utf8_string[0], 
                                         (int) char_count + 1);
        if (char_count) {
            draw_string(&utf8_string[0], font_size, xpos, ypos, color);
        }
    }
}

v8_bool_t v8::directx::renderer::disable_auto_alt_enter() const {
    assert(m_swap_chain);

    using namespace v8::base;

    scoped_ptr<IDXGIFactory, com_storage> factoryPtr;
    HRESULT ret_code = m_swap_chain->GetParent(
        __uuidof(IDXGIFactory), 
        reinterpret_cast<void**>(scoped_pointer_get_impl(factoryPtr))
        );
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to query for IDXGIFactory.");
        return false;
    }

    ret_code = factoryPtr->MakeWindowAssociation(
        m_target_window, DXGI_MWA_NO_WINDOW_CHANGES
        );
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to make window association");
    }
    return ret_code == S_OK;
}

v8_bool_t v8::directx::renderer::initialize_swap_chain(
    v8_bool_t fullscreen
    ) {
    using namespace v8::base;

    scoped_ptr<IDXGIFactory, com_storage> dxgifactory;
    HRESULT ret_code = ::CreateDXGIFactory(
        __uuidof(IDXGIFactory), 
        reinterpret_cast<void**>(scoped_pointer_get_impl(dxgifactory))
        );
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to create IDXGIFactory, %#08x", ret_code);
        return false;
    }

    scoped_ptr<IDXGIAdapter, com_storage> firstAdapter;
    ret_code = dxgifactory->EnumAdapters(
        0, scoped_pointer_get_impl(firstAdapter)
        );
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to enum adapters, %#08x", ret_code);
        return false;
    }

    ret_code = S_OK;
    DXGI_MODE_DESC closestMode;
    memset(&closestMode, 0, sizeof(closestMode));
    for (unsigned int outputIdx = 0; ; ++outputIdx) {
        scoped_ptr<IDXGIOutput, com_storage> adapterOutput;
        ret_code = firstAdapter->EnumOutputs(
            outputIdx, scoped_pointer_get_impl(adapterOutput)
            );

        if (FAILED(ret_code)) {
            break;
        }
        DXGI_MODE_DESC requestedMode;
        requestedMode.Format = static_cast<DXGI_FORMAT>(m_backbuffer_type);
        requestedMode.Width = static_cast<UINT>(m_target_width);
        requestedMode.Height = static_cast<UINT>(m_target_height);
        requestedMode.RefreshRate.Numerator = 60;
        requestedMode.RefreshRate.Denominator = 1;
        requestedMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        requestedMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

        ret_code = adapterOutput->FindClosestMatchingMode(
            &requestedMode, &closestMode, nullptr);
        if (ret_code == S_OK) {
            break;
        }
    }

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"No suitable mode found for %3f %3f "
                        L"DXGI_FORMAT_R8G8B8U8A8_UNORM", 
                        m_target_width, m_target_height);
        return false;
    }

    DXGI_SWAP_CHAIN_DESC swap_chain_description;
    swap_chain_description.BufferCount = 1;
    swap_chain_description.BufferDesc = closestMode;
    swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swap_chain_description.OutputWindow = m_target_window;
    swap_chain_description.SampleDesc.Count = 1;
    swap_chain_description.SampleDesc.Quality = 0;
    swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_description.Windowed = !fullscreen;

    const UINT device_creation_flags = D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL supported_feat_level = D3D_FEATURE_LEVEL_11_0;

    CHECK_D3D(
        &ret_code,
        D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, device_creation_flags, 
            nullptr, 0, D3D11_SDK_VERSION, &swap_chain_description,
            scoped_pointer_get_impl(m_swap_chain), 
            scoped_pointer_get_impl(m_device),
            &supported_feat_level, 
            scoped_pointer_get_impl(m_device_context)));

    return ret_code == S_OK;
}

v8_bool_t v8::directx::renderer::initialize_font_engine() {
    assert(m_device);
    assert(m_device_context);

    using namespace v8::base;
    HRESULT ret_code = ::FW1CreateFactory(
        FW1_VERSION, scoped_pointer_get_impl(m_font_factory));

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to create font factory, error %#08x", ret_code);
        return false;
    }

    ret_code = m_font_factory->CreateFontWrapper(
        scoped_pointer_get(m_device), L"Arial", 
        scoped_pointer_get_impl(m_font_wrapper));

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to create font, error %#08x", ret_code);
        return false;
    }

    return true;
}

void v8::directx::renderer::add_render_target(
    ID3D11RenderTargetView* rtv
    ) {
    assert(m_rtvs.size() < 8U);
    m_rtvs.push_back(rtv);
}

void v8::directx::renderer::set_render_target(
    ID3D11RenderTargetView* rtv, 
    v8_size_t slot_id
    ) {
    assert(slot_id < m_rtvs.size());
    m_rtvs[slot_id] = rtv;
}

void v8::directx::renderer::get_render_target(
    v8_size_t slot_id,
    ID3D11RenderTargetView** rtv
    ) {

    assert(slot_id < m_rtvs.size());
    assert(rtv);

    *rtv = m_rtvs[slot_id];
}