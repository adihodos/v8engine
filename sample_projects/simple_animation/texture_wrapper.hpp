#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/rendering/renderer.hpp>
#include <v8/base/operator_bool.hpp>

class texture_wrapper {
public :
    texture_wrapper() {}

    v8_bool_t initialize(v8::rendering::renderer* r_sys, const char* file_path) {
        assert(r_sys);
        assert(file_path);

        assert(!is_valid());

        ID3D11Device* graphics_dev = r_sys->internal_np_get_device();
        HRESULT ret_code = D3DX11CreateShaderResourceViewFromFile(
            graphics_dev, file_path, nullptr, nullptr, 
            v8::base::scoped_pointer_get_impl(tex_srv_),
            nullptr
            );
        return ret_code == S_OK;
    }

    ID3D11ShaderResourceView* get_srv() const {
        assert(is_valid());
        return v8::base::scoped_pointer_get(tex_srv_);
    } 

    operator int v8::base::operator_bool::*() const {
        return tex_srv_ == nullptr ? nullptr : &v8::base::operator_bool::a_member;
    }  

private :
    v8_bool_t is_valid() const {
        return tex_srv_ != nullptr;
    }

private :

    v8::base::com_exclusive_pointer
    <
        ID3D11ShaderResourceView
    >::type                                             tex_srv_;

private :
    NO_CC_ASSIGN(texture_wrapper);
};