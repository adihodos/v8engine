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

v8_bool_t 
v8::directx::renderer::initialize(const v8::rendering::renderOptions_t& options
    ) {
    if (check_if_object_state_valid()) {
        return true;
    }

    //
    // TODO : add support for :
    //      - multiple render targets
    //      - antialiasing

    m_target_window = static_cast<HWND>(options.OutputWindow);
    m_target_width  = static_cast<float>(options.Width);
    m_target_height = static_cast<float>(options.Height);
    m_clear_color   = options.ClearColor;

    //
    // Translate and validate requested backbuffer format.
    m_backbuffer_type = element_type_to_dxgi_format(
        options.BufferElementType, options.BufferElementCount
        );

    if (m_backbuffer_type == DXGI_FORMAT_UNKNOWN) {
        OUTPUT_DBG_MSGA("Invalid format requeste for the backbuffer");
        return false;
    }

    //
    // TODO : fix this
    m_depth_stencil_type = DXGI_FORMAT_D24_UNORM_S8_UINT;
    
    if (!initialize_swap_chain(options)) {
        return false;
    }
    
    disable_auto_alt_enter();

    if (!initialize_font_engine()) {
        return false;
    }

    return handle_render_target_resized(m_target_width, m_target_height);
}

v8_bool_t 
v8::directx::renderer::handle_render_target_resized(float   new_width, 
                                                    float   new_height) 
{
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
        m_swap_chain->ResizeBuffers(1, 
                                    static_cast<UINT>(m_target_width), 
                                    static_cast<UINT>(m_target_height), 
                                    m_backbuffer_type, 
                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
    if (FAILED(ret_code)) {
        return false;
    }
    
    scoped_ptr<ID3D11Texture2D, com_storage> backbuffer_texture;
    CHECK_D3D(
        &ret_code,
        m_swap_chain->GetBuffer(0, 
                                __uuidof(ID3D11Texture2D), 
                                reinterpret_cast<void**>(raw_ptr_ptr(backbuffer_texture))));

    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGW(L"Failed to get backbuffer reference.");
        return false;
    }

    CHECK_D3D(
        &ret_code,
        m_device->CreateRenderTargetView(raw_ptr(backbuffer_texture),
                                         nullptr, 
                                         raw_ptr_ptr(m_rendertarget_view)));

    if (FAILED(ret_code)) {
        return false;
    }

    m_rtvs[0] = raw_ptr(m_rendertarget_view);

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
        m_device->CreateTexture2D(&depth_stencil_tex_desc, 
                                  nullptr,
                                  raw_ptr_ptr(m_depth_stencil)));

    if (FAILED(ret_code)) {
        return false;
    }

    CHECK_D3D(
        &ret_code,
        m_device->CreateDepthStencilView(raw_ptr(m_depth_stencil), 
                                         nullptr, 
                                         raw_ptr_ptr(m_depthstencil_view)));

    if (FAILED(ret_code)) {
        return false;
    }

    m_device_context->OMSetRenderTargets(static_cast<UINT>(m_rtvs.size()), 
                                         &m_rtvs[0], 
                                         raw_ptr(m_depthstencil_view));

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

void 
v8::directx::renderer::
ia_stage_set_primitive_topology_type(PrimitiveTopology::Type topology) const 
{
    assert(check_if_object_state_valid());

    assert((static_cast<v8_size_t>(topology) 
           < dimension_of(internal::kTopologyMappings))
           && "Unknown constant for topology type");

    m_device_context->IASetPrimitiveTopology(internal::kTopologyMappings[topology]);
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

v8_bool_t 
v8::directx::renderer::initialize_swap_chain(
    const v8::rendering::renderOptions_t& options
    ) {
    const UINT create_device_flag   = D3D11_CREATE_DEVICE_DEBUG;
    const D3D_DRIVER_TYPE driver_type = options.UseHardwareAcceleration ?
        D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_WARP;

    D3D_FEATURE_LEVEL feat_level;
    HRESULT ret_code;

    using namespace v8::base;

    CHECK_D3D(
        &ret_code,
        D3D11CreateDevice(nullptr, 
                          driver_type, 
                          nullptr,
                          create_device_flag, 
                          nullptr, 
                          0, 
                          D3D11_SDK_VERSION,
                          raw_ptr_ptr(m_device),
                          &feat_level,
                          raw_ptr_ptr(m_device_context)));
    
    if (FAILED(ret_code)) {
        return false;
    }

    //if (feat_level != D3D_FEATURE_LEVEL_11_0) {
    //    return false;
    //}

    DXGI_SWAP_CHAIN_DESC swap_chain_info;
    swap_chain_info.BufferDesc.Width = static_cast<UINT>(m_target_width);
    swap_chain_info.BufferDesc.Height = static_cast<UINT>(m_target_height);
    swap_chain_info.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_info.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_info.BufferDesc.Format = m_backbuffer_type;
    swap_chain_info.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_info.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_info.SampleDesc.Count = 1;
    swap_chain_info.SampleDesc.Quality = 0;
    swap_chain_info.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_info.BufferCount = 1;
    swap_chain_info.OutputWindow = m_target_window;
    swap_chain_info.Windowed = !options.FullScreen;
    swap_chain_info.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_info.Flags = 0;

    com_exclusive_pointer<IDXGIDevice>::type dxgi_device;

    CHECK_D3D(&ret_code, m_device->QueryInterface(
        __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_device)));

    if (FAILED(ret_code)) {
        return false;
    }

    com_exclusive_pointer<IDXGIAdapter>::type dxgi_adapter;

    CHECK_D3D(&ret_code, dxgi_device->GetParent(
        __uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgi_adapter)));

    if (FAILED(ret_code)) {
        return false;
    }

    com_exclusive_pointer<IDXGIFactory>::type dxgi_factory;

    CHECK_D3D(&ret_code, dxgi_adapter->GetParent(
        __uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgi_factory)));

    if (FAILED(ret_code)) {
        return false;
    }

    CHECK_D3D(
        &ret_code, 
        dxgi_factory->CreateSwapChain(raw_ptr(m_device), 
                                      &swap_chain_info, 
                                      raw_ptr_ptr(m_swap_chain)));

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
