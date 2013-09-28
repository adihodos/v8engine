#include "v8/rendering/render_init_params.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/buffer_swapper.hpp"

v8_bool_t 
v8::directx::buffer_swapper::initialize(
    v8::directx::renderer const*            render_sys,
    v8::rendering::renderOptions_t const&   opts) NOEXCEPT
{
    using namespace v8::base;

    DXGI_SWAP_CHAIN_DESC swap_chain_info;
    swap_chain_info.BufferDesc.Width = opts.Width;// static_cast<UINT>(m_target_width);
    swap_chain_info.BufferDesc.Height = opts.Height; //static_cast<UINT>(m_target_height);
    swap_chain_info.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_info.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_info.BufferDesc.Format = render_sys->rendertarget_format();
    swap_chain_info.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swap_chain_info.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swap_chain_info.SampleDesc.Count = 1;
    swap_chain_info.SampleDesc.Quality = 0;
    swap_chain_info.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_info.BufferCount = 1;
    swap_chain_info.OutputWindow = static_cast<v8::directx::renderer::native_window_type>(opts.OutputWindow);
    swap_chain_info.Windowed = !opts.FullScreen;
    swap_chain_info.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_info.Flags = 0;

    com_exclusive_pointer<IDXGIDevice>::type dxgi_device;
    HRESULT ret_code;

    CHECK_D3D(&ret_code, render_sys->internal_np_get_device()->QueryInterface(
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
        dxgi_factory->CreateSwapChain(render_sys->internal_np_get_device(), 
                                      &swap_chain_info, 
                                      raw_ptr_ptr(swap_chain_)));

    return ret_code == S_OK;
}

v8::directx::bufferSwapStatus_t 
v8::directx::buffer_swapper::swap_buffers(
    v8::directx::bufferSwapOptions_t const swap_opt) const NOEXCEPT 
{
    assert(is_valid());

    const UINT present_flag = swap_opt == bufferSwapOptions_t::Swap ? 0 : DXGI_PRESENT_TEST;

    const HRESULT result = swap_chain_->Present(0,present_flag);

    if (result == S_OK) {
        return bufferSwapStatus_t::Ok;
    }

    if (result == DXGI_STATUS_OCCLUDED) {
        return bufferSwapStatus_t::Occlusion;
    }

    return bufferSwapStatus_t::Error;
}
