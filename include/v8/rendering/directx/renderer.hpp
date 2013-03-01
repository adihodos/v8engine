#pragma once

#include <vector>

#include <D3D11.h>
#include <third_party/fw1_fontwrapper/FW1FontWrapper.h>

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/pointer_policies.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/math/color.hpp>

//#include <v8/rendering/directx/directx_input_layout.hpp>
#include <v8/rendering/directx/gpu_buffer.hpp>
#include <v8/rendering/directx/topology.hpp>
#include <v8/rendering/directx/constants.hpp>

namespace v8 {
struct resize_event;
}

namespace v8 { namespace directx {


class renderer {
public :

    //! \name Constructors.
    //! @{

    renderer(); 

    ~renderer();

    //! @}

    //! \name Event delegates and event handling.
    //! @{

public :

    void on_viewport_resized(const resize_event& ev_resize);

private :

    v8_bool_t handle_render_target_resized(
        float new_width, float new_height
    );

    //! @}

public :

    //! \name Attributes
    //! @{

    v8_bool_t is_initialized() const {
        return m_swap_chain && m_device && m_device_context;
    }

    v8_bool_t is_fullscreen() const {
        return m_fullscreen;
    }

    HWND get_target_window() const {
        return m_target_window;
    }

    float get_target_width() const {
        return m_target_width;
    }

    float get_target_height() const {
        return m_target_height;
    }

    //! @}

public :

    //! \name Initialisation
    //! @{

    v8_bool_t initialize(
        HWND window, 
        float width, 
        float height,
        ElementType::Type backbuffer_elem_type,
        v8_int_t backbuffer_elem_count,
        v8_int_t depth_bits,
        v8_int_t stencil_bits,
        v8_bool_t full_screen = false,
        v8_bool_t handle_full_screen_transition = false
        );

private :

    v8_bool_t disable_auto_alt_enter() const;

    v8_bool_t initialize_swap_chain(
        v8_bool_t fullscreen
        );

    v8_bool_t initialize_font_engine();

    //! @}

public :

    //! \name Operations
    //! @{

    inline FramePresentResult::Type present_frame(
        FramePresent::Type flags
        );


    //! @}

public :

    //! \name Input assembly stage operations.
    //! @{

    //void ia_stage_set_vertex_buffers(
    //    const VertexBuffer_t* vxbuffers, 
    //    v8_uint32_t count
    //    ) const;

    //void ia_stage_set_index_buffer(
    //    const IndexBuffer_t* ibuff, 
    //    v8_uint_t offset = 0
    //    ) const;


    void ia_stage_set_primitive_topology_type(
        PrimitiveTopology::Type topology
        ) const;

    inline void ia_stage_set_input_layout(
        //H_vertex_declaration input_layout
        void* input_layout
    );

    // ! @}

public :

    //! \name Shader stage operations.
    //! @{

    template<typename shader_proxy_type>
    inline void set_active_shader(
        shader_proxy_type& shader_proxy
        );

    inline void unbind_vertex_shader() const;

    inline void unbind_geometry_shader() const;

    inline void unbind_fragment_shader() const;

    //! @}

public :

    //! \name Output merger stage operations.
    //! @{

    void add_render_target(
        ID3D11RenderTargetView* rtv
        );

    void set_render_target(
        ID3D11RenderTargetView* rtv, 
        v8_size_t slot_id = 0
        );

    void get_render_target(
        v8_size_t slot_id,
        ID3D11RenderTargetView** rtv
        );

    //! @}

public :
    //! \name State manipulation.
    //! @{

    void set_clear_color(
        const v8::math::color_rgb& clear_color
        ) {
        m_clear_color = clear_color;
    }

    inline v8_bool_t set_fullscreen(
        v8_bool_t fullscreen
        );

    /*inline void set_rasterizer_state(
        H_rasterizer_state raster_state
        );*/

    /*inline void set_depth_stencil_state(
        H_depth_stencil_state depth_stencil_state,
        v8_uint_t reference_value
        );*/

    /*inline void set_blend_state(
        H_blend_state blend_state,
        const v8::math::color_rgb& blend_factor = v8::math::color_rgb::C_Black,
        v8_uint_t sample_mask = v8_uint_t(-1)
        );*/

    inline void clear_backbuffer();

    inline void clear_depth_stencil();

    inline void reset_raster_state();

    inline void reset_depth_stencil_state();

    inline void reset_blending_state();

    //! @}

public :

    //! \name Drawing operations
    //! @{

    inline void draw(
        v8_uint_t vertex_count,
        v8_uint_t first_vertex_location = 0
        ) const;

    inline void draw_indexed(
        v8_uint_t index_count, 
        v8_uint_t first_index_location = 0,
        int index_offset = 0
        ) const;

    void draw_string(
        const wchar_t* text, 
        float font_size, 
        float xpos, 
        float ypos, 
        const v8::math::color_rgb& color
        );

    void draw_string(
        const char* text,
        float font_size, 
        float xpos, 
        float ypos, 
        const v8::math::color_rgb& color
        );

    //! @}

public :

    //! \name Internally reserved routines.
    //! Must not be called by client code.
    //! @{

    ID3D11Device* internal_np_get_device() const {
        return v8::base::scoped_pointer_get(m_device);
    }

    ID3D11DeviceContext* internal_np_get_device_context() const {
        return v8::base::scoped_pointer_get(m_device_context);
    }

    //! @}

private :

    //! \name Sanity checking.
    //! @{

    v8_bool_t check_if_object_state_valid() const {
        return m_device && m_device_context && m_swap_chain 
            && m_depth_stencil && m_depthstencil_view
            && m_rendertarget_view;
    }

    //! @}

private :
    v8::base::com_exclusive_pointer
    <
        ID3D11Device
    >::type                                                            m_device;

    v8::base::com_exclusive_pointer
    <
        ID3D11DeviceContext
    >::type                                                    m_device_context;

    v8::base::com_exclusive_pointer
    <
        IDXGISwapChain
    >::type                                                        m_swap_chain;

    v8::base::com_exclusive_pointer
    <
        ID3D11Texture2D
    >::type                                                     m_depth_stencil;

    v8::base::com_exclusive_pointer
    <
        ID3D11RenderTargetView
    >::type                                                 m_rendertarget_view;

    v8::base::com_exclusive_pointer
    <
        ID3D11DepthStencilView
    >::type                                                 m_depthstencil_view;

    v8::base::com_exclusive_pointer
    <
        IFW1Factory
    >::type                                                      m_font_factory;

    v8::base::com_exclusive_pointer
    <
        IFW1FontWrapper
    >::type                                                      m_font_wrapper;

    HWND                                                m_target_window;

    v8::math::color_rgb                                 m_clear_color;

    float                                               m_target_width;

    float                                               m_target_height;

    v8_bool_t                                           m_fullscreen;

    DXGI_FORMAT                                         m_backbuffer_type;

    DXGI_FORMAT                                         m_depth_stencil_type;

    //
    // Bound render targets.
    std::vector<ID3D11RenderTargetView*>                m_rtvs;

private :
    //! \name Disabled functions.
    //! @{

    NO_CC_ASSIGN(renderer);

    //! @}
};

inline v8_bool_t renderer::set_fullscreen(
    v8_bool_t fullscreen
    ) {
    assert(check_if_object_state_valid());

    HRESULT ret_code = m_swap_chain->SetFullscreenState(fullscreen, nullptr);
    if (SUCCEEDED(ret_code)) {
        m_fullscreen = fullscreen;
        return true;
    }

    return false;
}

inline void renderer::ia_stage_set_input_layout(
    void* input_layout
    ) {
    assert(check_if_object_state_valid());
    //m_device_context->IASetInputLayout(input_layout);
    assert(false && "Not implemented!");
}


template<typename shader_proxy_type>
inline void renderer::set_active_shader(
    shader_proxy_type& shader_proxy
    ) {
    assert(check_if_object_state_valid());

    shader_proxy.internal_np_bind_to_pipeline(
        v8::base::scoped_pointer_get(m_device_context));
}

inline void renderer::unbind_vertex_shader() const {
    assert(check_if_object_state_valid());
    m_device_context->VSSetShader(nullptr, nullptr, 0);
}

inline void renderer::unbind_geometry_shader() const {
    assert(check_if_object_state_valid());
    m_device_context->GSSetShader(nullptr, nullptr, 0);
}

inline void renderer::unbind_fragment_shader() const {
    assert(check_if_object_state_valid());
    m_device_context->PSSetShader(nullptr, nullptr, 0);
}

inline FramePresentResult::Type renderer::present_frame(
    FramePresent::Type flags
    ) {
    assert(check_if_object_state_valid());

    const UINT kFlagsMappings[] = {
        0,
        DXGI_PRESENT_TEST
    };

    HRESULT ret_code = m_swap_chain->Present(0, kFlagsMappings[flags]);

    if (ret_code == S_OK)
        return FramePresentResult::Ok;

    if (ret_code == DXGI_STATUS_OCCLUDED)
        return FramePresentResult::WindowOccluded;

    return FramePresentResult::Error;
}

//inline void renderer::set_rasterizer_state(
//    H_rasterizer_state raster_state
//    ) {
//    assert(check_if_object_state_valid());
//    m_device_context->RSSetState(raster_state);
//}

//inline void renderer::set_depth_stencil_state(
//    H_depth_stencil_state depth_stencil_state,
//    v8_uint_t reference_value
//    ) {
//    assert(check_if_object_state_valid());
//    m_device_context->OMSetDepthStencilState(depth_stencil_state, reference_value);
//}
//
//inline void renderer::set_blend_state(
//    H_blend_state blend_state,
//    const v8::math::color_rgb& blend_factor /*= v8::math::color::Black*/,
//    v8_uint_t sample_mask /*= v8_uint_t(-1)*/
//    ) {
//    assert(check_if_object_state_valid());
//    m_device_context->OMSetBlendState(
//        blend_state, blend_factor.components_, sample_mask
//        );
//}

inline void renderer::clear_backbuffer() {
    assert(check_if_object_state_valid());
    m_device_context->ClearRenderTargetView(
        v8::base::scoped_pointer_get(m_rendertarget_view), 
        m_clear_color.components_
        );
}

inline void renderer::clear_depth_stencil() {
    assert(check_if_object_state_valid());
    m_device_context->ClearDepthStencilView(
        v8::base::scoped_pointer_get(m_depthstencil_view),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);
}

inline void renderer::reset_raster_state() {
    assert(check_if_object_state_valid());

    m_device_context->RSSetState(nullptr);
}

inline void renderer::reset_depth_stencil_state() {
    assert(check_if_object_state_valid());

    m_device_context->OMSetDepthStencilState(nullptr, UINT(-1));
}

inline void renderer::reset_blending_state() {
    assert(check_if_object_state_valid());

    m_device_context->OMSetBlendState(nullptr, nullptr, UINT(-1));
}

inline void renderer::draw(
    v8_uint_t vertex_count,
    v8_uint_t first_vertex_location /*= 0*/
    ) const {

    assert(check_if_object_state_valid());

    m_device_context->Draw(vertex_count, first_vertex_location);
}

inline void renderer::draw_indexed(
    v8_uint_t index_count, 
    v8_uint_t first_index_location /*= 0*/,
    int index_offset /*= 0*/
    ) const {

    assert(check_if_object_state_valid());

    m_device_context->DrawIndexed(index_count, first_index_location, index_offset);
}

} // namespace directx
} // namespace v8