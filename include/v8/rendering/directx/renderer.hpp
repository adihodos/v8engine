#pragma once

#include <vector>

#include <D3D11.h>
#include <third_party/fw1_fontwrapper/FW1FontWrapper.h>

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/shims/scoped_ptr.hpp>
#include <v8/math/color.hpp>
#include <v8/rendering/viewport.hpp>
#include <v8/rendering/directx/constants.hpp>
#include <v8/rendering/directx/depth_stencil_state.hpp>
#include <v8/rendering/directx/rasterizer_state.hpp>

namespace v8 {
    struct resize_event;

    namespace rendering {
        struct renderOptions_t;
    }
}

namespace v8 { namespace directx {

//!
//! \brief  Abstraction over the DirectX graphics API.
class renderer {

//! \name Defined types.
//! @{

public :

    typedef HWND                    native_window_type;
    typedef ID3D11Device*           native_device_type;
    typedef ID3D11DeviceContext*    native_device_context_type;

//! @}

//! \name Constructors.
//! @{

public :    

    renderer(); 

    ~renderer();

//! @}

//! \name Event delegates and event handling.
//! @{

public :

    void 
    on_viewport_resized(
        resize_event const& ev_resize);

private :

    v8_bool_t 
    handle_render_target_resized(
        float const     new_width, 
        float const     new_height);

//! @}

//! \name Attributes
//! @{

public :    

    //! \brief  Returns true if the renderer was initialized successfully.
    inline v8_bool_t 
    is_initialized() const NOEXCEPT;

    //! \brief  Test if the renderer is running in full screen mode.
    inline v8_bool_t 
    is_fullscreen() const NOEXCEPT;

    //! \brief  Returns a (platform dependent) handle to the output window.
    inline native_window_type 
    get_target_window() const NOEXCEPT;

    //! \brief  Returns the output window's width.
    inline float 
    get_target_width() const NOEXCEPT;

    //! \brief  Returns the output window's height.
    inline float 
    get_target_height() const NOEXCEPT;

    //! \brief  Returns the format of the render target.
    inline DXGI_FORMAT
    rendertarget_format() const NOEXCEPT;

    //! \brief  Sets the specified viewport as the renderer's viewport.
    inline void 
    set_viewport(
        const v8::rendering::viewPort_t&    viewport) NOEXCEPT;

//! @}

//! \name Initialisation
//! @{

public :

    //! \brief  Initializes the renderer, using the specified options.
    //! \returns True on success, false on failure.
    v8_bool_t 
    initialize(
        v8::rendering::renderOptions_t const&   options);

//! @}

//! \name Operations
//! @{

public :    

    inline FramePresentResult::Type 
    present_frame(
        FramePresent::Type const flags) NOEXCEPT;

    void 
    ia_stage_set_primitive_topology_type(
        PrimitiveTopology::Type const topology) const NOEXCEPT;

    inline void 
    ia_stage_set_input_layout(
        ID3D11InputLayout* input_layout) NOEXCEPT;

// ! @}

//! \name Rasterizer state
//! @{

    inline void 
    set_rasterizer_state(
        const rasterizer_state& rsstate) NOEXCEPT;

    inline void 
    reset_rasterizer_state() NOEXCEPT;

//! @}

//! \name Shader stage operations.
//! @{

public :    

    inline void 
    unbind_vertex_shader() const NOEXCEPT;

    inline void 
    unbind_geometry_shader() const NOEXCEPT;

    inline void 
    unbind_fragment_shader() const NOEXCEPT;

//! @}

//! \name Output merger stage operations.
//! @{

public :    

    inline void 
    set_depth_stencil_state(
        const depth_stencil_state&          dsstate, 
        const v8_uint32_t                   ref_value = 0) NOEXCEPT;

    void 
    add_render_target(
        ID3D11RenderTargetView* rtv);

    void 
    set_render_target(
        ID3D11RenderTargetView*         rtv, 
        const v8_size_t                 slot_id = 0);

    void 
    get_render_target(
        const v8_size_t             slot_id, 
        ID3D11RenderTargetView**    rtv);

//! @}

//! \name State manipulation.
//! @{

public :    

    inline void 
    set_clear_color(
        v8::math::rgb_color const& clear_color) NOEXCEPT;

    inline 
    v8_bool_t set_fullscreen(
        v8_bool_t const fullscreen) NOEXCEPT;

    inline void 
    clear_backbuffer() NOEXCEPT;

    inline void 
    clear_depth_stencil() NOEXCEPT;

    inline void 
    reset_raster_state() NOEXCEPT;

    inline void 
    reset_depth_stencil_state() NOEXCEPT;

    inline void 
    reset_blending_state() NOEXCEPT;

//! @}

//! \name Drawing operations
//! @{

public :    

    inline void 
    draw(
        const v8_uint_t       vertex_count, 
        const v8_uint_t       first_vertex_location = 0) const NOEXCEPT;

    inline void 
    draw_indexed(
        v8_uint_t   index_count, 
        v8_uint_t   first_index_location = 0,
        int         index_offset = 0) const NOEXCEPT;

    void 
    v8::directx::renderer::draw_string(
        wchar_t const*                  text, 
        float const                     font_size, 
        float const                     xpos, 
        float const                     ypos, 
        v8::math::rgb_color const&      color) NOEXCEPT;

    void
    v8::directx::renderer::draw_string(
        char const*                     text,
        float const                     font_size, 
        float const                     xpos, 
        float const                     ypos, 
        v8::math::rgb_color const&      color) NOEXCEPT;

//! @}

//! \name Internally reserved routines. Must not be called by client code.
//! @{

public :    

    inline ID3D11Device* 
    internal_np_get_device() const NOEXCEPT;

    inline ID3D11DeviceContext* 
    internal_np_get_device_context() const NOEXCEPT;

//! @}

//! \name Sanity checking.
//! @{

public :

    inline v8_bool_t 
    check_if_object_state_valid() const NOEXCEPT;

//! @}

//! \name Data members

private :
    v8::base::com_exclusive_pointer
    <
        ID3D11Device
    >::type                                                             m_device;

    v8::base::com_exclusive_pointer
    <
        ID3D11DeviceContext
    >::type                                                             m_device_context;

    v8::base::com_exclusive_pointer
    <
        IDXGISwapChain
    >::type                                                             m_swap_chain;

    v8::base::com_exclusive_pointer
    <
        ID3D11Texture2D
    >::type                                                             m_depth_stencil;

    v8::base::com_exclusive_pointer
    <
        ID3D11RenderTargetView
    >::type                                                             m_rendertarget_view;

    v8::base::com_exclusive_pointer
    <
        ID3D11DepthStencilView
    >::type                                                             m_depthstencil_view;

    v8::base::com_exclusive_pointer
    <
        IFW1Factory
    >::type                                                             m_font_factory;

    v8::base::com_exclusive_pointer
    <
        IFW1FontWrapper
    >::type                                                             m_font_wrapper;

    HWND                                                                m_target_window;

    v8::math::rgb_color                                                 m_clear_color;

    float                                                               m_target_width;

    float                                                               m_target_height;

    v8_bool_t                                                           m_fullscreen;

    DXGI_FORMAT                                                         m_backbuffer_type;

    DXGI_FORMAT                                                         m_depth_stencil_type;

    //
    // Bound render targets.
    std::vector<ID3D11RenderTargetView*>                                m_rtvs;

//! @}

//! \name Disabled functions.
//! @{

public :

    NO_CC_ASSIGN(renderer);

//! @}
};

inline v8_bool_t 
renderer::is_initialized() const NOEXCEPT {
    return m_swap_chain && m_device && m_device_context;
}

inline v8_bool_t 
renderer::is_fullscreen() const NOEXCEPT {
    return m_fullscreen;
}

inline renderer::native_window_type 
renderer::get_target_window() const NOEXCEPT {
    return m_target_window;
}

inline float 
renderer::get_target_width() const NOEXCEPT {
    return m_target_width;
}

inline float 
renderer::get_target_height() const NOEXCEPT {
    return m_target_height;
}

inline DXGI_FORMAT
renderer::rendertarget_format() const NOEXCEPT {
    return m_backbuffer_type;
}

inline void 
renderer::set_viewport(
    const v8::rendering::viewPort_t&    viewport) NOEXCEPT 
{
    assert(is_initialized());
    m_device_context->RSSetViewports(1, (D3D11_VIEWPORT*) &viewport);
}

inline void 
renderer::ia_stage_set_input_layout(
    ID3D11InputLayout* input_layout) NOEXCEPT 
{
    assert(input_layout);
    assert(check_if_object_state_valid());
    m_device_context->IASetInputLayout(input_layout);
}

inline void 
renderer::set_clear_color(
    v8::math::rgb_color const& clear_color) NOEXCEPT 
{
    m_clear_color = clear_color;
}

inline 
v8_bool_t renderer::set_fullscreen(
    v8_bool_t const fullscreen) NOEXCEPT
{
    assert(check_if_object_state_valid());
    HRESULT ret_code = m_swap_chain->SetFullscreenState(fullscreen, nullptr);
    if (SUCCEEDED(ret_code)) {
        m_fullscreen = fullscreen;
        return true;
    }
    return false;
}

inline void 
renderer::unbind_vertex_shader() const NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->VSSetShader(nullptr, nullptr, 0);
}

inline void 
renderer::unbind_geometry_shader() const NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->GSSetShader(nullptr, nullptr, 0);
}

inline void 
renderer::unbind_fragment_shader() const NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->PSSetShader(nullptr, nullptr, 0);
}

inline FramePresentResult::Type 
renderer::present_frame(
    FramePresent::Type const flags) NOEXCEPT
{
    assert(check_if_object_state_valid());

    const UINT kFlagsMappings[] = { 0, DXGI_PRESENT_TEST  };
    HRESULT ret_code = m_swap_chain->Present(0, kFlagsMappings[flags]);

    if (ret_code == S_OK) {
        return FramePresentResult::Ok;
    } else if (ret_code == DXGI_STATUS_OCCLUDED) {
        return FramePresentResult::WindowOccluded;
    } else {
        return FramePresentResult::Error;
    }
}

inline void 
renderer::clear_backbuffer() NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->ClearRenderTargetView(
        v8::base::scoped_pointer_get(m_rendertarget_view), 
        m_clear_color.Elements
        );
}

inline void 
renderer::clear_depth_stencil() NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->ClearDepthStencilView(
        v8::base::scoped_pointer_get(m_depthstencil_view),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);
}

inline void 
renderer::reset_raster_state() NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->RSSetState(nullptr);
}

inline void 
renderer::reset_depth_stencil_state() NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->OMSetDepthStencilState(nullptr, UINT(-1));
}

inline void 
renderer::reset_blending_state() 
{
    assert(check_if_object_state_valid());
    m_device_context->OMSetBlendState(nullptr, nullptr, UINT(-1));
}

inline void 
renderer::draw(
    const v8_uint_t     vertex_count, 
    const v8_uint_t     first_vertex_location /*= 0*/) const NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->Draw(vertex_count, first_vertex_location);
}

inline 
void renderer::draw_indexed(
    const v8_uint_t index_count, 
    const v8_uint_t first_index_location /*= 0*/,
    int index_offset /*= 0*/) const NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->DrawIndexed(index_count, first_index_location, index_offset);
}

inline void 
renderer::set_rasterizer_state(
    const rasterizer_state& rsstate) NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->RSSetState(rsstate.internal_np_get_handle());
}

inline void 
renderer::reset_rasterizer_state() NOEXCEPT 
{
    assert(check_if_object_state_valid());
    m_device_context->RSSetState(nullptr);
}

inline void 
renderer::set_depth_stencil_state(
    const depth_stencil_state&      dsstate, 
    const v8_uint32_t               ref_value) NOEXCEPT
{
    assert(check_if_object_state_valid());
    m_device_context->OMSetDepthStencilState(dsstate.internal_np_get_handle(), 
                                             ref_value);
}

inline ID3D11Device* 
renderer::internal_np_get_device() const NOEXCEPT 
{
    return v8::base::scoped_pointer_get(m_device);
}

inline ID3D11DeviceContext* 
renderer::internal_np_get_device_context() const NOEXCEPT 
{
    return v8::base::scoped_pointer_get(m_device_context);
}

inline v8_bool_t 
renderer::check_if_object_state_valid() const NOEXCEPT 
{
    return  m_device && m_device_context && m_swap_chain 
            && m_depth_stencil && m_depthstencil_view
            && m_rendertarget_view;
}


} // namespace directx
} // namespace v8
