#include <algorithm>
#include <functional>
#include <utility>

#include <d3dx11effect.h>

#include <v8/v8.hpp>
#include <v8/base/debug_helpers.hpp>
#include <v8/base/fixed_pod_vector.hpp>
#include <third_party/stlsoft/stlsoft/string/static_string.hpp>

#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/rendering/directx/effect_technique.hpp"
#include "v8/rendering/directx/effect.hpp"
#include "v8/global_state.hpp"
#include "v8/utility/hash_fnv1a.hpp"

v8::directx::effect::~effect() {}

void v8::directx::effect::on_effect_loaded() {
    assert(is_effect_valid() && "Effect is not valid!");

    reset_data();

    //
    // Get a description of the shader.
    HRESULT             ret_code;
    CHECK_D3D(&ret_code, effect_handle_->GetDesc(&effect_info_));

    if (FAILED(ret_code)) {
        effect_handle_ = nullptr;
        return;
    }

    //
    // collect all techniques
    for (v8_uint_t i = 0; i < effect_info_.Techniques; ++i) {
        ID3DX11EffectTechnique* technique_handle = effect_handle_->GetTechniqueByIndex(i);

        v8::base::scoped_ptr<effect_technique> new_technique(new effect_technique());
        new_technique->internal_np_set_handle(technique_handle);

        if (!new_technique->is_valid()) {
            OUTPUT_DBG_MSGA("Warning : technique @ index %u failed to be created",
                            i);
            continue;
        }

        techniques_by_name_.insert(std::make_pair(
            new_technique->get_name(), 
            v8::base::scoped_pointer_get(new_technique)));

        techniques_by_index_.push_back(std::move(new_technique));
    }

    //
    // Collect all global variables.
    for (v8_uint32_t var_index = 0; var_index < effect_info_.GlobalVariables;
         ++var_index) {

        ID3DX11EffectVariable* effect_var = effect_handle_->GetVariableByIndex(
            var_index);

        if (!effect_var->IsValid()) {
            OUTPUT_DBG_MSGA("Warning : var with index %u reported as invalid",
                            var_index);
            continue;
        }

        HRESULT                         ret_code;
        D3DX11_EFFECT_VARIABLE_DESC     var_info;
        CHECK_D3D(&ret_code, effect_var->GetDesc(&var_info));

        if (FAILED(ret_code)) {
            OUTPUT_DBG_MSGA("Warning : failed to obtain info for variable with"
                            " index %u", var_index);
            continue;
        }

        const auto key_value_name = utility::FNV1AHash_t::hash_string(var_info.Name);
        parameters_by_name_.insert(std::make_pair(key_value_name, effect_var));

        //
        // If a variable does not have any attached semantic
        // we use its name as the semantic.
        if (!var_info.Semantic) {
            var_info.Semantic = var_info.Name;
        }

        const auto key_value_semantic = utility::FNV1AHash_t::hash_string(var_info.Semantic);
        parameters_by_semantic_.insert(std::make_pair(key_value_semantic, effect_var));
    }
}

void v8::directx::effect::set_variable_raw(
    effect_param_lookup_table_t& lookup_table,
    const char* var_name, 
    const void* var_data, 
    v8_size_t byte_count
    ) {

    assert(var_name);
    assert(var_data);

    auto var_table_entry = lookup_table.find(utility::FNV1AHash_t::hash_string(var_name));
    if (var_table_entry != lookup_table.end()) {
        assert(var_table_entry->second->IsValid());

        HRESULT ret_code;
        CHECK_D3D(&ret_code, var_table_entry->second->SetRawValue(
            var_data, 0U, static_cast<UINT>(byte_count)));
        return;
    }

    assert(false && "Trying to set variable by non existent semantic/name!");
}