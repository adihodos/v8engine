#pragma once

#include <cstring>
#include <unordered_map>
#include <vector>

#include <d3dx11effect.h>

#include <v8/v8.hpp>
#include <v8/base/com_exclusive_pointer.hpp>
#include <v8/base/operator_bool.hpp>
#include <v8/base/scoped_pointer.hpp>

#include <v8/rendering/directx/directx_resource_handle.hpp>
#include <v8/rendering/directx/effect_technique.hpp>

namespace v8 { namespace directx {

class effect : public directx_resource_traits<ID3DX11Effect> {
/// \name Defined types.
/// @{

private :
    typedef std::unordered_map<
        v8_uint_t, 
        ID3DX11EffectVariable*
    >                                               effect_param_lookup_table_t;

/// @}

/// \name Constructors.
/// @{

public :

    effect()
        :   effect_handle_()
    {
        memset(&effect_info_, 0, sizeof(effect_info_));
    }

    ~effect();

    effect(effect&& rval) {
        *this = std::move(rval);
    }

    effect& operator=(effect&& rval) {
        steal_from_rvalue(std::move(rval));
        return *this;
    }

/// @}

/// \name Attributes.
/// @{

public :

    v8_uint32_t get_const_buffer_count() const {
        assert(is_valid());
        return effect_info_.ConstantBuffers;
    }

    v8_uint32_t get_technique_count() const {
        assert(is_valid());
        return effect_info_.Techniques;
    }

    ///
    /// \brief Returns a pointer to the technique with the specified index.
    effect_technique* get_technique_by_index(v8_size_t index) const {
        assert(is_valid());
        assert(index < techniques_by_index_.size());
        return v8::base::scoped_pointer_get(techniques_by_index_[index]);
    }

    ///
    /// \brief Returns a pointer to the technique with the specified name.
    effect_technique* get_technique_by_name(const char* tech_name) const {
        auto itr = techniques_by_name_.find(tech_name);
        return itr == std::end(techniques_by_name_) ? nullptr : itr->second;
    }

/// @}

/// \name Variable access
/// @{

public :

    ///
    /// \brief Sets the value of a variable, using its name.
    template<typename T>
    inline void set_variable_by_name(
        const char* name, 
        const T& value
        );

    ///
    /// \brief Sets the value of a variable, using its name.
    template<typename T>
    inline void set_variable_by_semantic(
        const char* name, 
        const T& value
        );

    ///
    /// \brief Sets the value of a variable, using its defined semantic.
    template<typename T>
    inline void set_variable_by_name(
        const std::string& name,
        const T& value
        );

    ///
    /// \brief Sets the value of a variable, using its defined semantic.
    template<typename T>
    inline void set_variable_by_semantic(
        const std::string& name, 
        const T& value
        );

private :

    void set_variable_raw(
        effect_param_lookup_table_t& lookup_table,
        const char* var_name, 
        const void* value, 
        v8_size_t byte_count
        );

/// @}

/// \name Sanity checking.
/// @{

public :

    operator int v8::base::operator_bool::*() const {
        return is_valid() ? &v8::base::operator_bool::a_member : nullptr;
    }

    v8_bool_t is_effect_valid() const {
        return effect_handle_ && effect_handle_->IsValid();
    }

    v8_bool_t is_valid() const {
        return is_effect_valid();
    }

/// @}

/// \name Reserved, non portable functions. Must not be called by client code.
/// @{

public :

    handle_type internal_np_get_handle() const {
        return v8::base::scoped_pointer_get(effect_handle_);
    }

    void internal_np_set_handle(handle_type handle) {
        effect_handle_ = handle;
        on_effect_loaded();
    }

/// @}

/// \name Internal helper routines.
/// @{

private :
    ///
    /// \brief Called when the effect is loaded or reset. This will query
    /// the effect for the techniques, variables, etc and setup the
    /// coresponding data structures.
    void on_effect_loaded();

    ///
    /// \brief Clears any stored values.
    void reset_data() {
        memset(&effect_info_, 0, sizeof(effect_info_));
        techniques_by_index_.clear();
        techniques_by_name_.clear();
        parameters_by_name_.clear();
        parameters_by_semantic_.clear();
    }

    void steal_from_rvalue(effect&& rval) {
        effect_handle_ = std::move(rval.effect_handle_);
        memcpy(&effect_info_, &rval.effect_info_, sizeof(effect_info_));
        techniques_by_index_ = std::move(rval.techniques_by_index_);
        techniques_by_name_ = std::move(rval.techniques_by_name_);
    }

/// @}

/// \name Data members.
/// @}

private :
    
    ///< Handle to the compiled effect interface.
    v8::base::com_exclusive_pointer<ID3DX11Effect>::type        effect_handle_;

    ///< Effect information.
    D3DX11_EFFECT_DESC                                          effect_info_;

    ///< List of techniques, grouped by index.
    std::vector<v8::base::scoped_ptr<effect_technique>>         techniques_by_index_;

    ///< List of techniques, grouped by their names.
    std::unordered_map<std::string, effect_technique*>          techniques_by_name_;

    ///< List of parameters, grouped by their names.
    effect_param_lookup_table_t                                 parameters_by_name_;

    ///< List of parameters, grouped by their associated semantic.
    ///< If a parameter does not have a defined semantic, it will be
    ///< stored in the table by substituting its name for the semantic.
    effect_param_lookup_table_t                                 parameters_by_semantic_;
    
/// @}

/// \name Disabled member functions.
/// @{

private :
    NO_CC_ASSIGN(effect);

/// @}
};

} // namespace directx
} // namespace v8

template<typename T>
inline
void v8::directx::effect::set_variable_by_name(
    const char* name, 
    const T& value
    ) {
    assert(is_valid());
    set_variable_raw(parameters_by_name_, name, &value, sizeof(value));
}

template<typename T>
inline
void v8::directx::effect::set_variable_by_semantic(
    const char* name, 
    const T& value
    ) {
    assert(is_valid());
    set_variable_raw(parameters_by_semantic_, name, &value, sizeof(value));
}

template<typename T>
inline 
void v8::directx::effect::set_variable_by_name(
    const std::string& name,
    const T& value
    ) {
    set_variable_by_name(name.c_str(), value);
}

template<typename T>
inline 
void set_variable_by_semantic(
    const std::string& name, 
    const T& value
    ) {
    set_variable_by_semantic(name.c_str(), value);
}
