#pragma once

#include <cassert>
#include <string>

#include <d3dx11effect.h>

#include <v8/v8.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/base/com_exclusive_pointer.hpp>

#include <v8/rendering/directx/directx_resource_handle.hpp>

namespace v8 { namespace directx {

class effect_pass : public directx_resource_traits<ID3DX11EffectPass> {
public :
    effect_pass() 
        :   pass_handle_(nullptr)
    {
        memset(&pass_info_, 0, sizeof(pass_info_));
    }

/// \name Sanity checking.
/// @{

public :

    v8_bool_t is_valid() const {
        return pass_handle_ && pass_handle_->IsValid();
    }

    operator int v8::base::operator_bool::*() const {
        return is_valid() ? &v8::base::operator_bool::a_member : nullptr;
    }

/// @}

/// \name Attributes.
/// @{

    const char* get_name() const {
        assert(is_valid());
        return pass_info_.Name;
    }

    ID3D11InputLayout* get_input_layout() const {
        assert(is_valid());
        return v8::base::scoped_pointer_get(input_layout_);
    }

    v8_bool_t has_input_signature() const {
        return static_cast<bool>(input_layout_);
    }

/// @}

/// \name Operations.
/// @{

    ///
    /// \brief Applies the settings for this pass. Also, if there is an
    /// input signature associated with the pass, it will be bound to the
    /// input assembly stage. This function must be called, before the draw call
    /// is made.
    void apply();

/// @}

/// \name Reserved, non portable functions.
/// @{

public :

    void internal_np_set_handle(handle_type handle) {
        pass_handle_ = handle;
        on_pass_loaded();
    }

    handle_type internal_np_get_handle() const {
        return pass_handle_;
    }

/// @}

/// \name Internal helper routines.
/// @{

    void on_pass_loaded();

/// @}

private :
    ///< Handle to the ID3DX11EffectPass interface.
    handle_type                                                 pass_handle_;

    ///< Pointer to the input layout signature (if present).
    v8::base::com_exclusive_pointer<ID3D11InputLayout>::type    input_layout_;

    ///< Information about the pass.
    D3DX11_PASS_DESC                                            pass_info_;

private :
    NO_CC_ASSIGN(effect_pass);
};

} // namespace directx
} // namespace v8
