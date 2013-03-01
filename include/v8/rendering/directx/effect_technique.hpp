#pragma once

#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include <d3dx11effect.h>

#include <v8/v8.hpp>
#include <v8/base/scoped_pointer.hpp>
#include <v8/base/operator_bool.hpp>

#include <v8/rendering/directx/directx_resource_handle.hpp>
#include <v8/rendering/directx/effect_pass.hpp>

namespace v8 { namespace directx {

class effect_technique 
    : public directx_resource_traits<ID3DX11EffectTechnique> {

public :
    effect_technique()
        :   technique_handle_(nullptr) {
        memset(&technique_info_, 0, sizeof(technique_info_));
    }

    ~effect_technique();

    effect_technique(effect_technique&& rval) {
        *this = std::move(rval);
    }

    effect_technique& operator=(effect_technique&& rval) {
        steal_from_rvalue(std::move(rval));
        return *this;
    }

/// \name Sanity checking.
/// @{

    v8_bool_t is_valid() const {
        return technique_handle_ && technique_handle_->IsValid();
    }

    operator int v8::base::operator_bool::*() const {
        return is_valid() ? &v8::base::operator_bool::a_member : nullptr;
    }

/// @}

/// \name Attributes.
/// @{

public :

    const char* get_name() const {
        assert(is_valid());
        return technique_info_.Name;
    }

    v8_uint32_t get_pass_count() const {
        return technique_info_.Passes;
    }

    effect_pass* get_pass_by_index(v8_size_t index) const {
        assert(is_valid());
        assert(index < passes_by_index_.size());
        return v8::base::scoped_pointer_get(passes_by_index_[index]);
    }

    effect_pass* get_pass_by_name(const char* pass_name) const {
        assert(is_valid());

        auto itr = passes_by_name_.find(pass_name);
        return itr == std::end(passes_by_name_) ? itr->second : nullptr;
    }

/// @}

/// \name Reserved, non portable functions.
/// @{

public :

    void internal_np_set_handle(handle_type handle) {
        technique_handle_ = handle;
        on_technique_loaded();
    }

    handle_type internal_np_get_handle() const {
        return technique_handle_;
    }

/// @}

/// \name Internal helper routines.
/// @{

private :

    void on_technique_loaded();

    void reset_data() {
        memset(&technique_info_, 0, sizeof(technique_info_));
        passes_by_index_.clear();
        passes_by_name_.clear();
    }

    void steal_from_rvalue(effect_technique&& rval) {
        technique_handle_ = rval.technique_handle_;
        memcpy(&technique_info_, &rval.technique_info_, sizeof(technique_info_));
        passes_by_index_ = std::move(rval.passes_by_index_);
        passes_by_name_ = std::move(rval.passes_by_name_);
    }

/// @}

private :
    handle_type                                             technique_handle_;
    D3DX11_TECHNIQUE_DESC                                   technique_info_;
    std::vector<v8::base::scoped_ptr<effect_pass>>          passes_by_index_;
    std::unordered_map<std::string, effect_pass*>           passes_by_name_;

private :

    NO_CC_ASSIGN(effect_technique);
};

} // namespace directx
} // namespace v8
