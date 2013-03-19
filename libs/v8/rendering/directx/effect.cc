#include <algorithm>
#include <functional>
#include <utility>

#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <third_party/stlsoft/platformstl/filesystem/path.hpp>
#include <third_party/stlsoft/platformstl/filesystem/memory_mapped_file.hpp>
#include <third_party/stlsoft/rangelib/sequence_range.hpp>
#include <third_party/stlsoft/stlsoft/string/static_string.hpp>

#include "v8/v8.hpp"
#include "v8/base/debug_helpers.hpp"
#include "v8/base/fixed_pod_vector.hpp"

#include "v8/global_state.hpp"
#include "v8/io/filesystem.hpp"
#include "v8/rendering/directx/internal/debug_helpers.hpp"
#include "v8/rendering/directx/internal/include_handler.hpp"
#include "v8/rendering/directx/internal/utilities.hpp"
#include "v8/rendering/directx/effect_info.hpp"
#include "v8/rendering/directx/effect_technique.hpp"
#include "v8/rendering/directx/renderer.hpp"
#include "v8/utility/hash_fnv1a.hpp"

#include "v8/rendering/directx/effect.hpp"

namespace {

const char* const kFXTargetStrings[] = {
    "fx_4_0",
    "fx_4_1",
    "fx_5_0"
};

} // anonymous namespace

v8::directx::effect::effect()
    : effect_handle_()
{}

v8::directx::effect::~effect() {}

v8_bool_t v8::directx::effect::initialize(
    const effect_info_t& eff_info, renderer* rsys
    ) {
    if (is_effect_valid()) {
        return true;
    }

        //
    // Collect defined compilation macros.
    std::vector<directx::user_defined_macro_t> user_macros;
    if (!eff_info.eff_compile_macros.empty()) {
        if (!directx::parse_macro_string(eff_info.eff_compile_macros.c_str(), 
                                         &user_macros)) {
            return false;
        }
    }

    v8::base::fixed_pod_vector<D3D10_SHADER_MACRO> api_macro_list;
    for (rangelib::sequence_range<const std::vector<directx::user_defined_macro_t>> r(user_macros);
         r; ++r) {

        D3D10_SHADER_MACRO macro = { (*r).define.c_str(), (*r).value.c_str() };
        api_macro_list.push_back(macro);
    }

    D3D10_SHADER_MACRO list_terminator = { nullptr, nullptr };
    api_macro_list.push_back(list_terminator);

    //
    // Load the effect file, compile the code and create the effect.
    platformstl::memory_mapped_file mmfile(eff_info.eff_filepath.c_str());

    v8::base::com_exclusive_pointer<ID3D10Blob>::type   compiled_bytecode;
    v8::base::com_exclusive_pointer<ID3D10Blob>::type   error_message;
    internal::compiler_include_handler                  include_handler;
    HRESULT                                             ret_code;
    const UINT dx_compile_flags = translate_compiler_options_to_directx_value(
        eff_info.eff_compile_flags);

    CHECK_D3D(
        &ret_code,
        D3DCompile(mmfile.memory(), mmfile.size(), nullptr,
                   &api_macro_list[0], &include_handler, nullptr,
                   kFXTargetStrings[eff_info.eff_target], 
                   dx_compile_flags,
                   0U,
                   v8::base::scoped_pointer_get_impl(compiled_bytecode),
                   v8::base::scoped_pointer_get_impl(error_message)));

    
    if (FAILED(ret_code)) {
        OUTPUT_DBG_MSGA("Failed to compile effect [%s]", effect_file_path.c_str());
        OUTPUT_DBG_MSGA("Compiler error [%s]", error_message->GetBufferPointer() ?
            static_cast<const char*>(error_message->GetBufferPointer()) : "not available"
            );
        return false;
    }

    base::com_exclusive_pointer<ID3DX11Effect>::type effect_interface;

    CHECK_D3D(
        &ret_code,
        D3DX11CreateEffectFromMemory(compiled_bytecode->GetBufferPointer(),
                                     compiled_bytecode->GetBufferSize(),
                                     0U, rsys->internal_np_get_device(),
                                     base::scoped_pointer_get_impl(effect_interface)));
    if (!effect_interface) {
        return false;
    }
    if (!on_effect_loaded(base::scoped_pointer_get(effect_interface))) {
        return false;
    }
    effect_handle_ = std::move(effect_interface);
    return true;
}

v8_bool_t v8::directx::effect::on_effect_loaded(ID3DX11Effect* eff) {
    //
    // Get a description of the shader.
    HRESULT             ret_code;
    CHECK_D3D(&ret_code, eff->GetDesc(&effect_info_));

    if (FAILED(ret_code)) {
        return false;
    }

    //
    // collect all techniques
    for (v8_uint_t i = 0; i < effect_info_.Techniques; ++i) {
        ID3DX11EffectTechnique* technique_handle = eff->GetTechniqueByIndex(i);

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

        ID3DX11EffectVariable* effect_var = eff->GetVariableByIndex(
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

    return true;
}

void v8::directx::effect::set_variable_raw(
    effect_param_lookup_table_t& lookup_table,
    const char* var_name, 
    const void* var_data, 
    v8_size_t byte_count
    ) {

    assert(var_name);
    assert(var_data);
    assert(is_effect_valid());

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
